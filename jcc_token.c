#include "jcc_token.h"
#include <string.h>

/*
 [ ] ( ) { } . ->
 ++ -- & * + - ~ !
 / % << >> < > <= >= == != ^ | && ||
 ? : ; ...
 = *= /= %= += -= <<= >>= &= ^= |=
 , # ##
each non-white-space character that cannot be one of the above
*/
#if 0
extern jvec_char_t *jcc_char1_pptokens[256];

jcc_pptoken_tag tag;
jvec_char_t string;

[]() {}.->++--&*+-~!/ % << >> <> <= >= == != ^| &&|| ?: ;
... = *= /= %= += -= <<= >>= &= ^= |=,
# ##<::><%%> %: %:%:

    jcc_pptoken_tag_header_name = 1, jcc_pptoken_tag_identifier = 2,
    jcc_pptoken_tag_pp_number = 3, jcc_pptoken_tag_character_constant = 4,
    jcc_pptoken_tag_string_literal = 5, jcc_pptoken_tag_punctuator = 6,
    jcc_pptoken_tag_other =
        7 /* each non-white-space character that cannot be one of the above */

    int
    jcc_get_pptoken(jcc_t * jcc, int ch, jcc_pptoken_t **pptoken) {
  if (ch < 0)
    return 0;
  switch (ch) {
  case '[': {
    static jcc_pptoken_t pptoken_lbr = {jcc_pptoken_tag_punctuator, "[", 1, 1,
                                        1};
    return &pptoken_lbr;
  }
  }
}
}

#endif

jcc_token_t jcc_token_pound;
jcc_token_t jcc_token_newline;
jcc_token_t jcc_token_define;
jcc_token_t jcc_token_error;
jcc_token_t jcc_token_line;
jcc_token_t jcc_token_include;
jcc_token_t jcc_token_pragma;
jcc_token_t jcc_token_undef;

void jcc_initialize_token_string(jcc_token_t *token, const char *short_string) {
  int size;
  size = (int)strlen(short_string);
  token->size = size;
  memcpy(token->short_string, short_string, size);
}

void jcc_initialize_tokens(void) {
  jcc_initialize_token_string(&jcc_token_pound, "#");
  jcc_token_pound.tag = jcc_token_tag_punctuator;

  jcc_initialize_token_string(&jcc_token_newline, "\n");
  jcc_token_newline.tag = jcc_token_tag_punctuator;

  jcc_initialize_token_string(&jcc_token_define, "define");
  jcc_token_define.tag = jcc_token_tag_define;

  jcc_initialize_token_string(&jcc_token_error, "error");
  jcc_token_error.tag = jcc_token_tag_error;

  jcc_initialize_token_string(&jcc_token_include, "include");
  jcc_token_include.tag = jcc_token_tag_include;

  jcc_initialize_token_string(&jcc_token_line, "line");
  jcc_token_line.tag = jcc_token_tag_line;

  jcc_initialize_token_string(&jcc_token_undef, "undef");
  jcc_token_undef.tag = jcc_token_tag_undef;

  jcc_initialize_token_string(&jcc_token_pragma, "pragma");
  jcc_token_pragma.tag = jcc_token_tag_pragma;
}
