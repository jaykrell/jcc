/* see https://github.com/modula3/cm3/blob/master/m3-libs/m3core/src/m3core.h */

#if _MSC_VER > 1000
#pragma once
#endif

#ifndef INCLUDED_M3CORE_H
#define INCLUDED_M3CORE_H

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN 1
#endif
#define _NO_CRT_STDIO_INLINE 1 /* Do not accidentally export printf. */
#define _CRT_SECURE_NO_DEPRECATE 1
#define _CRT_NONSTDC_NO_DEPRECATE 1
#define _WINSOCK_DEPRECATED_NO_WARNINGS 1
#define _KERNEL32_                                                             \
  1 /* inhibit declspec(dllimport) for consistency; it is not needed on        \
       functions */
#ifdef _MSC_VER
/* These two must come first. */
#pragma warning(disable : 4616) /* there is no warning x (unavoidable if       \
                                   targeting multiple compiler versions) */
#pragma warning(disable : 4619) /* there is no warning x (unavoidable if       \
                                   targeting multiple compiler versions) */
/* The rest are sorted. */
#pragma warning(disable : 4115) /* named type definition in parentheses */
#pragma warning(disable : 4100) /* unused parameter */
#pragma warning(disable : 4127) /* conditional expression is constant */
#pragma warning(                                                               \
    disable : 4201) /* nonstandard extension: nameless struct/union */
#pragma warning(disable : 4209) /* nonstandard extension: benign re-typedef */
#pragma warning(                                                               \
    disable : 4214) /* nonstandard extension: bitfield other than int */
#pragma warning(disable : 4226) /* nonstandard extension: __export */
#pragma warning(disable : 4242) /* 'return': conversion from '' to '',         \
                                   possible loss of data */
#pragma warning(disable : 4244) /* integer conversion */
#pragma warning(disable : 4255) /* () change to (void) */
#pragma warning(disable : 4514) /* unused inline function removed */
#pragma warning(disable : 4705) /* statement has no effect for merely using    \
                                   assert() at -W4 */
#pragma warning(disable : 4710) /* function not inlined */
#pragma warning(disable : 4668) /* #if of undefined symbol */
#pragma warning(disable : 4820) /* padding inserted */
#pragma warning(                                                               \
    disable : 5045) /* Compiler will insert Spectre mitigation for memory load \
                       if /Qspectre switch specified */
#endif

typedef int BOOL;
#define TRUE 1
#define FALSE 0

/* __DARWIN_UNIX03 defaults to 1 on older and newer headers,
 * but older headers still have context "ss" instead of "__ss"
 * and such, so we have to force 0.
 * That is -- the defaults vary, the behavior of the newer
 * default is not available in older headers, so we must
 * force the older behavior, so that we can write one compatible source.
 */
#if defined(__APPLE__) && !defined(__DARWIN_UNIX03)
#define __DARWIN_UNIX03 0
#endif

#if __GNUC__ || __SUNPRO_C >= 0x590
#define M3_NO_INLINE __attribute__((noinline))
#elif _MSC_VER >= 1300
#define M3_NO_INLINE __declspec(noinline)
#else
#define M3_NO_INLINE /* nothing */
#endif

#ifdef __osf__
/* To get struct tm.tm_gmtoff, tm_zone. Would be good to autoconf this? */
#ifndef _OSF_SOURCE
#define _OSF_SOURCE 1
#endif
/* For socklen_t. Would be good to autoconf this.
 * This also gives us "uin-len".
 */
#ifndef _POSIX_PII_SOCKET
#define _POSIX_PII_SOCKET 1
#endif
/* More clearly get "uin-len". */
#ifndef _SOCKADDR_LEN
#define _SOCKADDR_LEN 1
#endif
/* Request 64bit time_t. Not available on v4. Would be good to autoconf this.
 * We later check for TIMEVAL64TO32/TIMEVAL32TO64 to see if this works.
 */
#ifndef _TIME64_T
#define _TIME64_T 1
#endif
#endif /* osf */

/* Autoconf: AC_SYS_LARGEFILE
 */
#define _FILE_OFFSET_BITS 64

/* const is extern const in C, but static const in C++,
 * but gcc gives a warning for the correct portable form "extern const",
 * unless you also forward declare:
 *  extern const int foo;
 *  extern const int foo = 123;
 */
#if defined(__cplusplus) || !defined(__GNUC__)
#define EXTERN_CONST extern const
#else
#define EXTERN_CONST const
#endif

#if defined(__sun) && !defined(__MAKECONTEXT_V2_SOURCE)
/* Support for userthreads on Solaris 9 4/03 and later.
 * Support for older is easy.
 * [m3core.h not likely to ever matter in jm3core.h]
 */
#define __MAKECONTEXT_V2_SOURCE 1
#endif

/* Autoconf: AC_USE_SYSTEM_EXTENSIONS */
/* Enable extensions on AIX 3, Interix.  */
#ifndef _ALL_SOURCE
#define _ALL_SOURCE 1
#endif
/* Enable GNU extensions on systems that have them.  */
#ifndef _GNU_SOURCE
#define _GNU_SOURCE 1
#endif
/* Enable threading extensions on Solaris.  */
#ifndef _POSIX_PTHREAD_SEMANTICS
#define _POSIX_PTHREAD_SEMANTICS 1
#endif
/* Enable extensions on HP NonStop.  */
#ifndef _TANDEM_SOURCE
#define _TANDEM_SOURCE 1
#endif
/* Enable general extensions on Solaris.  */
#ifndef __EXTENSIONS__
#define __EXTENSIONS__ 1
#endif

#ifndef _REENTRANT
#define _REENTRANT 1
#endif

/* AC_SYS_LARGEFILE */
#ifndef _LARGE_FILES
#define _LARGE_FILES 1
#endif

#ifndef _FILE_OFFSET_BITS
#define _FILE_OFFSET_BITS 64
#endif

/*#ifdef __vms*/
/* Enable support for files larger than 2GB.
 * Autoconf: AC_SYS_LARGEFILE?
 */
#ifndef _LARGEFILE
#define _LARGEFILE 1
#endif
/* Enable 32bit gids and reveal setreuids. */
#ifndef __USE_LONG_GID_T
#define __USE_LONG_GID_T 1
#endif
/* st_ino has three forms that all fit in the
 * same space; pick the one we want.
 */
#ifndef __USE_INO64
#define __USE_INO64 1
#endif
/*#endif*/

#if defined(__arm__) && defined(__APPLE__)
/* Reveal the correct struct stat? */
#ifndef _DARWIN_FEATURE_64_ONLY_BIT_INODE
#define _DARWIN_FEATURE_64_ONLY_BIT_INODE 1
#endif
#endif

/* Calling convention noise for Windows/x86 and possibly Haiku/x86. */
#if !defined(_WIN32) && !defined(__CYGWIN__)
#undef __cdecl
#undef __stdcall
#define __cdecl   /* nothing */
#define __stdcall /* nothing */
#endif
/* Prior definition that is good but produces many warnings on Haiku. */
#if 0
#if !defined(_MSC_VER) && !defined(__cdecl)
#define __cdecl /* nothing */
#endif
#if !defined(_MSC_VER) && !defined(__stdcall)
#define __stdcall /* nothing */
#endif
#endif

#ifndef _MSC_VER
#define __try     /* nothing */
#define __finally /* nothing */
#endif

#ifdef __cplusplus
#define J_EXTERN_C_BEGIN extern "C" {
#define J_EXTERN_C_END }
#else
#define J_EXTERN_C_BEGIN /* nothing */
#define J_EXTERN_C_END   /* nothing */
#endif

#define AGGREGATE_TYPEDEF(aggr, x)                                             \
  aggr x;                                                                      \
  typedef aggr x x;
#define STRUCT_TYPEDEF(x) AGGREGATE_TYPEDEF(struct, x)
#define UNION_TYPEDEF(x) AGGREGATE_TYPEDEF(union, x)

#include <limits.h>

#ifdef _WIN32
typedef ptrdiff_t ssize_t;
#else
#endif

#define ZERO_MEMORY(a) (ZeroMemory(&(a), sizeof(a)))

#if UCHAR_MAX == 0x0FFUL
typedef signed char INT8;
typedef unsigned char UINT8;
#else
#error unable to find 8bit integer
#endif
#if USHRT_MAX == 0x0FFFFUL
typedef short INT16;
typedef unsigned short UINT16;
#else
#error unable to find 16bit integer
#endif
#if UINT_MAX == 0x0FFFFFFFFUL
typedef int INT32;
typedef unsigned int UINT32;
#elif ULONG_MAX == 0x0FFFFFFFFUL
typedef long INT32;
typedef unsigned long UINT32;
#else
#error unable to find 32bit integer
#endif

/* Unix 64bit systems have 64bit long and do not require C99. */

#if ULONG_MAX > 0x0FFFFFFFFUL
typedef long INT64;
typedef unsigned long UINT64;
#else
/* Support pre-C99 for Windows and VMS and any system with an __int64 macro.*/
#if !defined(_LONGLONG) && (defined(_MSC_VER) || defined(__DECC) ||            \
                            defined(__DECCXX) || defined(__int64))
typedef __int64 INT64;
typedef unsigned __int64 UINT64;
#else
typedef long long INT64;
typedef unsigned long long UINT64;
#endif
#endif

typedef unsigned char BOOLEAN;

#define J_FIELD_SIZE(type, field) (sizeof((type *)0)->field)
#define J_SIZE_THROUGH_FIELD(type, field)                                      \
  (offsetof(type, field) + M3_FIELD_SIZE(type, field))

#if 0
// from m3c
// http://c.knowcoding.com/view/23699-portable-alloca.html
// Find a good version of alloca.
#ifndef alloca
#ifdef __GNUC__
#define alloca __builtin_alloca
//This was thought to handle VMS but has never been tested.
//# elif !defined(__osf__) && (defined(__DECC) || defined(__DECCXX))
//#  define alloca(x) __ALLOCA(x)
#elif defined(_MSC_VER)
#ifdef __cplusplus
extern "C" {
#endif
   void * __cdecl _alloca(size_t size);
#ifdef __cplusplus
} /* extern "C" */
#endif
#define alloca _alloca
#else
/* This is verified correct for Solaris, Tru64/OSF1, HP-UX, documented correct for AIX. TODO: Irix, etc. */
#include <alloca.h>
#endif
#endif
#endif

#endif
