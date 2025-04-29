/* C preprocessor. */

typedef struct cpre_t cpre_t;
typedef struct cmacro_t cmacro_t;
typedef struct cpre_unget_t cpre_unget_t;
typedef struct cpre_t cpre_t;
typedef struct cpre_expanding_t cpre_expanding_t;
typedef int (*cpre_directive_handler)(cpre_t *);
typedef struct cpre_directive_t cpre_directive_t;

int cpre_if(cpre_t *cpre);
int cpre_endif(cpre_t *cpre);
int cpre_else(cpre_t *cpre);
int cpre_elif(cpre_t *cpre);
int cpre_include(cpre_t *cpre);
int cpre_line(cpre_t *cpre);
int cpre_undef(cpre_t *cpre);
int cpre_directive(cpre_t *cpre, int ch);
int cpre_translate_space(int ch);
int cpre_get_token(cpre_t *cpre);
int cpre_get_char(cpre_t *cpre, int *ch);
