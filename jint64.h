#ifndef JINT64_H
#define JINT64_H 1

#include <limits.h>
/*#include <inttypes.h>*/

/* Unix 64bit systems have 64bit long and do not require C99. */
#if ULONG_MAX > 0x0FFFFFFFFUL
typedef long jint64, jint64_t;
typedef unsigned long juint64, juint64_t;
#define JINT64_PRI "l"

/* Clear indication from gcc predefines that long long works. */
#elif __LONG_LONG_WIDTH__ == 64
typedef long long jint64, jint64_t;
typedef unsigned long long juint64, juint64_t;
#define JINT64_PRI "ll"

/* Support pre-C99 for Windows and VMS and any system with an __int64 macro.
 * Note that define __int64 does not mean "I64" works.
 */
#elif !defined(_LONGLONG) && (defined(_MSC_VER) || defined(__DECC) || defined(__DECCXX) || defined(__int64))
typedef __int64 jint64, jint64_t;
typedef unsigned __int64 juint64, juint64_t;
#define JINT64_PRI "I64"

#else
typedef long long jint64, jint64_t;
typedef unsigned long long juint64, juint64_t;
#define JINT64_PRI "ll"
#endif

#endif
