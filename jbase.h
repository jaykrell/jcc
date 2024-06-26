#ifndef JBASE_H
#define JBASE_H

/* pointer sized integer, maybe the one true integer */
#if _WIN32 || VMS
typedef __int64 Long;
#else
typedef long Long;
#endif

typedef void* voidp;
typedef char* charp;

typedef char jbool;

#define JPASTEx(x, y) x ## y
#define JPASTE(x, y) JPASTEx(x, y)

#endif
