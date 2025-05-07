/* C preprocessor. */

#include "cpre_internal.h"
#include "jcc.h"
#include "jcommon.h"
#include "jcount.h"
#include "jhash.h"
#include "jstring_constant.h"
#include "jvec.h"

/*TODO: generic*/
typedef struct jcc_span_t {
  char *p;
  size_t len;
} jcc_span_t;

/* TODO! */
typedef struct jcc_token_t {
  char *p;
  size_t len;
} jcc_token_t;

typedef JVEC(jcc_span_t) jcc_vec_span_t;

struct cmacro_t {
  jvec_char_t name;
  size_t ord; /* index jcc_macro_expanding_t.recurse_guard */
  /* TODO: tokenize body */
  jvec_char_t body; /* does not include args */
  jbool undef;
  jbool parens;
  jcc_vec_span_t args;
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
struct jcc_macro_expanding_t {
  jvec_uint64_t recurse_guard; /* really jvec_size_t */
  jvec_char_t chars;
};

struct jcc_ppdirective_t {
  jstring_constant_t str;
  jcc_ppdirective_handler handler;
};

int jcc_pp_define(jcc_t *jcc, jvec_char_t *body) { return 0; }

int jcc_pp_endif(jcc_t *jcc, jvec_char_t *body) { return 0; }

int jcc_pp_else(jcc_t *jcc, jvec_char_t *body) { return 0; }

int jcc_pp_elif(jcc_t *jcc, jvec_char_t *body) { return 0; }

int jcc_pp_error(jcc_t *jcc, jvec_char_t *body) { return 0; }

int jcc_pp_if(jcc_t *jcc, jvec_char_t *body) { return 0; }

int jcc_pp_include(jcc_t *jcc, jvec_char_t *body) { return 0; }

int jcc_pp_line(jcc_t *jcc, jvec_char_t *body) { return 0; }

int jcc_pp_once(jcc_t *jcc, jvec_char_t *body) { return 0; }

int jcc_pp_pragma(jcc_t *jcc, jvec_char_t *body) { return 0; }

int jcc_pp_undef(jcc_t *jcc, jvec_char_t *body) { return 0; }

jcc_ppdirective_t jcc_ppdirectives[] = {
    {{JSTRING_CONSTANT("define")}, jcc_pp_define},
    {{JSTRING_CONSTANT("endif")}, jcc_pp_endif},
    {{JSTRING_CONSTANT("elif")}, jcc_pp_elif},
    {{JSTRING_CONSTANT("else")}, jcc_pp_else},
    {{JSTRING_CONSTANT("error")}, jcc_pp_error},
    {{JSTRING_CONSTANT("if")}, jcc_pp_if},
    {{JSTRING_CONSTANT("include")}, cpre_include},
    {{JSTRING_CONSTANT("line")}, cpre_line},
    {{JSTRING_CONSTANT("once")}, cpre_once},
    {{JSTRING_CONSTANT("pragma")}, cpre_pragma},
    {{JSTRING_CONSTANT("undef")}, cpre_undef},
};

int jcc_is_ppdirective_char(int ch) {
  switch (ch) {
  case 'a': /* pragma */
  case 'c': /* include */
  case 'd': /* define */
  case 'e': /* define */
  case 'f': /* define */
  case 'g': /* pragma */
  case 'i': /* define */
  case 'm': /* pragma */
  case 'n': /* define */
  case 'l': /* line */
  case 'o': /* once */
  case 'p': /* pragma */
  case 'r': /* pragma */
  case 's': /* else */
  case 'u': /* undef */
    return 1;
  }
  return 0;
}

void jcc_report_error(const char *a, const char *b) { fprintf(stderr, a, b); }

int jcc_ppdirective_found(jcc_t *jcc, jcc_ppdirective_t *directive)
/* Read to newline and call handler. */
{
  int i = 0;
  char ch = 0;
  int err = 0;
  jvec_char_t body = {0};
  /* read to end of line or file */
  /* further handling is done by each handler */
  while (1) {
    if ((err = cpre_get_char(cpre, &i)))
      goto exit;
    if (i < 0 || i == '\n')
      break;
    ch = i;
    if ((err = JVEC_PUSH_BACK(&body, &ch)))
      goto exit;
  }
  err = directive->handler(cpre, &body);
exit:
  JVEC_CLEANUP(&body);
  return err;
}

int jcc_ppdirective(jcc_t *jcc, int ch) {
  jcc_ppdirective_t *directive = jcc_ppdirectives;
  int i = 0;
  int err = 0;
  char buf[7] = {0};
  buf[0] = ch;
  for (i = 1; i < 7; ++i) {
    if (((err = jcc_phase2_getchar(jcc, &ch))) || !jcc_is_ppdirective_char(ch))
      break;
    buf[i] = ch;
  }
  for (i = 0; i < JCOUNT(directives); ++i) {
    if (i == directive->str.size && memcmp(buf, directive->str.data, i) == 0)
      return jcc_ppdirective_found(cpre, directive);
    ++directive;
  }
  cpre_report_error("ERROR: Unknown directive %s\n", buf);
  return -1;
}

int jcc_unget_get(jcc_unget_t *unget, int *value)
/* Generic get from unget buffer (i.e. lookahead of length 1). */
{
  if (unget->valid) {
    unget->valid = 0;
    *value = unget->value;
    return 1;
  }
  return 0;
}

void jcc_unget_unget(jcc_unget_t *unget, int value)
/* Generic unget. */
{
  assert(!unget->valid);
  unget->valid = 1;
  unget->value = value;
}

void jcc_phase1_unget(jcc_t *jcc, int ch)
/* Unget next character, using newline/carriage_returns unget buffer.
 */
{
  jcc_unget_unget(&jcc->unget_char, ch);
}

int jcc_translate_space(int ch)
/* Translate whitespace except carriage return and newline to canonical space.
 * No other layer should see these characters.
 * Therefore there is one horizontal whitespace and one vertical whitespace, ' ' and '\n'.
 * Vertical whitespace is handled elsewhere.
 */
{
  switch (ch) {
  case '\f':
  case '\t':
  case '\v':
    return ' ';
  }
  return ch;
}

int jcc_phase1_getchar(jcc_t *jcc, int *pch)
/* Get next character, handling newline/carriage_returns.
 * Specifically: \n returns as \n
 * \r\n returns as \n
 * \r not followed by \n returns as \n
 * Everything else is itself (which covers \n).
 * Upon seeing \r we have to readahead 1.
 * If the readahead is \n, it accepted.
 * If the readhead is not \n, it is put back ("unget").
 * As such, no other layer should have to handle or see \r.
 *
 * Translation phase 1.
 *
 * Physical source file multibyte characters are mapped, in an implementation-
 * defined manner, to the source character set (introducing new-line characters for
 * end-of-line indicators) if necessary. Trigraph sequences are replaced by
 * corresponding single-character internal representations
 */
{
  size_t actual = 0;
  int err = 0;
  unsigned char ch = 0;
  int i = 0;

  if (cpre_unget_get(&jcc->phase1_unget, pch))
    return 0;

  if ((err = jcc->cfile.file->err)) {
    *pch = JCC_CHAR_ERROR;
    return err;
  }

  if (jcc->cfile.file->eof) {
    *pch = JCC_CHAR_END_OF_FILE;
    return 0;
  }

  if ((err = jfile_read(jcc->cfile.file, &ch, 1, &actual))) {
    *pch = JCC_CHAR_ERROR;
    return err;
  }

  if (actual == 0) {
    jcc->cfile.file->eof = 1;
    *pch = JCC_CHAR_END_OF_FILE;
    return 0;
  }

  *pch = ch;

  if (ch != '\r')
    return 0;

  /* Try to read one past \r, which could be end of file. */

  err = jfile_read(jcc->cfile.file, &ch, 1, &actual);
  if (err)
    goto return_newline;
  if (actual == 0) {
    jcc->cfile.file->eof = 1;
    goto return_newline;
  }

  if (ch != '\n')
    cpre_unget_unget(&jcc->phase1_unget, ch);
return_newline:
  *pch = '\n';
  return 0;
}

void jcc_phase2_unget(jcc_t *jcc, int ch) {
  jcc_unget_unget(&jcc->phase2_unget, ch);
}

int jcc_phase2_getchar(jcc_t *jcc, int *ch)
/* Get next character, handling line continuations.
 * This is translation phase 2.
 *
 * Each instance of a backslash character (\) immediately followed by a new-line
 * character is deleted, splicing physical source lines to form logical source lines.
 * Only the last backslash on any physical source line shall be eligible for being part
 * of such a splice. A source file that is not empty shall end in a new-line character,
 * which shall not be immediately preceded by a backslash character before any such
 * splicing takes place
 */
{
  int err = 0;
  while (1) {
    if (cpre_unget_get(&jcc->phase2_unget, ch))
      return 0;
    if (((err = jcc_phase1_getchar(jcc, ch))) || *ch != '\\')
      return err;
    if (((err = jcc_phase1_getchar(jcc, ch))) || *ch != '\n') {
      jcc_phase1_unget(jcc, *ch);
      return '\\';
    }
  }
}

int jcc_phase3_getchar(jcc_t *jcc, int *ch)
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
  int err = 0;

  /* If next character is definitely not opening a comment, return it. */
  if (((err = jcc_phase2_getchar(jcc, ch))) || *ch != '/')
    return err;
  /* If the second character does not complete the comment start, push it back
   * and return the first character. */
  if (((err = jcc_phase2_getchar(jcc, ch))) || *ch != '*') {
    jcc_phase2_unget(jcc, *ch);
    *ch = '/';
    return 0;
  }
  while (1) {
    /* Read until end of comment, yielding a space. */
    while (!((err = jcc_phase2_getchar(jcc, ch))) && *ch != '*')
      ; /* nothing */
    if (err)
      return err;
    assert(*ch == '*');
    if ((err = jcc_phase2_getchar(jcc, ch)))
      return err;
    if (*ch == '/') {
      *ch = ' ';
      return 0;
    }
    jcc_phase2_unget(jcc, *ch); /* put back in case it is star */
  }
}

int cpre_get_char(cpre_t *cpre, int *ch) {
  return cpre_get_char_handling_comments(cpre, ch);
}

int (*jcc_call_t)(jcc_t* jcc, size_t* recognized);

int jcc_preprocess_if_group(jcc_t* jcc, size_t* recognized)
/*  # if constant-expression new-line group_opt
 *  # ifdef identifier new-line group_opt
 *  # ifndef identifier new-line group_opt
*/
{
}

int jcc_preprocess_elif_group(jcc_t* jcc, size_t* recognized)
/*  # elif constant-expression new-line group_opt
 */
{
}

int jcc_preprocess_elif_groups_opt(jcc_t* jcc, size_t* recognized)
/*  elif-groups elif-group
 */
{
}

int jcc_preprocess_if_section(jcc_t* jcc, size_t* recognized)
/*   if-group elif-groups_opt else-group_opt endif-line
 */
{
}

int jcc_preprocess_control_line(jcc_t* jcc, size_t* recognized)
/*
 *  # include pp-tokens new-line
 *  # define identifier replacement-list new-line
 *  # define identifier lparen identifier-list_opt ) replacement-list new-line
 *  # define identifier lparen ... ) replacement-list new-line
 *  # define identifier lparen identifier-list , ... ) replacement-list new-line
 *  # undef identifier new-line
 *  # line pp-tokens new-line
 *  # error pp-tokens_opt new-line
 *  # pragma pp-tokens_opt new-line
 *  # new-line
 */
{
}

int jcc_preprocess_text_line(jcc_t* jcc, size_t* recognized)
/* pp-tokens_opt new-line */
{
}

int jcc_preprocess_pound_nondirective(jcc_t* jcc, size_t* recognized)
/* # pp-tokens new-line */
{
}

int jcc_preprocess_try_alternates_once(jcc_t* jcc, jcc_call_t* alternates, size_t* precognized)
{
  jcc_call_t call;
  int err = 0;
  size_t i = 0;
  while ((call = alternates[i]))
  {
    int recognized = 0;
    ++i;
    jcc_preprocess_backtrack_prepare(jcc);
    if ((err = call(jcc, &recognized)))
      return err;
    if (recognized) {
      jcc_preprocess_backtrack_cancel(jcc);
      *precognized += 1;
      return 0;
    }
    jcc_preprocess_backtrack(jcc);
  }
  return JCC_UNRECOGNIZED;
}

int jcc_preprocess_try_alternates_repeatedly(jcc_t* jcc, jcc_call_t* alternates, size_t* precognized)
{
  int err = 0;
  size_t recognized = 0;
  while (!(err = jcc_preprocess_try_alternates_once(jcc, alternates, &recognized)) && recognized)
      recognized = 0;
  return err;
}

jcc_call_t jcc_preprocess_group_parts[] = {
  jcc_preprocess_if_section,
  jcc_preprocess_control_line,
  jcc_preprocess_pound_nondirective,
  jcc_preprocess_text_line,
  0
};

int jcc_preprocess_group_part(jcc_t* jcc, size_t* precognized)
{
  return jcc_preprocess_try_alternates_once(jcc, jcc_preprocess_group_parts, precognized);
}

int jcc_preprocess_group(jcc_t* jcc)
{
  int err = 0;
  size_t recognized = 0;
  while (!(err = jcc_jcc_preprocess_group_part(jcc, &recognized)) && recognized)
    recognized = 0;
  return err;
}

int jcc_preprocess_group_opt(jcc_t* jcc)
{
  size_t recognized = 0;
  return jcc_preprocess_group(jcc, &recognized);
}

int jcc_preprocess_file(jcc_t* jcc)
{
  return jcc_preprocess_group_opt(jcc);
}

int jcc_preprocess_get_token(jcc_t *jcc, jcc_preprocess_token_t** pptoken)
/* Read a preprocessing token from the preprocessor.
 *
 * This is meant to become translation phase 3.
 */
{
  int ch = 0;
  int err = 0;

  if ((*pptoken = JBASE(jcc_preprocess_token_t, list, jlist_remove_first(jcc->preprocess_tokens))))
    return 0;

  while (1) {
    err = jcc_phase3_getchar(jcc, &ch);
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
          cpre_directive(jcc, ch);
        }
        break;
    }
}

int jcc_get_token(jcc_t *jcc, jcc_token_t** pptoken)
/* Read a C token from the preprocessor.
 * This handles #include, #define, #undef, #if, #ifdef, etc.
 *
 * This is meant to become translation phase 4.
 */
{
  int ch = 0;
  int err = 0;

  if ((*pptoken = JBASE(jcc_preprocess_token_t, list, jlist_remove_first(jcc->preprocess_tokens))))
    return 0;

  err = jcc_phase3_getchar(jcc, &ch);
  int (*get)(jcc_t *, int *) = cpre_get_char;
  int start_of_line = 1;
  int pound = 0;
  int ch = 0;
  int err = 0;
  if ((err = get(jcc, &ch)))
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
      cpre_directive(jcc, ch);
    }
    break;
  }
exit:
  return err;
}
