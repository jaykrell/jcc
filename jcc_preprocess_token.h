#if !JCC_PREPROCESS_TOKEN_H
#define JCC_PREPROCESS_TOKEN_H 1

#include "jlist.h"
#include "jvec.h"

typedef enum jcc_preprocess_token_tag {
  jcc_preprocess_token_tag_header_name = 1,
  jcc_preprocess_token_tag_identifier = 2,
  jcc_preprocess_token_tag_pp_number = 3,
  jcc_preprocess_token_tag_character_constant = 4,
  jcc_preprocess_token_tag_string_literal = 5,
  jcc_preprocess_token_tag_punctuator = 6,
  jcc_preprocess_token_tag_other =
      7 /* each non-white-space character that cannot be one of the above */
} jcc_preprocess_token_tag;

typedef struct jcc_preprocess_token_t {
  jcc_preprocess_token_tag tag;
  char short_string[4];
  jvec_char_t string;
  jlist_t free;
  jlist_t list;
} jcc_preprocess_token_t;

#endif
