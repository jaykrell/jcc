#ifndef JCC_H
#define JCC_H

#include "jbase.h"
#include "jtype.h"
#include "jvec.h"
#include "jmap.h"
#include "jstring.h"

//struct jmap { };
//struct jhash { };

//struct jstr { };

typedef enum ceType
{
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

    ceTypesChar,
    ceTypesInt,
    ceTypesLong,
} ceType;

typedef struct ctype ctype;
struct ctype
{
    ceType e;
};

/* or hfile, really a cprefile */
typedef struct cfile cfile;
struct cfile
{
    jstring path;
    jlong size;
    jlong position;
    cfile* stack;
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
    cpreTokenDirective,
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
    //jstack_cSourceFile include_stack;
    cSourceFile source_file;
    cFrontFunction* function;
    cToken token;
} ccState;

cToken cgetNextToken(cSourceFile* file);

/* cpre */

//copen

/* clex */

/* cparse */

/* ccheck */

/* ccode */

#endif
