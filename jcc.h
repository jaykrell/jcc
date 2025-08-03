#if !JCC_H
#define JCC_H 1

#include "jbool.h"
#include "jcc_unget.h"
#include "jdec.h"
#include "jfile.h"
#include "jlist.h"
#include "jmap.h"
#include "jstr.h"
#include "jtype.h"
#include "jvec.h"
#include <stdint.h>

#define JCC_CHAR_ERROR (-1)
#define JCC_CHAR_END_OF_FILE (-2)
#define JCC_UNRECOGNIZED (-3)
#define JCC_NO_MEMORY (-4)

struct jcc_t;
typedef struct jcc_t jcc_t;

struct jcc_token_t;
typedef struct jcc_token_t jcc_token_t;

/* body of macro */
struct jcc_replacement_list_t;
typedef struct jcc_replacement_list_t jcc_replacement_list_t;

/* Like ctype.h */
int jcc_char_is_space(int ch);
int jcc_char_is_horizontal_space(int ch);
int jcc_char_is_vertical_space(int ch);
int jcc_char_can_be_in_identifier(int ch);
int jcc_char_can_start_identifier(int ch);
int jcc_char_can_start_preprocessor_directive(int ch);
int jcc_char_can_be_in_preprocessor_directive(int ch);
int jcc_char_to_lower(int ch);
int jcc_char_to_upper(int ch);
int jcc_char_is_alpha(int ch);
int jcc_char_is_lower(int ch);
int jcc_char_is_upper(int ch);
int jcc_char_is_num(int ch);
int jcc_char_starts_indefinitely_long_token(int ch);

int jcc_is_xid_start(int ch);
int jcc_is_universal_xid_start(int ch);

int jcc_is_xid_continue(int ch);
int jcc_is_universal_xid_continue(int ch);

/* Given a vector of characters, is it a keyword such as "for", "if", "else",
 * etc.? */
int jcc_is_keyword(jvec_char_t *);

/* Many characters are their own class, map to themselves.
   Therefore character classes 0-0x7F roughly are already taoken.
   Therefore non-self characters classes are >0x7F.
   Or >0xFF for 'good measure".
*/
typedef enum jcc_char_class_t {
  jcc_char_alpha = 0x101,
  jcc_char_num = 0x102,
  jcc_char_space = 0x103,
  jcc_char_punc = 0x104,
} jcc_char_class_t;

extern jcc_char_class_t jcc_char_class[256];
/*extern jbool jcc_space[256];*/

typedef enum jcc_token_tag {
  jcc_token_tag_invalid = 0,
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

struct jcc_token_t {
  /* for example, replacement list in macros */
  jcc_token_t *next;
  jcc_token_tag tag;
  char short_string[16];
  jvec_char_t string;
  size_t size;
  /*  jlist_t free; */
  jlist_t queued;    /* queue_tokens within jcc */
  jlist_t pp_queued; /* pp_queue_tokens within jcc */
  jcc_token_t *original;
};

/* Body of a macro. */
struct jcc_replacement_list_t {
  jcc_token_t *first;
  size_t size;
};

int jcc_new_token(jcc_t *jcc, jcc_token_t **token);

/*extern int jcc_ch;*/

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
  jdec_char_t queued_chars; /* phase3_unget */
  jlist_t /*jcc_token_t*/ commited_tokens;
  jlist_t /*jcc_token_t*/ queued_tokens;
  jlist_t /*jcc_token_t*/ pp_queued_tokens;
  cfile_t *cfile;
  jcc_token_t *token;
  int ch;
};

void jcc_initialize_token_string(jcc_token_t *token, const char *short_string);
int jcc_dup_token(jcc_t *jcc, jcc_token_t *token1, jcc_token_t **token2);
void jcc_initialize_token(jcc_token_t *token, const char *str,
                          jcc_token_tag tag);
void jcc_initialize_tokens(void);

extern jcc_token_t
    /* This line is to aid clang-format. */
    jcc_token_and,      /* & */
    jcc_token_ands,     /* && */
    jcc_token_assign,   /* = */
    jcc_token_bar,      /* | */
    jcc_token_bars,     /* || */
    jcc_token_colon,    /* : */
    jcc_token_comma,    /* , */
    jcc_token_define,   /* #define */
    jcc_token_dot,      /* . */
    jcc_token_dots,     /* ... */
    jcc_token_else,     /* else */
    jcc_token_eq,       /* == */
    jcc_token_error,    /* #error */
    jcc_token_exclaim,  /* ! */
    jcc_token_ge,       /* >= */
    jcc_token_greater,  /* > */
    jcc_token_if,       /* if */
    jcc_token_int,      /* integer at least 16bits, usually 32bits */
    jcc_token_include,  /* #include */
    jcc_token_lbrace,   /* to start scopes, structs, ints, initializers etc. */
    jcc_token_lbracket, /* [ used with arrays */
    jcc_token_le,       /* <= less or equal */
    jcc_token_left_shift, /* << multiply by a power of 2 */
    jcc_token_less,       /* single character */
    jcc_token_line,       /* as in #line directive */
    jcc_token_long,       /* integer at least 32bits, sometimes 64bits */
    jcc_token_lparen,     /* single character such as in grouping expressions or
                             function calls */
    jcc_token_minus, /* single character binary subtraction or unary negation */
    jcc_token_minuss,  /* --  decrement and return old or new value */
    jcc_token_newline, /* \r, \n, \r\n single character after phase 1 */
    jcc_token_percent, /* single character modulo */
    jcc_token_plus,    /* single character addition */
    jcc_token_pluss,   /* ++ */
    jcc_token_pound,  /* # introduces e.g. #include #define and token pasting */
    jcc_token_pounds, /* ## preprocessor token pasting */
    jcc_token_pragma, /* implementation defined preprocessor
                                            directive, e.g. #pragma once */
    jcc_token_question,    /* single character question mark */
    jcc_token_rbrace,      /* single character question { */
    jcc_token_rbracket,    /* single character question [ */
    jcc_token_return,      /* function return */
    jcc_token_right_shift, /* >> */
    jcc_token_rparen,      /* single character question ) */
    jcc_token_semi,        /* single character question ; */
    jcc_token_short,       /* i.e. int16_t */
    jcc_token_signed,      /* integer that can be negative */
    jcc_token_slash,       /* single character, division */
    jcc_token_star,        /* single character, multiplication or
                                                 pointer dereference */
    jcc_token_tilde,       /* single character, bit inversion */
    jcc_token_undef,       /* preprocessor undefine */
    jcc_token_unsigned,    /* integer that cannot be negative */
    jcc_token_identifier,  /* */
    jcc_token_string_constant;
/*extern jcc_token_t jcc_token_character_constant[256];*/

typedef struct jcc_lex_trie_t jcc_lex_trie_t;
struct jcc_lex_trie_t {
  jcc_lex_trie_t *map[256];
  jcc_token_t *token;
};
extern jcc_lex_trie_t jcc_lex_trie;
/*extern jcc_lex_trie_t jcc_lex_trie0[256];*/

/* char starts indefinitely long token
 * This is: string, identifier, number.
 * This is not the many single or double or triple character
 * tokens, like plus minus assign, shift, etc.
 * The 1-3 character tokens are interesting in that
 * substrings of longer tokens can be valid tokens themselves.
 * For example: <, <<, <<=.
 * For eample:  =, ==
 * For eample:  +, +=
 * For eample:  -, --, ->
 * Dot can start an effectively indefinite floating point constant, or be an
 * entire token. a-z can start keywords or identifiers. Numbers are effectively
 * indefinite (any number of leading 0s are OK, right?)
 */
typedef enum jcc_char_starts_indefinitely_long_token_t {
  jcc_char_starts_indefinitely_long_token_false = 0,
  jcc_char_starts_indefinitely_long_token_str = 1,
  jcc_char_starts_indefinitely_long_token_id = 2,
  jcc_char_starts_indefinitely_long_token_num = 3,
} jcc_char_starts_indefinitely_long_token_t;

extern jcc_char_starts_indefinitely_long_token_t
    jcc_char_starts_indefinitely_long_token_lookup[256];

typedef struct jcc_char_traits_t {
#if 0
  unsigned starts_indefinitely_long_token : 2;
  unsigned is_lower : 1;
  unsigned is_upper : 1;
  unsigned is_num : 1;
  unsigned is_space : 1;
  char to_lower;
  char to_upper;
#else
  int starts_indefinitely_long_token;
  int is_lower;
  int is_upper;
  int is_num;
  int is_space;
  int to_lower;
  int to_upper;
#endif
} jcc_char_traits_t;

extern jcc_char_traits_t jcc_char_traits[256];

#endif
