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

char jcc_token;