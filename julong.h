#ifndef JULONG_H
#define JULONG_H 1

/* pointer sized integer, the one true integer */
#if _WIN32 || VMS
typedef unsigned __int64 julong, julong_t;
#define JULONG_PRI "I64"
#else
typedef unsigned long julong, julong_t;
#define JULONG_PRI ""
#endif

#endif
