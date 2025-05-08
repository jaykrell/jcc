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

int jcc_preprocess_find_directive(jcc_t *jcc, int ch) {
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
 * This is meant to become translation phase 3 or 4.
 */
{
  int ch = 0;
  int err = 0;
  int pound = 0;
  int start_of_line = 0;

  /* Handle backtracking. */
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
