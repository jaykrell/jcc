#ifndef JCC_H
#define JCC_H

#include "jbase.h"
#include "jtype.h"
#include "jvec.h"

struct jmap
{
};

struct jhash
{
};

struct jstr
{
};

struct ctype
{
};

/* or hfile, really a cprefile */
struct cfile {
};

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
    cpreDirectiveEmpty,
} cpreDirective;

typedef enum cpreToken {
    cpreTokenInvalid,
} cpreToken;

typedef enum ceToken {
    cetokenInvalid,
    cetokenPlus,
    cetokenStar,
    cetokenMinus,
    cetokenDiv,
    cetokenMod,
    cetokenAnd,
    cetokenOr,
    cetokenEq,
    cetokenPlusEq,
    cetokenStarEq,
    cetokenMinusEq,
    cetokenDivEq,
    cetokenModEq,
    cetokenAndEq,
    cetokenOrEq,
    cetokenEqEq,
    cetokenAnd2,
    cetokenOr2,
    cetokenEq2,
} ceToken;

typedef struct cSourceFile {
    char* file;
    long  line;
    char* contents;
    long position;
} cSourceFile;

typedef struct cSourceLocation {
    char* file;
    long  line;
    long  column;
} cSourceLocation;

typedef struct cSourceSpan {
    cSourceLocation start;
    cSourceLocation end;
} cSourceSpan;

typedef struct cToken {
    cSourceSpan src;
    char* rep;
    ceToken e;
} cToken;

typedef struct cFrontFunction {
    cSourceSpan span;
} cFrontFunction;

typedef struct ccState {
    jstack_cSourceFile include_stack;
    cSourceFile source_file;
    cFrontFunction* function;
    cToken token;
} ccState;

cgetNextToken(cSourceFile* file);

typedef struct cToken {
    char* pretty;
} cToken;

/* cpre */

//copen

/* clex */

/* cparse */

/* ccheck */

/* ccode */

#endif
