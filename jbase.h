#ifndef JBASE_H
#define JBASE_H

/* pointer sized integer, the one true integer */
#if _WIN32 || VMS
typedef __int64 jlong;
#define JLONG_PRI "I64"
#else
typedef long jlong;
#define JLONG_PRI ""
#endif

typedef void* voidp;
typedef char* charp;

typedef char jbool;

#define JPASTEx(x, y) x ## y
#define JPASTE(x, y) JPASTEx(x, y)

#endif
