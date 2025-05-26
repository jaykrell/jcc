/* C preprocessor. */

#include "jcc.h"
#include "jcommon.h"
#include "jcount.h"
#include "jhash.h"
#include "jmem.h"
#include "jstring_constant.h"
#include "jvec.h"
#if _MSC_VER
#pragma warning(disable : 4100) /* unused parameter */
#endif

/*TODO: generic*/
typedef struct jcc_span_t {
  char *p;
  size_t len;
} jcc_span_t;

#if 0
/* TODO! */
typedef struct jcc_token_t {
  char *p;
  size_t len;
} jcc_token_t;
#endif

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

struct jcc_preprocess_directive_t {
  jstring_constant_t str;
  jcc_preprocess_directive_handler_t handler;
};

int jcc_preprocess_define(jcc_t *jcc, jvec_char_t *body) { return 0; }

int jcc_preprocess_endif(jcc_t *jcc, jvec_char_t *body) { return 0; }

int jcc_preprocess_else(jcc_t *jcc, jvec_char_t *body) { return 0; }

int jcc_preprocess_elif(jcc_t *jcc, jvec_char_t *body) { return 0; }

int jcc_preprocess_error(jcc_t *jcc, jvec_char_t *body) { return 0; }

int jcc_preprocess_if(jcc_t *jcc, jvec_char_t *body) { return 0; }

int jcc_preprocess_include(jcc_t *jcc, jvec_char_t *body) { return 0; }

int jcc_preprocess_line(jcc_t *jcc, jvec_char_t *body) { return 0; }

int jcc_preprocess_once(jcc_t *jcc, jvec_char_t *body) { return 0; }

int jcc_preprocess_pragma(jcc_t *jcc, jvec_char_t *body) { return 0; }

int jcc_preprocess_undef(jcc_t *jcc, jvec_char_t *body) { return 0; }

jcc_preprocess_directive_t jcc_preprocess_directives[] = {
    {{JSTRING_CONSTANT("define")}, jcc_preprocess_define},
    {{JSTRING_CONSTANT("endif")}, jcc_preprocess_endif},
    {{JSTRING_CONSTANT("elif")}, jcc_preprocess_elif},
    {{JSTRING_CONSTANT("else")}, jcc_preprocess_else},
    {{JSTRING_CONSTANT("error")}, jcc_preprocess_error},
    {{JSTRING_CONSTANT("if")}, jcc_preprocess_if},
    {{JSTRING_CONSTANT("include")}, jcc_preprocess_include},
    {{JSTRING_CONSTANT("line")}, jcc_preprocess_line},
    {{JSTRING_CONSTANT("once")}, jcc_preprocess_once},
    {{JSTRING_CONSTANT("pragma")}, jcc_preprocess_pragma},
    {{JSTRING_CONSTANT("undef")}, jcc_preprocess_undef},
};

int jcc_is_preprocess_directive_char(int ch) {
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

int jcc_preprocess_directive_found(jcc_t *jcc,
                                   jcc_preprocess_directive_t *directive)
/* Read to newline and call handler. */
{
  int i = 0;
  char ch = 0;
  int err = 0;
  jvec_char_t body = {0};
  /* read to end of line or file */
  /* further handling is done by each handler */
  while (1) {
    if ((err = jcc_getchar(jcc, &i)))
      goto exit;
    if (i < 0 || i == '\n')
      break;
    ch = (char)i;
    if ((err = JVEC_PUSH_BACK(&body, &ch)))
      goto exit;
  }
  err = directive->handler(jcc, &body);
exit:
  JVEC_CLEANUP(&body);
  return err;
}

int jcc_preprocess_find_directive(jcc_t *jcc, int ch) {
  jcc_preprocess_directive_t *directive = 0;
  ;
  int i = 0;
  int err = 0;
  char buf[7] = {0};

  for (i = 0; i < 7; ++i)
    buf[i] = 0;
  directive = jcc_preprocess_directives;
  buf[0] = (char)ch;

  for (i = 1; i < 7; ++i) {
    err = jcc_getchar(jcc, &ch);
    if (err || !jcc_is_preprocess_directive_char(ch))
      break;
    buf[i] = (char)ch;
  }
  for (i = 0; i < JCOUNT(jcc_preprocess_directives); ++i) {
    if (i == directive->str.size && memcmp(buf, directive->str.data, i) == 0)
      return jcc_preprocess_directive_found(jcc, directive);
    ++directive;
  }
  jcc_report_error("ERROR: Unknown directive %s\n", buf);
  return -1;
}

int jcc_translate_space(int ch)
/* Translate whitespace except carriage return and newline to canonical space.
 * No other layer should see these characters.
 * Therefore there is one horizontal whitespace and one vertical whitespace, ' '
 * and '\n'. Vertical whitespace is handled elsewhere.
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

#if 0
int cpre_get_char(cpre_t *cpre, int *ch) {
  return cpre_get_char_handling_comments(cpre, ch);
}
#endif

typedef int (*jcc_call_t)(jcc_t *jcc, size_t *recognized);

int jcc_preprocess_if_group(jcc_t *jcc, size_t *recognized)
/*  # if constant-expression new-line group_opt
 *  # ifdef identifier new-line group_opt
 *  # ifndef identifier new-line group_opt
 */
{
  return -1;
}

int jcc_preprocess_elif_group(jcc_t *jcc, size_t *recognized)
/*  # elif constant-expression new-line group_opt
 */
{
  return -1;
}

int jcc_preprocess_elif_groups_opt(jcc_t *jcc, size_t *recognized)
/*  elif-groups elif-group
 */
{
  return -1;
}

int jcc_preprocess_if_section(jcc_t *jcc, size_t *recognized)
/*   if-group elif-groups_opt else-group_opt endif-line
 */
{
  return -1;
}

jcc_token_t jcc_token_define;
jcc_token_t jcc_token_error;
jcc_token_t jcc_token_line;
jcc_token_t jcc_token_include;
jcc_token_t jcc_token_pragma;
jcc_token_t jcc_token_undef;

void jcc_initialize_tokens(void) {}

int jcc_lex_candidate_token(jcc_t *jcc, jcc_token_t *candidate,
                            size_t *recognized) {
  return -1;
}

int jcc_preprocess_control_line(jcc_t *jcc, size_t *recognized)
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
  int err;
  int ch;
  if (jcc->ch != '#')
    return JCC_UNRECOGNIZED;
  while (1) {
    err = jcc_getchar(jcc, &ch);
    if (err)
      return err;
    switch (ch) {
    case '\n':
      return 1;
    case 'd':
      return jcc_lex_candidate_token(jcc, &jcc_token_define, recognized);
    case 'u':
      return jcc_lex_candidate_token(jcc, &jcc_token_undef, recognized);
    case 'l':
      return jcc_lex_candidate_token(jcc, &jcc_token_line, recognized);
    case 'e':
      return jcc_lex_candidate_token(jcc, &jcc_token_error, recognized);
    case 'i':
      return jcc_lex_candidate_token(jcc, &jcc_token_include, recognized);
    case 'p':
      return jcc_lex_candidate_token(jcc, &jcc_token_pragma, recognized);
    }
  }
  return JCC_UNRECOGNIZED;
}

int jcc_preprocess_text_line(jcc_t *jcc, size_t *recognized)
/* pp-tokens_opt new-line */
{
  return -1;
}

int jcc_preprocess_pound_nondirective(jcc_t *jcc, size_t *recognized)
/* # pp-tokens new-line */
{
  return -1;
}

int jcc_preprocess_try_alternates_once(jcc_t *jcc, jcc_call_t *alternates,
                                       size_t *precognized) {
  jcc_call_t call;
  int err = 0;
  size_t i = 0;
  while ((call = alternates[i])) {
    size_t recognized = 0;
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

int jcc_preprocess_try_alternates_repeatedly(jcc_t *jcc, jcc_call_t *alternates,
                                             size_t *precognized) {
  int err = 0;
  size_t recognized = 0;
  while (!(err = jcc_preprocess_try_alternates_once(jcc, alternates,
                                                    &recognized)) &&
         recognized)
    recognized = 0;
  return err;
}

jcc_call_t jcc_preprocess_group_parts[] = {
    jcc_preprocess_if_section, jcc_preprocess_control_line,
    jcc_preprocess_pound_nondirective, jcc_preprocess_text_line, 0};

int jcc_preprocess_group_part(jcc_t *jcc, size_t *precognized) {
  return jcc_preprocess_try_alternates_once(jcc, jcc_preprocess_group_parts,
                                            precognized);
}

int jcc_preprocess_group(jcc_t *jcc) {
  int err = 0;
  size_t recognized = 0;
  while (!(err = jcc_preprocess_group_part(jcc, &recognized)) && recognized)
    recognized = 0;
  return err;
}

int jcc_preprocess_group_opt(jcc_t *jcc) { return jcc_preprocess_group(jcc); }

int jcc_preprocess_file(jcc_t *jcc) { return jcc_preprocess_group_opt(jcc); }

int jcc_preprocess_get_identifier(jcc_t *jcc, jvec_char_t *identifier) {
  identifier->size = 0;
  return -1;
}

int jcc_is_identifier_char(int);

int jcc_preprocess_pound_lex(jcc_t *jcc, int ch) {
  char directive[32];
  int i = 1;
  int err = 0;

  jmemset0(directive, sizeof(directive));
  directive[0] = (char)ch;
  while (i < JCOUNT(directive) && jcc_is_identifier_char(ch)) {
    err = jcc_getchar(jcc, &ch);
    if (err)
      return err;
    directive[i] = (char)ch;
    ++i;
  }
  return -1;
}

int jcc_preprocess_get_token(jcc_t *jcc, jcc_token_t **pptoken)
/* Read a preprocessing token from the preprocessor.
 *
 * This is meant to become translation phase 3 or 4.
 *
 * Preprocessor tokens are similar to tokens, but not always the same.
 */
{
  int ch = 0;
  int err = 0;
  int pound = 0;
  int start_of_line = 0;
  jcc_token_t *ptoken;

  while (1) {
    /* Handle queuing and backtracking. */
    *pptoken = 0;
    ptoken = JBASE(jcc_token_t, list, jlist_remove_first(&jcc->tokens));
    if (ptoken) {
      *pptoken = ptoken;
      return 0;
    }
    err = jcc_getchar(jcc, &ch);
    switch (ch) {
    case '\r':
    case '\n':
      start_of_line = 1;
      break;
    /* Vertical whitespace does not change start_of_line. */
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
        err = jcc_preprocess_pound_lex(jcc, ch);
      }
      break;
    }
  }
  return 0;
}

int jcc_get_token(jcc_t *jcc, jcc_token_t **pptoken)
/* Read a C token from the preprocessor.
 * C tokens and preprocessor tokens are almost but not quite the same.
 * This handles #include, #define, #undef, #if, #ifdef, etc.
 *
 * This is meant to become translation phase 4.
 */
{
  return 0;
}
