/* Based on m3core.h. */

#pragma once

/* Before including m3core.h, check if INCLUDED_M3CORE_H is defined.
 * This is not so much as to optimize compilation, but to enable
 * concatenating all the files. */
#ifndef INCLUDED_JCORE_H
#define INCLUDED_JCORE_H

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN 1
#endif
#define _NO_CRT_STDIO_INLINE 1 /* Do not accidentally export printf. */
#define _CRT_SECURE_NO_DEPRECATE 1
#define _CRT_NONSTDC_NO_DEPRECATE 1
#define _WINSOCK_DEPRECATED_NO_WARNINGS 1
#define _KERNEL32_ 1 /* inhibit declspec(dllimport) for consistency; it is not needed on functions */
#ifdef _MSC_VER
/* These two must come first. */
#pragma warning(disable:4616) /* there is no warning x (unavoidable if targeting multiple compiler versions) */
#pragma warning(disable:4619) /* there is no warning x (unavoidable if targeting multiple compiler versions) */
/* The rest are sorted. */
#pragma warning(disable:4115) /* named type definition in parentheses */
#pragma warning(disable:4100) /* unused parameter */
#pragma warning(disable:4127) /* conditional expression is constant */
#pragma warning(disable:4201) /* nonstandard extension: nameless struct/union */
#pragma warning(disable:4209) /* nonstandard extension: benign re-typedef */
#pragma warning(disable:4214) /* nonstandard extension: bitfield other than int */
#pragma warning(disable:4226) /* nonstandard extension: __export */
#pragma warning(disable:4242) /* 'return': conversion from '' to '', possible loss of data */
#pragma warning(disable:4244) /* integer conversion */
#pragma warning(disable:4255) /* () change to (void) */
#pragma warning(disable:4514) /* unused inline function removed */
#pragma warning(disable:4705) /* statement has no effect for merely using assert() at -W4 */
#pragma warning(disable:4710) /* function not inlined */
#pragma warning(disable:4668) /* #if of undefined symbol */
#pragma warning(disable:4820) /* padding inserted */
#pragma warning(disable:5045) /* Compiler will insert Spectre mitigation for memory load if /Qspectre switch specified */
#endif

#ifdef _WIN32
#include "winsock2.h"
#include "ws2tcpip.h"
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
#define J_NO_INLINE __attribute__((noinline))
#elif _MSC_VER >= 1300
#define J_NO_INLINE __declspec(noinline)
#else
#define J_NO_INLINE /* nothing */
#endif

#ifdef __osf__
/* To get struct tm.tm_gmtoff, tm_zone. Would be good to autoconf this? */
#ifndef _OSF_SOURCE
#define _OSF_SOURCE
#endif
/* For socklen_t. Would be good to autoconf this.
 * This also gives us "uin-len".
 */
#ifndef _POSIX_PII_SOCKET
#define _POSIX_PII_SOCKET
#endif
/* More clearly get "uin-len". */
#ifndef _SOCKADDR_LEN
#define _SOCKADDR_LEN
#endif
/* Request 64bit time_t. Not available on v4. Would be good to autoconf this.
 * We later check for TIMEVAL64TO32/TIMEVAL32TO64 to see if this works.
 */
#ifndef _TIME64_T
#define _TIME64_T
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
 * Support for older is easy but absent.
 */
#define __MAKECONTEXT_V2_SOURCE
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
#define _REENTRANT
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
#define _LARGEFILE
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
#define _DARWIN_FEATURE_64_ONLY_BIT_INODE
#endif
#endif

/* Explicit __cdecl supports building Windows/x86 code
 * with non-default calling convention (cl /Gz /Gr /Gv),
 * while retaining constant meaning for Modula-3 m3core and
 * m3c output. This is messy and causes many warnings on Haiku/amd64.
 *
 * Explicit __stdcall is for a few helper functions on Windows/x86.
 * This also produces warnings on Haiku/amd64 but not as many as __cdecl.
 *
 * cdecl/stdcall have essentially no meaning outside
 * of Windows/x86. They might have meaning on Windows/amd64
 * compiled with /Gv for default vector call. (Or only for certain parameter lists?)
 * Do not do that?
 * They might have meaning on Haiku/x86?
 * They are correctly placed for Haiku/gcc, for functions
 * but maybe not function pointers. The warnings are
 * that Haiku/amd64 ignores them and/or they are misplaced for function pointers.
 *
 * We should probably change the helpers to cdecl.
 * Explicit stdcall would remain in use then only for
 * imported Windows functions (unless we wrap them all as we do for Posix).
*/
#if !defined(_WIN32) && !defined(__CYGWIN__)
#undef __cdecl
#undef __stdcall
#define __cdecl /* nothing */
#define __stdcall /* nothing */
#endif
// Prior definition that is good but produces many warnings on Haiku.
//#if !defined(_MSC_VER) && !defined(__cdecl)
//#define __cdecl /* nothing */
//#endif
//#if !defined(_MSC_VER) && !defined(__stdcall)
//#define __stdcall /* nothing */
//#endif

#ifndef _MSC_VER
#define __try /* nothing */
#define __finally /* nothing */
#endif

#ifdef __cplusplus
#define J_EXTERN_C         extern "C"
#define J_EXTERN_C_BEGIN   extern "C" {
#define J_EXTERN_C_END     }
#define M3EXTERNC_BEGIN     extern "C" {
#define J_EXTERNC_BEGIN    extern "C" {
#define M3EXTERNC_END       }
#define J_EXTERNC_END      }
#else
#define J_EXTERN_C         /* nothing */
#define M3EXTERNC_BEGIN     /* nothing */
#define J_EXTERN_C_BEGIN   /* nothing */
#define J_EXTERN_C_END     /* nothing */
#define J_EXTERNC_BEGIN    /* nothing */
#define M3EXTERNC_END       /* nothing */
#define J_EXTERNC_END      /* nothing */
#endif

#define JPASTE_(a, b) a##b
#define JPASTE(a, b)  JPASTE_(a, b)

#define JPASTE3_(a, b, c) a##b##c
#define JPASTE3(a, b, c)  JPASTE3_(a, b, c)

#define AGGREGATE_TYPEDEF(aggr, x) aggr x; typedef aggr x x;
#define STRUCT_TYPEDEF(x) AGGREGATE_TYPEDEF (struct, x)
#define UNION_TYPEDEF(x)  AGGREGATE_TYPEDEF (union, x)

STRUCT_TYPEDEF(linger)
STRUCT_TYPEDEF(sockaddr)
STRUCT_TYPEDEF(sockaddr_storage)
STRUCT_TYPEDEF(sockaddr_un)  /* unix fifo socket address */
STRUCT_TYPEDEF(sockaddr_in)  /* internet ipv4 socket address */
STRUCT_TYPEDEF(sockaddr_in6) /* internet ipv6 socket address */
STRUCT_TYPEDEF(in_addr)      /* 4 bytes within sockaddr_in */
STRUCT_TYPEDEF(in6_addr)     /* 16 bytes within sockaddr_in */

#include <sys/types.h>
#include <sys/stat.h>
#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <signal.h>
#include <math.h>
#include <limits.h>
#include <setjmp.h>

/* This supports very old toolsets. */
#ifdef _WIN32
#ifndef WIN32
#define WIN32
#endif

#include <direct.h>
#include <io.h>
#include <process.h>
typedef ptrdiff_t ssize_t;
#else
#include <sys/ioctl.h>
#include <sys/mman.h>
#ifndef __DJGPP__
#include <sys/socket.h>
#endif
#include <sys/time.h>
/* Check if this system really supports _TIME64_T, i.e. Tru64 v5.1 or later. */
#if defined(_TIME64_T) && !defined(TIMEVAL64TO32) && !defined(TIMEVAL32TO64)
#undef _TIME64_T
#endif
#include <sys/wait.h>
#include <dirent.h>
#include <grp.h>
#ifndef __DJGPP__
#include <netdb.h>
#endif
#ifndef __DJGPP__
#include <pthread.h>
#endif
#include <unistd.h>
#include <pwd.h>
#ifndef __DJGPP__
#include <semaphore.h>
#endif
#if !(defined (__OpenBSD__) || defined (__CYGWIN__) || defined (__vms) || defined (__HAIKU__) || defined (__DJGPP__))
#include <sys/ucontext.h>
#ifndef __APPLE__
/* OpenBSD 4.3, 4.7: ucontext.h doesn't exist, ucontext_t is in signal.h
   Cygwin: no state provided to signal handler?
   Apple: http://tinderbox.elegosoft.com/tinderbox/cgi-bin/gunzip.cgi\
          ?tree=cm3&brief-log=1258879870.10595#err9
          /usr/include/ucontext.h:42:2: error: #error ucontext routines are
          deprecated, and require _XOPEN_SOURCE to be defined
          http://duriansoftware.com/joe/PSA:-avoiding-the-%22ucontext-\
          routines-are-deprecated%22-error-on-Mac-OS-X-Snow-Leopard.html */
#include <ucontext.h>
#endif /* Apple */
#endif /* OpenBSD, Cygwin, VMS */
#define ZeroMemory(a, b) (memset((a), 0, (b)))
// Posix says include <arpa/inet.h>, but FreeBSD 4 inet.h
// requires netinet/in.h
#include <netinet/in.h>
#ifndef __DJGPP__
#include <arpa/inet.h>
#include <netinet/tcp.h>
#include <sys/un.h>
#include <sys/select.h>
#endif
#endif /* Win32 vs. Posix */

#define ZERO_MEMORY(a) (ZeroMemory(&(a), sizeof(a)))

#ifdef __INTERIX
#include <utime.h>
#endif

#if UCHAR_MAX == 0x0FFUL
typedef   signed char        INT8; // TODO: C99
typedef unsigned char       UINT8;
#else
#error unable to find 8bit integer
#endif
#if USHRT_MAX == 0x0FFFFUL
typedef          short      INT16;
typedef unsigned short     UINT16;
#else
#error unable to find 16bit integer
#endif
#if UINT_MAX == 0x0FFFFFFFFUL
typedef          int        INT32;
typedef unsigned int       UINT32;
#elif ULONG_MAX == 0x0FFFFFFFFUL
typedef          long       INT32;
typedef unsigned long      UINT32;
#else
#error unable to find 32bit integer
#endif

/* Support pre-C99 for Windows and VMS and any system with an __int64 macro. */
#if !defined(_LONGLONG) && (defined(_MSC_VER) || defined(__DECC) || defined(__DECCXX) || defined(__int64))
typedef          __int64    int64_t;
typedef unsigned __int64   UINT64;
#else
typedef          long long  int64_t;
typedef unsigned long long uint64_t;
#endif

#if __cplusplus
extern "C" {
#endif

// On all known systems, sizeof(size_t) == sizeof(intptr_t) == sizeof(void*).
// Except VMS.
// On VMS/Alpha confirmed April 2021:
//   size_t is always 32bits, allocations are limited to 4GB.
//   void* is 32bits or 64bits, depending on command line and pragmas
//    e.g. cc /pointer_size=64
//   intptr_t is always 64bits, can always store a pointer.
// It is quite possible that VMS/IA64 and VMS/AMD64 are the same.
// These environments provide maximum compatibility with older 32bit code,
//  while still changing instruction set.
/* commented out is correct, but so is the #else */
/*#if defined(_WIN64) || __INITIAL_POINTER_SIZE == 64 || defined(__LP64__) || defined(_LP64) || __WORDSIZE == 64*/
#if __INITIAL_POINTER_SIZE == 64
typedef INT64 INTEGER;
typedef UINT64 WORD_T;
#else
typedef ptrdiff_t INTEGER;
typedef size_t WORD_T;
#endif

#if 1 /* Some compilers don't like this, will adjust as needed. e.g. old SGI */
#if __GNUC__ /* gcc 4.8 warns about unused local typedefs. */
#define GCC_ATTRIBUTE_UNUSED __attribute__ ((unused))
#else
#define GCC_ATTRIBUTE_UNUSED /* nothing */
#endif
#define J_STATIC_ASSERT(expr) GCC_ATTRIBUTE_UNUSED typedef char M3PASTE(j_static_assert, __LINE__)[(expr)?1:-1]
#else
#define J_STATIC_ASSERT(expr) assert(expr)
#endif

#define J_FIELD_SIZE(type, field) (sizeof((type*)0)->field)
#define J_SIZE_THROUGH_FIELD(type, field) (offsetof(type, field) + J_FIELD_SIZE(type, field))

typedef union jtrace_t
{
    int all;
    struct
    {
        unsigned chdir : 1;
        unsigned close : 1;
        unsigned creat : 1;
        unsigned fstat : 1;
        unsigned open : 1;
        unsigned readdir : 1;
        unsigned stat : 1;
        unsigned write : 1;
    } s;
} m3core_trace_t;

extern jtrace_t jtrace;

#ifdef __cplusplus
} /* extern "C" */
#endif

// from m3c
// http://c.knowcoding.com/view/23699-portable-alloca.html
// Find a good version of alloca.
#ifndef alloca
# ifdef __GNUC__
#  define alloca __builtin_alloca
//This was thought to handle VMS but has never been tested.
//# elif !defined(__osf__) && (defined(__DECC) || defined(__DECCXX))
//#  define alloca(x) __ALLOCA(x)
# elif defined(_MSC_VER)
#ifdef __cplusplus
extern "C" {
#endif
   void * __cdecl _alloca(size_t size);
#ifdef __cplusplus
} /* extern "C" */
#endif
#  define alloca _alloca
# else
/* This is verified correct for Solaris, Tru64/OSF1, HP-UX, documented correct for AIX. TODO: Irix, etc. */
#  include <alloca.h>
# endif
#endif

#endif
