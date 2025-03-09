#ifndef JBASE_H
#define JBASE_H

/* pointer sized integer, maybe the one true integer */
#if _WIN32 || VMS
typedef __int64 Long;
typedef __int64 jlong, jssize, jlong_t, jssize_t;
#else
typedef long Long;
typedef long jlong, jssize, jlong_t, jssize_t;
#endif

typedef void* voidp; /* void pointer, not portable to pre-ANSI C */
typedef char* charp; /* character pointer */

typedef char jbool;

#define JPASTEx(x, y) x ## y
#define JPASTE(x, y) JPASTEx(x, y)

#endif
