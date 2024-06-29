#ifndef JBASE_H
#define JBASE_H

/* pointer sized integer, the one true integer */
#if _WIN32 || VMS
typedef __int64 jlong;
#else
typedef long jlong;
#endif

typedef void* voidp;
typedef char* charp;

typedef char jbool;

#define JPASTEx(x, y) x ## y
#define JPASTE(x, y) JPASTEx(x, y)

#endif
