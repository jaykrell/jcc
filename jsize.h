#if !JSIZE_H
#define JSIZE_H 1

#if _WIN64 || VMS || defined(__int64)
typedef unsigned __int64 jsize, jsize_t;
#define JSIZE_PRI "I64"
#else
typedef unsigned long jsize, jsize_t;
#define JSIZE_PRI "l"
#endif

#endif
