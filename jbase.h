#ifndef JBASE_H
#define JBASE_H

#include "jbool.h"
#include "jssize.h"

typedef void* voidp;
typedef char* charp;

#define JPASTEx(x, y) x ## y
#define JPASTE(x, y) JPASTEx(x, y)

#endif
