#include "jcc_token.h"

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

void jcc_initialize_tokens(void) {
  jcc_token_pound.size = 1;
  jcc_token_pound.short_string[0] = '#';
  jcc_token_pound.tag = jcc_token_tag_punctuator;

  jcc_token_newline.size = 1;
  jcc_token_newline.short_string[0] = '\n';
  jcc_token_newline.tag = jcc_token_tag_punctuator;

  jcc_token_error.size = 5;
  jcc_token_newline.short_string[0] = 'e';
  jcc_token_newline.short_string[1] = 'r';
  jcc_token_newline.short_string[2] = 'r';
  jcc_token_newline.short_string[3] = 'o';
  jcc_token_newline.short_string[4] = 'r';
  jcc_token_newline.tag = jcc_token_tag_error;

  jcc_token_error.size = 4;
  jcc_token_newline.short_string[0] = 'l';
  jcc_token_newline.short_string[1] = 'i';
  jcc_token_newline.short_string[2] = 'n';
  jcc_token_newline.short_string[3] = 'e';
  jcc_token_newline.tag = jcc_token_tag_line;
}
