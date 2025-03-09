#ifndef JLONG_H
#define JLONG_H 1

/* pointer sized integer, the one true integer */
#if _WIN32 || VMS
typedef __int64 jlong, jlong_t;
#define JLONG_PRI "I64"
#else
typedef long jlong, jlong_t;
#define JLONG_PRI ""
#endif

#endif
