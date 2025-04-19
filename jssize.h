#if !JSSIZE_H
#define JSSIZE_H 1

#include <stddef.h>

typedef ptrdiff_t ssize_t;

#if _WIN64 || VMS || defined(__int64)
typedef unsigned __int64 jssize, jssize_t;
#define JSSIZE_PRI "I64"
#else
typedef long jssize, jssize_t;
#define JSSIZE_PRI "l"
#endif

#endif
