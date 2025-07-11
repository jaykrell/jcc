/* C preprocessor. */

#include "jcc.h"
#include "jcommon.h"
#include "jcount.h"
#include "jhash.h"
#include "jmem.h"
#include "jstring_constant.h"
#include "jvec.h"
#include <string.h>
#if _MSC_VER
#pragma warning(disable : 4100) /* unused parameter */
#endif

jcc_char_class_t jcc_char_class[256];
jbool jcc_space[256];
jcc_lex_trie_t jcc_lex_trie0[256];
jcc_token_t jcc_token_identifier;
jcc_token_t jcc_token_string_constant;
jcc_token_t jcc_token_character_constant[256];

/* keywords */
jcc_token_t jcc_token_auto;
jcc_token_t jcc_token_char;
jcc_token_t jcc_token_do;
jcc_token_t jcc_token_for;
jcc_token_t jcc_token_double;
jcc_token_t jcc_token_float;
jcc_token_t jcc_token_goto;
jcc_token_t jcc_token_int;
jcc_token_t jcc_token_long;
jcc_token_t jcc_token_struct;
jcc_token_t jcc_token_static;
jcc_token_t jcc_token_typedef;
jcc_token_t jcc_token_union;
jcc_token_t jcc_token_while;

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

int jcc_char_to_lower(int ch)
{
  for (i = 'A'; i <= 'Z'; ++i) {
    jcc_char_class[i] = jcc_char_alpha;
    jcc_char_class[i - 'A' + 'a'] = jcc_char_alpha;
}

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

int jcc_char_space_first = 9;
int jcc_char_space_last = 13;
int jcc_char_tab = 9;              /* horizontal tab \t */
int jcc_char_newline = 10;         /* also known as linefeed or \n */
int jcc_char_vertical_tab = 11;    /* \v */
int jcc_char_formfeed = 12;        /* \f */
int jcc_char_carriage_return = 13; /* \r */

int jcc_is_space(int ch) { return ch >= 9 && ch <= 13; }

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

jcc_lex_trie_t jcc_lex_trie;

void jcc_init_trie(const char *str, jcc_token_t *token) {
  unsigned char ch;
  jcc_lex_trie_t *trie;

  trie = &jcc_lex_trie;
  while ((ch = (unsigned char)*str++)) {
    if (!trie->map[ch])
      trie->map[ch] = (jcc_lex_trie_t *)calloc(1, sizeof(jcc_lex_trie_t));
    trie = trie->map[ch];
  }
  trie->token = token;
}

jcc_token_t jcc_token_and;
jcc_token_t jcc_token_ands;
jcc_token_t jcc_token_assign;
jcc_token_t jcc_token_bar;
jcc_token_t jcc_token_bars;
jcc_token_t jcc_token_colon;
jcc_token_t jcc_token_comma;
jcc_token_t jcc_token_define;
jcc_token_t jcc_token_dot;
jcc_token_t jcc_token_dots;
jcc_token_t jcc_token_eq;
jcc_token_t jcc_token_error;
jcc_token_t jcc_token_exclaim;
jcc_token_t jcc_token_ge;
jcc_token_t jcc_token_greater;
jcc_token_t jcc_token_include;
jcc_token_t jcc_token_lbrace;
jcc_token_t jcc_token_lbracket;
jcc_token_t jcc_token_le;
jcc_token_t jcc_token_left_shift;
jcc_token_t jcc_token_less;
jcc_token_t jcc_token_line;
jcc_token_t jcc_token_lparen;
jcc_token_t jcc_token_minuss;
jcc_token_t jcc_token_neq;
jcc_token_t jcc_token_newline;
jcc_token_t jcc_token_percent;
jcc_token_t jcc_token_plus;
jcc_token_t jcc_token_pluss;
jcc_token_t jcc_token_pound;
jcc_token_t jcc_token_pounds;
jcc_token_t jcc_token_pragma;
jcc_token_t jcc_token_question;
jcc_token_t jcc_token_rbrace;
jcc_token_t jcc_token_rbracket;
jcc_token_t jcc_token_right_shift;
jcc_token_t jcc_token_rparen;
jcc_token_t jcc_token_semi;
jcc_token_t jcc_token_slash;
jcc_token_t jcc_token_star;
jcc_token_t jcc_token_tilde;
jcc_token_t jcc_token_undef;

int jcc_lex_candidate_token(jcc_t *jcc, jbool prefer_header_name,
                            jcc_token_t *candidate, jcc_token_t **success,
                            size_t *recognized) {
  return -1;
}

int jcc_pp_token(jcc_t *jcc, jbool prefer_header_name, jcc_token_t **token) {
  /* longest whitespace-separated sequence that can be a single token
  with special handling regarding "prefer_header_name" vs. string. */

  size_t size = 0;
  int ch = 0;
  unsigned char uch = 0;
  int err = 0;
  jcc_lex_trie_t *trie = 0;
  int indefinite = 0;
  jcc_char_traits traits={0};

  *token = JBASE(jcc_token_t, pp_queued, jlist_remove_first(&jcc->pp_queued_tokens));
  if (*token)
	  return 0;

  trie = &jcc_lex_trie;

  while (1) {
    err = jcc_getchar(jcc, &ch);
    uch = (unsigned char)ch;
    if (err && err != JCC_CHAR_END_OF_FILE)
      return err;
	traits = jcc_char_traits[uch];
	if (size == 0)
		indefinite = traits.indefinite;
    /* TODO: prefer_header_name */
    if (traits.is_space)
      break;
    ++size;
	if (trie->map[uch])
		trie = trie->map[uch];
	else if (!indefinite)
		break;
  }
  if (!indefinite)
	  return trie->token;
  /* indefinite is identifier and number and string constant
  and todo: character constant */
  return -1;
}

int jcc_lex_identifier(jcc_t *jcc, jcc_token_t **identifier) { return -1; }

int jcc_lex_define_parameters(jcc_t *jcc, jcc_token_t **replacement_list)
/* Read a list of comma separated identifiers for a define
 * TODO: Variadic and possibly empty.
 */
{
  return -1;
}

int jcc_lex_replacement_list(jcc_t *jcc, jcc_token_t **replacement_list)
/* Body of a macro.
 */
{
  return -1;
}

int jcc_new_token(jcc_t *jcc, jcc_token_t **token) {
  if (!(*token = (jcc_token_t *)calloc(1, sizeof(**token))))
    return JCC_NO_MEMORY;

  (*token)->next = jcc->token;
  jcc->token = *token;
  return 0;
}

int jcc_lex_char(jcc_t *jcc, int ch, jcc_token_t **token) {
  int ch2 = 0;
  int err = 0;
  err = jcc_getchar(jcc, &ch2);
  if (err || ch != ch2 || (err = jcc_new_token(jcc, token)))
    return err;
  (*token)->short_string[0] = (char)ch;
  (*token)->tag = jcc_token_tag_punctuator;
  (*token)->size = 1;
  return 0;
}

int jcc_lex_newline(jcc_t *jcc, jcc_token_t **token_newline) {
  return jcc_lex_char(jcc, '\n', token_newline);
}

void jcc_lex_commit(jcc_t *jcc) {}

int jcc_preprocess_control_line(jcc_t *jcc, size_t *recognized)
/*  # include pp-tokens new-line
 *  # define identifier replacement-list new-line
 *  # define identifier lparen identifier-list_opt ) replacement-list new-line
 *  # define identifier lparen ... ) replacement-list new-line
 *  # define identifier lparen identifier-list , ... ) replacement-list
 * new-line # undef identifier new-line # line pp-tokens new-line # error
 * pp-tokens_opt new-line # pragma pp-tokens_opt new-line # new-line
 */
{
  int err = 0;
  jcc_token_t *token = 0;

  if (jcc->ch != '#')
    goto error_label;

  token = jcc->token;

  while (1) {
    jcc_token_t *identifier = 0;
    jcc_token_t *lparen = 0;
    jcc_token_t *newline = 0;
    jcc_token_t *replacement_list = 0;
    int ch = 0;

    err = jcc_getchar(jcc, &ch);
    if (err)
      goto error_label;
    switch (ch) {
    case ' ':
      break;
    case '\n':
      *recognized += 1;
      return 0;
    case 'd':
      err = jcc_lex_candidate_token(jcc, false, &jcc_token_define, &token,
                                    recognized);
      if (err)
        goto error_label;
      if (token) {
        err = jcc_lex_identifier(jcc, &identifier);
        if (err || !identifier)
          goto error_label;
        err = jcc_lex_char(jcc, '(', &lparen);
        if (err)
          goto error_label;
        err = jcc_getchar(jcc, &ch);
        if (err)
          goto error_label;
        if (ch == '(') {
        } else {
          err = jcc_lex_replacement_list(jcc, &replacement_list);
          if (err || !replacement_list)
            goto error_label;
          err = jcc_lex_newline(jcc, &newline);
          if (err || !newline)
            goto error_label;
        }
        jcc_lex_commit(jcc);
      }
    case 'u':
      err = jcc_lex_candidate_token(jcc, false, &jcc_token_undef, &token,
                                    recognized);
      if (err)
        goto error_label;
    case 'l':
      err = jcc_lex_candidate_token(jcc, false, &jcc_token_line, &token,
                                    recognized);
      if (err)
        goto error_label;
    case 'e':
      err = jcc_lex_candidate_token(jcc, false, &jcc_token_error, &token,
                                    recognized);
      if (err)
        goto error_label;
    case 'i':
      err = jcc_lex_candidate_token(jcc, false, &jcc_token_include, &token,
                                    recognized);
      if (err)
        goto error_label;
    case 'p':
      err = jcc_lex_candidate_token(jcc, false, &jcc_token_pragma, &token,
                                    recognized);
      if (err)
        goto error_label;
    }
  }
error_label:
  jcc->token = token;
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
    /*jcc_preprocess_backtrack_prepare(jcc);*/
    if ((err = call(jcc, &recognized)))
      return err;
    if (recognized) {
      /*jcc_preprocess_backtrack_cancel(jcc);*/
      *precognized += 1;
      return 0;
    }
    /*jcc_preprocess_backtrack(jcc);*/
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
  char directive[16];
  int i = 1;
  int err = 0;

  JMEMSET0_VALUE(directive);
  directive[0] = (char)ch;
  while (i < JCOUNT(directive) && jcc_is_identifier_char(ch)) {
    err = jcc_getchar(jcc, &ch);
    if (err)
      return err;
    directive[i] = (char)ch;
    ++i;
  }
  return 0;
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
  jcc_token_t *ptoken = 0;

  while (1) {
    /* Handle queuing and backtracking. */
    *pptoken = 0;
    ptoken = JBASE(jcc_token_t, pp_queued, jlist_remove_first(&jcc->pp_queued_tokens));
    if (ptoken) {
      *pptoken = ptoken;
      return 0;
    }
    err = jcc_getchar(jcc, &ch);
    if (err)
      return err;
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
        if (err)
          return err;
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

void jcc_init_token_string(jcc_token_t *token, const char *short_string) {
  int size;
  size = (int)strlen(short_string);
  token->size = size;
  memcpy(token->short_string, short_string, size);
}

int jcc_dup_token(jcc_t *jcc, jcc_token_t *token1, jcc_token_t **token2) {
  int err;

  if ((err = jcc_new_token(jcc, token2)))
    return err;
  **token2 = *token1;
  JMEMSET0_VALUE((*token2)->queued);
  JMEMSET0_VALUE((*token2)->pp_queued);
  return err;
}

void jcc_init_token(jcc_token_t *token, const char *str, jcc_token_tag tag) {
  if (str) {
    if (tag == jcc_token_tag_punctuator)
      jcc_char_class[*str] = tag;
    jcc_init_token_string(token, str);
  }
  token->tag = tag;
  jcc_init_trie(str, token);
}

void jcc_init(void) {
  int i;
  for (i = jcc_char_space_first; i <= jcc_char_space_last; ++i) {
    jcc_char_class[i] = jcc_char_space;
    jcc_space[i] = 1;
  }
  for (i = 0; i < 256; ++i)
    jcc_char_class[i] = i;

  for (i = 'A'; i <= 'Z'; ++i) {
    jcc_char_class[jcc_char_to_upper(i)] = jcc_char_alpha;
    jcc_char_class[jcc_char_to_lower(i)] = jcc_char_alpha;
    jcc_char_starts_indefinite_token_fast[jcc_char_to_lower(i)] = 1;
    jcc_char_starts_indefinite_token_fast[jcc_char_to_upper(i)] = 1;
  }

  jcc_char_starts_indefinite_token_fast['.'] = jcc_indefinite_num;
  jcc_char_starts_indefinite_token_fast['_'] = jcc_indefinite_id ;
  jcc_char_starts_indefinite_token_fast['"'] = jcc_indefinite_str;

  for (i = '0'; i <= '9'; ++i) {
    jcc_char_class[i] = jcc_char_num;
  }

  jcc_init_token(&jcc_token_newline, "\n", jcc_token_tag_punctuator);
  jcc_init_token(&jcc_token_define, "define", jcc_token_tag_define);
  jcc_init_token(&jcc_token_error, "error", jcc_token_tag_error);
  jcc_init_token(&jcc_token_include, "include", jcc_token_tag_include);
  jcc_init_token(&jcc_token_line, "line", jcc_token_tag_line);
  jcc_init_token(&jcc_token_undef, "undef", jcc_token_tag_undef);
  jcc_init_token(&jcc_token_pragma, "pragma", jcc_token_tag_pragma);

  jcc_init_token(&jcc_token_left_shift, "<<", jcc_token_tag_punctuator);
  jcc_init_token(&jcc_token_right_shift, ">>", jcc_token_tag_punctuator);
  jcc_init_token(&jcc_token_eq, "==", jcc_token_tag_punctuator);
  jcc_init_token(&jcc_token_neq, "!=", jcc_token_tag_punctuator);
  jcc_init_token(&jcc_token_assign, "=", jcc_token_tag_punctuator);
  jcc_init_token(&jcc_token_less, "<", jcc_token_tag_punctuator);
  jcc_init_token(&jcc_token_greater, ">", jcc_token_tag_punctuator);
  jcc_init_token(&jcc_token_ge, ">", jcc_token_tag_punctuator);
  jcc_init_token(&jcc_token_le, "<=", jcc_token_tag_punctuator);
  jcc_init_token(&jcc_token_lparen, "(", jcc_token_tag_punctuator);
  jcc_init_token(&jcc_token_rparen, ")", jcc_token_tag_punctuator);
  jcc_init_token(&jcc_token_lbracket, "[", jcc_token_tag_punctuator);
  jcc_init_token(&jcc_token_rbracket, "]", jcc_token_tag_punctuator);
  jcc_init_token(&jcc_token_lbrace, "{", jcc_token_tag_punctuator);
  jcc_init_token(&jcc_token_rbrace, "}", jcc_token_tag_punctuator);
  jcc_init_token(&jcc_token_dot, ".", jcc_token_tag_punctuator);
  jcc_init_token(&jcc_token_dot, "...", jcc_token_tag_punctuator);

  jcc_init_token(&jcc_token_plus, "+", jcc_token_tag_punctuator);
  jcc_init_token(&jcc_token_slash, "/", jcc_token_tag_punctuator);
  jcc_init_token(&jcc_token_percent, "%", jcc_token_tag_punctuator);
  jcc_init_token(&jcc_token_star, "*", jcc_token_tag_punctuator);

  jcc_init_token(&jcc_token_and, "&", jcc_token_tag_punctuator);
  jcc_init_token(&jcc_token_tilde, "~", jcc_token_tag_punctuator);
  jcc_init_token(&jcc_token_exclaim, "!", jcc_token_tag_punctuator);
  jcc_init_token(&jcc_token_bar, "|", jcc_token_tag_punctuator);

  jcc_init_token(&jcc_token_pluss, "++", jcc_token_tag_punctuator);
  jcc_init_token(&jcc_token_minuss, "--", jcc_token_tag_punctuator);
  jcc_init_token(&jcc_token_ands, "&&", jcc_token_tag_punctuator);
  jcc_init_token(&jcc_token_bars, "||", jcc_token_tag_punctuator);

  jcc_init_token(&jcc_token_colon, ":", jcc_token_tag_punctuator);
  jcc_init_token(&jcc_token_semi, ";", jcc_token_tag_punctuator);
  jcc_init_token(&jcc_token_pound, "#", jcc_token_tag_punctuator);
  jcc_init_token(&jcc_token_pounds, "##", jcc_token_tag_punctuator);
  jcc_init_token(&jcc_token_question, "?", jcc_token_tag_punctuator);
  jcc_init_token(&jcc_token_comma, ",", jcc_token_tag_punctuator);

  jcc_init_token(&jcc_token_auto, "auto", 0);
  jcc_init_token(&jcc_token_char, "char", 0);
  jcc_init_token(&jcc_token_do, "do", 0);
  jcc_init_token(&jcc_token_for, "for", 0);
  jcc_init_token(&jcc_token_double, "double", 0);
  jcc_init_token(&jcc_token_else, "else", 0);
  jcc_init_token(&jcc_token_float, "float", 0);
  jcc_init_token(&jcc_token_goto, "goto", 0);
  jcc_init_token(&jcc_token_if, "if", 0);
  jcc_init_token(&jcc_token_int, "int", 0);
  jcc_init_token(&jcc_token_long, "long", 0);
  jcc_init_token(&jcc_token_return, "return", 0);
  jcc_init_token(&jcc_token_short, "short", 0);
  jcc_init_token(&jcc_token_signed, "signed", 0);
  jcc_init_token(&jcc_token_struct, "struct", 0);
  jcc_init_token(&jcc_token_static, "static", 0);
  jcc_init_token(&jcc_token_typedef, "typedef", 0);
  jcc_init_token(&jcc_token_union, "union", 0);
  jcc_init_token(&jcc_token_unsigned, "unsigned", 0);
  jcc_init_token(&jcc_token_while, "while", 0);
}

int jcc(int argc, char **argv) {
  argc &&argv;

  jcc_init();
  return 0;
}
