/* C preprocessor. */

#include "cpre_internal.h"
#include "jcc.h"
#include "jcommon.h"
#include "jcount.h"
#include "jhash.h"
#include "jstring_constant.h"

struct cmacro_t {
  jvec_char_t name;
  long nary;  /* or <0 */
  size_t ord; /* index cpre_expanding_t.recurse_guard */
  /* TODO: tokenize body? */
  jvec_char_t body;
  jbool undef;
};

/* Every phase has a get and unget. unget requires a buffer.
 * TODO: expand to arbitrrary size? jvec?
 * TODO: Rename to lookahead?
 */
struct cpre_unget_t {
  int value;
  int valid;
};

struct cpre_t {
  /* Every macro is assigned an ordinal, to index the bits
   * of cpre_expanding_t. That is macros.size.
   * To assist this, undef will mark the entry without deleting it.
   */
  jhash_t macros;
  cfile_t cfile;

  /* Every phase has a get and unget. unget requires a buffer. */
  cpre_unget_t unget_char;
  cpre_unget_t unget_comment;
  cpre_unget_t unget_line_cont;
};

/* Because macros cannot recurse, expanding a macro is stateful.
 * i.e. every expanded macro needs a bool
 * This is that state.
 * e.g. define FOO FOO BAR is valid and makes FOO expand to FOO BAR
 * BAR may be subject to further expansion but FOO is not.
 */
struct cpre_expanding_t {
  jvec_uint64_t recurse_guard; /* really jvec_size_t */
  jvec_char_t chars;
};

struct cpre_directive_t {
  jstring_constant_t str;
  cpre_directive_handler handler;
};

int cpre_if(cpre_t *cpre) { return 0; }

int cpre_include(cpre_t *cpre) { return 0; }

int cpre_endif(cpre_t *cpre) { return 0; }

int cpre_else(cpre_t *cpre) { return 0; }

int cpre_elif(cpre_t *cpre) { return 0; }

int cpre_line(cpre_t *cpre) { return 0; }

int cpre_undef(cpre_t *cpre) { return 0; }

cpre_directive_t directives[] = {
    {{JSTRING_CONSTANT("define")}, cpre_else},
    {{JSTRING_CONSTANT("endif")}, cpre_endif},
    {{JSTRING_CONSTANT("elif")}, cpre_elif},
    {{JSTRING_CONSTANT("else")}, cpre_else},
    {{JSTRING_CONSTANT("if")}, cpre_if},
    {{JSTRING_CONSTANT("include")}, cpre_include},
    {{JSTRING_CONSTANT("line")}, cpre_line},
    {{JSTRING_CONSTANT("undef")}, cpre_undef},
};

int cpre_is_directive_char(int ch) {
  switch (ch) {
  case 'c':
  case 'd':
  case 'e':
  case 'f':
  case 'i':
  case 'n':
  case 'u':
  case 'l':
  case 's':
    return 1;
  }
  return 0;
}

void cpre_error(const char *a, const char *b) { fprintf(stderr, a, b); }

int cpre_directive_found(cpre_t *cpre, cpre_directive_t *directive)
/* Read to newline and call handler. */
{
  jvec_char_t body = {0};
  return directive->handler(cpre);
}

int cpre_directive(cpre_t *cpre, int ch) {
  cpre_directive_t *directive = directives;
  int i = 0;
  int err = 0;
  char buf[7] = {0};
  buf[0] = ch;
  for (i = 1; i < 7; ++i) {
    if (((err = cpre_get_char(cpre, &ch))) || !cpre_is_directive_char(ch))
      break;
    buf[i] = ch;
  }
  for (i = 0; i < JCOUNT(directives); ++i) {
    if (i == directive->str.size && memcmp(buf, directive->str.data, i) == 0)
      return cpre_directive_found(cpre, directive);
    ++directive;
  }
  cpre_error("ERROR: Unknown directive %s\n", buf);
  return -1;
}

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

void cpre_unget_char_handling_newlines_and_carriage_returns(cpre_t *cpre,
                                                            int ch)
/* Unget next character, using newline/carriage_returns unget buffer. */
{
  cpre_unget_unget(&cpre->unget_char, ch);
}

int cpre_translate_space(int ch)
/* Translate whitespace except carriage return and newline to canonical space.
 */
/* No other layer should see these characters. */
/* Therefore there is one horizontal whitespace and one vertical whitespace, ' '
   and '\n'. */
/* Vertical whitespace is handled elsewhere. */
{
  switch (ch) {
  case '\f':
  case '\t':
  case '\v':
    return ' ';
  }
  return ch;
}

int cpre_get_char_handling_newlines_and_carriage_returns(cpre_t *cpre, int *ch)
/* Get next character, handling newline/carriage_returns.
 * Specifically: \n returns as \n
 * \r\n returns as \n
 * \r not followed by \n returns as \n
 * Everything else is itself (which covers \n).
 * Upon seeing \r we have to readahead 1.
 * If the readahead is \n, it accepted.
 * If the readhead is not \n, it is put back ("unget").
 *
 * As such, no other layer should have to handle or see \r.
 */
{
  size_t actual = 0;
  int err = 0;

  if (cpre_unget_get(&cpre->unget_char, ch))
    return 0;

  if (cpre->cfile.file->eof) {
    *ch = JFILE_END;
    return 0;
  }

  if ((err = jfile_read(cpre->cfile.file, ch, 1, &actual)))
    return err;

  if (actual == 0) {
    cpre->cfile.file->eof = 1;
    *ch = JFILE_END;
    return 0;
  }

  if (*ch != '\r')
    return 0;

  /* Try to read one past \r, which could be end of file. */

  err = jfile_read(cpre->cfile.file, ch, 1, &actual);
  if (err)
    goto return_newline;
  if (actual == 0) {
    cpre->cfile.file->eof = 1;
    goto return_newline;
  }

  if (*ch != '\n')
    cpre_unget_unget(&cpre->unget_char, *ch);
return_newline:
  *ch = '\n';
  return 0;
}

void cpre_unget_char_handling_line_continuations(cpre_t *cpre, int ch) {
  cpre_unget_unget(&cpre->unget_line_cont, ch);
}

int cpre_get_char_handling_line_continuations(cpre_t *cpre, int *ch)
/* Get next character, handling line continuations. */
{
  int (*get)(cpre_t *, int *) =
      cpre_get_char_handling_newlines_and_carriage_returns;
  void (*unget)(cpre_t *, int) =
      cpre_unget_char_handling_newlines_and_carriage_returns;
  int err = 0;
  while (1) {
    if (cpre_unget_get(&cpre->unget_line_cont, ch))
      return 0;
    if (((err = get(cpre, ch))) || *ch != '\\')
      return err;
    if (((err = get(cpre, ch))) || *ch != '\n') {
      unget(cpre, *ch);
      return '\\';
    }
  }
}

int cpre_get_char_hndling_comments(cpre_t *cpre, int *ch)
/* C preprocessor scanning.
 * Read, at the phase that handles comments, turning them into spaces.
 *
 * Read a character. If it is not '/', return it.
 * If it is '/', read ahead another. If that is not '*' push it back and return
 * '/'. If it is '*', then read until closing '*' and '/', which is similar to
 * opening. Matters such as line continuation with backward slashes are handled
 * at a different layer.
 * TODO: C99/C++ comments.
 */
{
  int (*get)(cpre_t *, int *) = cpre_get_char_handling_line_continuations;
  void (*unget)(cpre_t *, int) = cpre_unget_char_handling_line_continuations;
  int err = 0;
  /* If our caller did an unget to this layer, return that. */
  if (cpre_unget_get(&cpre->unget_comment, ch))
    return 0;
  /* If next character is definitely not opening a comment, return it. */
  if (((err = get(cpre, ch))) || *ch != '/')
    return err;
  /* If the second character does not complete the comment start, push it back
   * and return the first character. */
  if (((err = get(cpre, ch))) || *ch != '*') {
    unget(cpre, *ch);
    *ch = '/';
    return 0;
  }
  while (1) {
    /* Read until end of comment, yielding a space. */
    while (!((err = get(cpre, ch))) && *ch != '*')
      ; /* nothing */
    if (err)
      return err;
    assert(*ch == '*');
    if ((err = get(cpre, ch)))
      return err;
    if (*ch == '/') {
      *ch = ' ';
      return 0;
    }
    unget(cpre, *ch); /* put back in case it is star */
  }
}

int cpre_get_char(cpre_t *cpre, int *ch) {
  return cpre_get_char_hndling_comments(cpre, ch);
}

int cpre_get_token(cpre_t *cpre)
/* Read a C token from the preprocessor.
 * This handles #include, #define, #undef, #if, #ifdef.
 */
{
  int (*get)(cpre_t *, int *) = cpre_get_char;
  int start_of_line = 1;
  int pound = 0;
  int ch = 0;
  int err = 0;
  if ((err = get(cpre, &ch)))
    goto exit;
  assert(ch != '\v');
  assert(ch != '\f');
  assert(ch != '\t');
  assert(ch != '\r');
  switch (ch) {
  case '\r':
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
      cpre_directive(cpre, ch);
    }
    break;
  }
exit:
  return err;
}
