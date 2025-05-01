/* C preprocessor. */

#if !JPRE_INTERNAL_H
#define JPRE_INTERNAL_H 1

#include "jvec.h"

typedef enum jcc_pptoken_tag {
  jcc_pptoken_tag_header_name        = 1,
  jcc_pptoken_tag_identifier         = 2,
  jcc_pptoken_tag_pp_number          = 3,
  jcc_pptoken_tag_character_constant = 4,
  jcc_pptoken_tag_string_literal     = 5,
  jcc_pptoken_tag_punctuator         = 6,
  jcc_pptoken_tag_other              = 7 /* each non-white-space character that cannot be one of the above */
} jcc_pptoken_tag;

typedef struct jcc_pptoken_t {
  jcc_pptoken_tag tag;
  jvec_char_t  string;
  /* TODO: What representations are needed? */
} jcc_pptoken_t;

typedef struct cpre_t cpre_t;
typedef struct cmacro_t cmacro_t;
typedef struct cpre_unget_t cpre_unget_t;
typedef struct cpre_t cpre_t;
typedef struct cpre_expanding_t cpre_expanding_t;
typedef int (*cpre_directive_handler)(cpre_t *, jvec_char_t *);
typedef struct cpre_directive_t cpre_directive_t;

int cpre_endif(cpre_t *cpre, jvec_char_t *body);
int cpre_error(cpre_t *cpre, jvec_char_t *body);
int cpre_else(cpre_t *cpre, jvec_char_t *body);
int cpre_elif(cpre_t *cpre, jvec_char_t *body);
int cpre_if(cpre_t *cpre, jvec_char_t *body);
int cpre_include(cpre_t *cpre, jvec_char_t *body);
int cpre_line(cpre_t *cpre, jvec_char_t *body);
int cpre_once(cpre_t *cpre, jvec_char_t *body);
int cpre_pragma(cpre_t *cpre, jvec_char_t *body);
int cpre_undef(cpre_t *cpre, jvec_char_t *body);

int cpre_directive(cpre_t *cpre, int ch);
int cpre_translate_space(int ch);
int cpre_get_token(cpre_t *cpre);
int cpre_get_char(cpre_t *cpre, int *ch);

#endif
