#ifndef JBASE_H
#define JBASE_H

#include "jbool.h"
#include "jlong.h"

typedef void* voidp; /* void pointer, not portable to pre-ANSI C */
typedef char* charp; /* character pointer */

#define JPASTEx(x, y) x ## y
#define JPASTE(x, y) JPASTEx(x, y)

#endif
