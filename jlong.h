#ifndef JLONG_H
#define JLONG_H

/* pointer sized integer, the one true integer */
#if _WIN32 || VMS
typedef __int64 jlong;
#define JLONG_PRI "I64"
#else
typedef long jlong;
#define JLONG_PRI ""
#endif

#endif
