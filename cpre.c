/* C preprocessor. */

#include "jcommon.h"
#include "jhash.h"

#if 0 /* work in progress */

struct cpre_t;
typedef struct cpre_t cpre_t;

typedef struct cmacro_t {
  jvec_char_t name;
  long nary; /* or <0 */
  long ord;
  /* TODO: tokenize body? */
  jvec_char_t body;
} cmacro_t;

/* Every phase has a get and unget. unget requires a buffer.
 * TODO: expand to arbitrrary size? jvec?
 * TODO: Rename to lookahead?
 */
typedef struct cpre_unget_t {
  int value;
  int valid;
} cpre_unget_t;

typedef struct cpre_t {
  /* Every macro is assigned an ordinal, to index the bits
   * of cpre_expanding_t.
   */
  long macro_ord;
  jhash_t macros;
  cfile_t cfile;

  /* Every phase has a get and unget. unget requires a buffer. */
  cpre_unget_t unget_char;
  cpre_unget_t unget_line_cont;
} cpre_t;

/* Because macros cannot recurse, expanding a macro is stateful.
 * i.e. every expanded macro needs a bool
 * This is that state.
 * e.g. define FOO FOO BAR is valid and makes FOO expand to FOO BAR
 * BAR may be subject to further expansion but FOO is not.
 */
typedef struct cpre_expanding_t {
  jvec_uint64_t recurse_guard;
  jvec_char_t chars;
} cpre_expanding_t;

void (*cpre_directive_handler)(cpre_t *);

typedef struct cpre_directive_t {
  jstring_constant_t str;
  cpre_directive_handler handler;
} cpre_directive_t;

int cpre_if(cpre_t *cpre) { return 0; }

int cpre_endif(cpre_t *cpre) { return 0; }

int cpre_else(cpre_t *cpre) { return 0; }

int cpre_elif(cpre_t *cpre) { return 0; }

int cpre_line(cpre_t *cpre) { return 0; }

directive_t directives[] = {
    {JSTRING_CONSTANT("if"), cpre_if},
    {JSTRING_CONSTANT("endif"), cpre_endif},
    {JSTRING_CONSTANT("elif"), cpre_elif},
    {JSTRING_CONSTANT("else"), cpre_else},
    {JSTRING_CONSTANT("define"), cpre_else},
    {JSTRING_CONSTANT("undef"), cpre_undef},
};

int cpre_unget_get(cpre_unget_t *unget, int *value)
/* Generic get from unget buffer (i.e. lookahead of length 1). */
{
  if (unget->valid) {
    unget->valid = 0;
    *value = unget->value;
    return 1;
  }
  return 0;
}

void cpre_unget_unget(cpre_unget_t *unget, int value)
/* Generic unget. */
{
  assert(!unget->valid);
  unget->valid = 1;
  unget->value = value;
}

void cpre_unget_char(cpre_t *cpre, int ch)
/* Unget next character, using newline/carriage_returns unget buffer. */
{
  cpre_unget_unget(&cpre->unget_char, ch);
}

int cpre_translate_space(int ch) {
  switch (ch) {
  case '\f':
  case '\t':
  case '\v':
    return ' ';
  }
  return ch;
}

int cpre_get_char(cpre_t *cpre)
/* Get next character, handling newline/carriage_returns. */
{
  int ch = 0;
  if (cpre_unget_get(&cpre->unget_char, &ch))
    return ch;
  ch = fgetc(jcc->cfile);
  if (ch != '\r')
    return ch;
  ch = fgetc(jcc->cfile);
  if (ch != '\n')
    cpre_unget_unget(&cpre->unget_char, ch);
  return '\n';
}

void cpre_unget_line_cont(cpre_t *cpre, int ch) {
  cpre_unget_unget(&cpre->unget_line_cont, ch);
}

int cpre_get_line_cont(cpre_t *cpre)
/* Get next character, handling line continuations. */
{
  int (*get)(cpre_t *) = cpre_get_char;
  int (*unget)(cpre_t *, int) = cpre_unget_unchar;
  int ch = 0;
  if (cpre_unget_get(&cpre->unget_line_cont, &ch))
    return ch;
  while (1) {
    int ch = get(cpre);
    if (ch != '\\')
      return ch;
    ch = get(cpre);
    if (ch != '\n') {
      unget(cpre, ch);
      return '\\';
    }
  }
}

int cpre_get_comment(cpre_t *cpre) {
  int (*get)(cpre_t *) = cpre_get_line_cont;
  int (*unget)(cpre_t *, int) = cpre_unget_line_cont;
  int ch = get(cpre);
  if (cpre_unget_get(&cpre->unget_comment, &ch))
    return ch;
  if (ch != '/')
    return ch;
  ch = get(cpre);
  if (ch != '*') {
    unget(cpre, ch);
    return '/';
  }
  while (1) {
    /* Read until end of comment, yielding a space. */
    while ((ch = get(cpre)) != '*')
      ;
    ch = get(cpre);
    if (ch == '/')
      return ' ';
    unget(cpre, ch); /* put back in case it is star */
  }
}

int cpre_get_token() {
  int (*get)(cpre_t *) = cpre_get_comment;
  int start_of_line = 1;
  int pound = 0;
  int ch = get(cpre);
  switch (ch) {
  case '\n':
    start_of_line = 1;
    break;
  case '\v':
  case '\f':
  case '\t':
  case ' ':
    break;
  case '#':
    pound = start_of_line;
    start_of_line = 0;
    break;
  default:
    if (pound) {
      cpre_directive(cpre);
    }
    break;
  }
}

#endif
