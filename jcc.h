#if !JCC_H
#define JCC_H 1

/* #include "jbool.h" */
/* #include "jpaste.h" */
#include "jfile.h"
#include "jmap.h"
#include "jstr.h"
#include "jtype.h"
#include "jvec.h"
#include <stdint.h>

#define JCC_CHAR_ERROR (-1)
#define JCC_CHAR_END_OF_FILE (-2)

/* Some phases require lookahead of a single character.
 * This is like stdio unget.
 * TODO: expand to arbitrrary size? jvec?
 * TODO: Rename to lookahead?
 * TODO: Expand to contain tokens?
 */
typedef struct jcc_unget_t {
  int value;
  int valid;
} jcc_unget_t;

typedef enum jcc_pptoken_tag {
  jcc_pptoken_tag_header_name        = 1,
  jcc_pptoken_tag_identifier         = 2,
  jcc_pptoken_tag_pp_number          = 3,
  jcc_pptoken_tag_character_constant = 4,
  jcc_pptoken_tag_string_literal     = 5,
  jcc_pptoken_tag_punctuator         = 6,
  jcc_pptoken_tag_other              = 7 /* each non-white-space character that cannot be one of the above */
} jcc_pptoken_tag;

extern jvec_char_t jcc_char1_pptokens[256];
extern jvec_char_t jcc_char2_pptokens[256][256];
extern jvec_char_t jcc_char3_pptokens[256][256][256];

typedef struct jcc_pptoken_t {
  jcc_pptoken_tag tag;
  jvec_char_t  *pstring;
  jvec_char_t  string;
  /* TODO: What representations are needed? */
} jcc_pptoken_t;

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

struct jcc_t;
typedef struct jcc_t jcc_t;

struct jcc_t {
  jcc_unget_t phase1_unget;
  jcc_unget_t phase2_unget;
  jcc_unget_t phase3_unget;
  cfile_t* cfile;
};

#endif
