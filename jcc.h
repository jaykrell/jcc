#if !JCC_H
#define JCC_H 1

/* #include "jbool.h" */
/* #include "jpaste.h" */
/*#include "jcc_preprocess_token.h"*/
#include "jcc_unget.h"
#include "jfile.h"
#include "jmap.h"
#include "jstr.h"
#include "jtype.h"
#include "jvec.h"
#include <stdint.h>

#define JCC_CHAR_ERROR (-1)
#define JCC_CHAR_END_OF_FILE (-2)
#define JCC_UNRECOGNIZED (-3)
#define JCC_NO_MEMORY (-4)

#include "jlist.h"
#include "jvec.h"

struct jcc_t;
typedef struct jcc_t jcc_t;

struct jcc_token_t;
typedef struct jcc_token_t jcc_token_t;

/* body of macro */
struct jcc_replacement_list_t;
typedef struct jcc_replacement_list_t jcc_replacement_list_t;

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
  jcc_token_tag_undef = 206,
} jcc_token_tag;

extern jcc_token_t jcc_token_pound;
extern jcc_token_t jcc_token_newline;
extern jcc_token_t jcc_token_define;
extern jcc_token_t jcc_token_error;
extern jcc_token_t jcc_token_line;
extern jcc_token_t jcc_token_include;
extern jcc_token_t jcc_token_pragma;
extern jcc_token_t jcc_token_undef;

struct jcc_token_t {
  /* for example, replacement list in macros */
  jcc_token_t *next;
  jcc_token_tag tag;
  char short_string[16];
  jvec_char_t string;
  size_t size;
  /*  jlist_t free; */
  jlist_t list; /* queue_tokens within jcc */
};

/* Body of a macro. */
struct jcc_replacement_list_t {
  jcc_token_t *first;
  size_t size;
};

int jcc_new_token(jcc_t *jcc, jcc_token_t **token);

extern int jcc_ch;

/* phase1 is newline handling. */
int jcc_phase1_getchar(jcc_t *jcc, int *ch);
void jcc_phase1_unget(jcc_t *jcc, int ch);

/* phase2 is backslash line continuation. */
void jcc_phase2_unget(jcc_t *jcc, int ch);
int jcc_phase2_getchar(jcc_t *jcc, int *ch);

/* phase3 handles comments, replacing them with space. */
/* int jcc_phase3_getchar(jcc_t *jcc, int *ch); */
/* This is the intuitive last phase that gets characters before higher level
 * constructs so the phase notion can be hidden. */
int jcc_getchar(jcc_t *jcc, int *ch);

/* Hide the phases. */
int jcc_getchar(jcc_t *jcc, int *ch);

int jcc_new_token(jcc_t *jcc, jcc_token_t **token);

void jcc_preprocess_backtrack_prepare(jcc_t *jcc);
void jcc_preprocess_backtrack_cancel(jcc_t *jcc);
void jcc_preprocess_backtrack(jcc_t *jcc);

/*
typedef struct cpre_t cpre_t;
typedef struct cmacro_t cmacro_t;
typedef struct cpre_unget_t cpre_unget_t;
typedef struct cpre_t cpre_t;
typedef struct cpre_expanding_t cpre_expanding_t;
*/
typedef int (*jcc_preprocess_directive_handler_t)(jcc_t *, jvec_char_t *);
typedef struct jcc_preprocess_directive_t jcc_preprocess_directive_t;

int jcc_preprocess_endif(jcc_t *jcc, jvec_char_t *body);
int jcc_preprocess_error(jcc_t *jcc, jvec_char_t *body);
int jcc_preprocess_else(jcc_t *jcc, jvec_char_t *body);
int jcc_preprocess_elif(jcc_t *jcc, jvec_char_t *body);
int jcc_preprocess_if(jcc_t *jcc, jvec_char_t *body);
int jcc_preprocess_include(jcc_t *jcc, jvec_char_t *body);
int jcc_preprocess_line(jcc_t *jcc, jvec_char_t *body);
int jcc_preprocess_once(jcc_t *jcc, jvec_char_t *body);
int jcc_preprocess_pragma(jcc_t *jcc, jvec_char_t *body);
int jcc_preprocess_undef(jcc_t *jcc, jvec_char_t *body);

int jcc_ppdirective(jcc_t *jcc, int ch);
/*
int cpre_translate_space(int ch);
int cpre_get_token(jcc_t *jcc);
int cpre_get_char(jcc_t *jcc, int *ch);
*/

/* C compiler type enum */
/* language and backend types must be considered different, i.e. int32 vs. int
 * vs. long */
/* 'e' for enum */
typedef enum ceType {
  ceInvalid,

  ceTypeInt8,
  ceTypeInt16,
  ceTypeInt32,
  ceTypeInt64,
  ceTypeUInt8,
  ceTypeUInt16,
  ceTypeUInt32,
  ceTypeUInt64,
  ceTypeFloat32,
  ceTypeFloat64,
  ceTypePointer,

  ceIntegral,
  ceFloat,
  cePointer,
  ceAggregate,
  ceStruct,
  ceUnion,

  ceTypeChar,
  ceTypeShort,
  ceTypeInt,
  ceTypeLong,
  ceTypeLongLong,

  ceTypeSChar,
  ceTypeUChar,
  ceTypeUShort,
  ceTypeUInt,
  ceTypeULong,
  ceTypeULongLong

} ceType;

typedef struct cmember cmember;

typedef struct ctype ctype;
struct ctype {
  ceType e;
  int bytes;
  int bits;
  int byte_align;
  int bit_align;
  int member_count; /* for struct or union */
  cmember *members; /* for struct or union */
  jbool integer;
  jbool unsign; /* unsigned */
  jbool sign;   /* signed */
  jbool floating_point;
  jbool aggregate;
  jbool bitfield;
  jbool def; /* typedef */
  jbool user;
  ctype *link; /* e.g. for typedef */
};

#if 0
typedef struct cmember cmember;
struct cmember {
    ctype* type;
    jvec*  name;
    int    bit_offset;
};
#endif

/* or hfile, really a cprefile */
typedef struct cfile_t cfile_t;
struct cfile_t {
  jvec_char_t path;
  int64_t size;
  int64_t position;
  cfile_t *stack;
  jfile_t *file;
  jlist_t /*jcc_token_t */ tokens_free;
};

/* C compiler preprocessor directive */
typedef enum cpreDirective {
  cpreDirectiveInvalid,
  cpreDirectiveDefine,
  cpreDirectiveInclude,
  cpreDirectiveEndif,
  cpreDirectiveIf,
  cpreDirectiveElse,
  cpreDirectiveElif,
  cpreDirectiveIfdef,
  cpreDirectiveIfndef,
  cpreDirectiveEmpty
} cpreDirective;

/* C compiler preprocessor token enum */
typedef enum cpreToken { cpreTokenInvalid, cpreTokenDirective } cpreToken;

/* C compiler token enum */
typedef enum ceToken {
  ceTokenInvalid,

  ceTokenCaret = '^',
  ceTokenDot = '.',
  ceTokenComma = ',',
  ceTokenLeftParen = '(',
  ceTokenRigtParen = ')',
  ceTokenSemi = ';',
  ceTokenColon = ':',
  ceTokenLeftBracket = '[',
  ceTokenRightBracket = ']',
  ceTokenLeftBrace = '{',
  ceTokenRightBrace = '}',
  ceTokenPlus = '+',
  ceTokenStar = '*',
  ceTokenMinus = '-',
  ceTokenDiv = '/',
  ceTokenMod = '%',
  ceTokenAnd = '&',
  ceTokenOr = '|',
  ceTokenEq = '=',
  ceTokenBang = '!',
  ceTokenQuestioMark = '?',
  ceTokenLessThan = '<',
  ceTokenGreaterThan = '>',

  ceTokenPlusEq = 128,
  ceTokenStarEq,
  ceTokenMinusEq,
  ceTokenDivEq,
  ceTokenModEq,
  ceTokenAndEq,
  ceTokenOrEq,
  ceTokenEqEq,
  ceTokenAnd2,             /* && */
  ceTokenOr2,              /* || */
  ceTokenEq2,              /* == */
  ceTokenLeftShift,        /* << */
  ceTokenRightShift,       /* >> */
  ceTokenLeftShiftAssign,  /* <<= */
  ceTokenRightShiftAssign, /* >>= */

  ceTokenContinue,
  ceTokenDo,
  ceTokenElse,
  ceTokenFor,
  ceTokenIf,
  ceTokenWhile,

  ceTokenCharacterConstant,
  ceTokenStringConstant,

  /* integer types */
  ceTokenInt8,
  ceTokenInt16,
  ceTokenInt32,
  ceTokenInt64,
  ceTokenChar,
  ceTokenShort,
  ceTokenInt,
  ceTokenLong,
  ceTokenUnsigned,

  /* floating point types */
  ceTokenFloat,
  ceTokenDouble,
  ceTokenLongDouble, /* translated to LongDouble, really */

  /* fancy types */
  ceTokenEnum,
  ceTokenStruct,
  ceTokenUnion,
  ceTokenTypedef
} ceToken;

struct cMember;
typedef struct cMember cMember;
struct cMember {
  ctype *type;
  char *name;
  int64_t bit_offset;
  int64_t byte_offset;
  cMember *next;
};

struct cAggregate;
typedef struct cAggregate cAggregate;

struct cAggregate {
  jbool is_union;
  jbool is_struct;
  jbool pad[6];
  int64_t n;
  cMember *members;
};

typedef struct cSourceFile {
  char *file;
  long line;
  char *contents;
  long position; /* index into position */
} cSourceFile;

typedef struct cSourceLocation {
  char *file;
  long line;
  long column;
} cSourceLocation;

typedef struct cSourceSpan {
  cSourceLocation start;
  cSourceLocation end;
} cSourceSpan;

typedef struct cToken {
  cSourceSpan src;
  char *rep;
  ceToken e;
} cToken;

typedef struct cFrontFunction {
  cSourceSpan span;
} cFrontFunction;

typedef struct ccState {
  /*jstack_cSourceFile include_stack;*/
  cSourceFile source_file;
  cFrontFunction *function;
  cToken token;
} ccState;

cToken cgetNextToken(cSourceFile *file);

/* cpre */

/* copen */

/* clex */

/* cparse */

/* ccheck */

/* ccode */

struct jcc_t {
  jcc_unget_t phase1_unget;
  jcc_unget_t phase2_unget;
  jcc_unget_t phase3_unget;
  jlist_t /*jcc_token_t*/ commited_tokens;
  jlist_t /*jcc_token_t*/ queued_tokens;
  cfile_t *cfile;
  jcc_token_t *token;
  int ch;
};

#endif
