/* C preprocessor. */

#if !JPRE_INTERNAL_H
#define JPRE_INTERNAL_H 1

#include "jvec.h"

typedef struct cpre_t cpre_t;
typedef struct cmacro_t cmacro_t;
typedef struct cpre_unget_t cpre_unget_t;
typedef struct cpre_t cpre_t;
typedef struct cpre_expanding_t cpre_expanding_t;
typedef int (*cpre_directive_handler)(cpre_t *, jvec_char_t *);
typedef struct cpre_directive_t cpre_directive_t;

int jcc_ppendif(cpre_t *cpre, jvec_char_t *body);
int jcc_pperror(cpre_t *cpre, jvec_char_t *body);
int jcc_ppelse(cpre_t *cpre, jvec_char_t *body);
int jcc_ppelif(cpre_t *cpre, jvec_char_t *body);
int jcc_ppif(cpre_t *cpre, jvec_char_t *body);
int jcc_ppinclude(cpre_t *cpre, jvec_char_t *body);
int jcc_ppline(cpre_t *cpre, jvec_char_t *body);
int jcc_pponce(cpre_t *cpre, jvec_char_t *body);
int jcc_pppragma(cpre_t *cpre, jvec_char_t *body);
int jcc_ppundef(cpre_t *cpre, jvec_char_t *body);

int jcc_ppdirective(cpre_t *cpre, int ch);
int cpre_translate_space(int ch);
int cpre_get_token(cpre_t *cpre);
int cpre_get_char(cpre_t *cpre, int *ch);

#endif
