#ifndef JSSIZE_H
#define JSSIZE_H 1

/* pointer sized integer, the one true integer */
#if _WIN32 || VMS || defined(__int64)
typedef __int64 jssize, jssize_t;
#else
typedef long jssize, jssize_t;
#endif

#endif
