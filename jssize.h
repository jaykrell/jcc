#ifndef JSSIZE_H
#define JSSIZE_H 1

/* pointer sized integer, the one true integer */
#if _WIN64 || VMS || defined(__int64)
typedef __int64 jssize, jssize_t;
#define JSSIZE_PRI "I64"
#else
typedef long jssize, jssize_t;
#define JSSIZE_PRI "l"
#endif

#endif
