#if !JCC_PREPROCESS_TOKEN_H
#define JCC_PREPROCESS_TOKEN_H 1

#include "jlist.h"
#include "jvec.h"

int jcc_is_xid_start(int ch);
int jcc_is_universal_xid_start(int ch);

int jcc_is_xid_continue(int ch);
int jcc_is_universal_xid_continue(int ch);

int jcc_is_keyword(jvec_char_t *);

typedef enum jcc_token_tag {
  jcc_token_tag_keyword = 1,
  jcc_token_tag_identifier = 2,
  jcc_token_tag_string_literal = 3,
  jcc_token_tag_punctuator = 4,
  jcc_token_tag_constant = 5,
  /*-----------------------------------------*/
  jcc_token_tag_integer_constant = 6,
  jcc_token_tag_floating_constant = 7,
  jcc_token_tag_enumeration_constant = 8,
  jcc_token_tag_character_constant = 9,
  jcc_token_tag_predefined_constant = 10,
  /*-----------------------------------------*/
  jcc_preprocess_token_tag_header_name = 100,
  jcc_preprocess_token_tag_identifier = 101,
  jcc_preprocess_token_tag_pp_number = 102,
  jcc_preprocess_token_tag_character_constant = 103,
  jcc_preprocess_token_tag_string_literal = 104,
  jcc_preprocess_token_tag_punctuator = 105,
  jcc_preprocess_token_tag_universal_char = 106,
  jcc_preprocess_token_tag_nonwhite_char = 107,
  /*-----------------------------------------*/
  jcc_token_tag_define = 201,
  jcc_token_tag_error = 202,
  jcc_token_tag_include = 203,
  jcc_token_tag_line = 204,
  jcc_token_tag_pragma = 205,
} jcc_token_tag;

struct jcc_token_t;
typedef struct jcc_token_t jcc_token_t;

struct jcc_token_t {
  jcc_token_t *next;
  jcc_token_tag tag;
  char short_string[16];
  jvec_char_t string;
  size_t size;
  /*  jlist_t free;
    jlist_t list;*/
};

struct jcc_replacement_list_t;
typedef struct jcc_replacement_list_t jcc_replacement_list_t;

/* Body of a macro. */
struct jcc_replacement_list_t {
  jcc_token_t *first;
  size_t size;
};

#endif /* once */
