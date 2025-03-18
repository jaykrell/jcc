/* Based on m3core.h. */

#pragma once

/* Before including m3core.h, check if INCLUDED_M3CORE_H is defined.
 * This is not so much as to optimize compilation, but to enable
 * concatenating all the files. */
#ifndef INCLUDED_JCORE_H
#define INCLUDED_JCORE_H 1

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
#pragma warning(disable : 4100) /* unused parameter */
#pragma warning(disable : 4115) /* named type definition in parentheses */
#pragma warning(disable : 4127) /* conditional expression is constant */
#pragma warning(                                                               \
    disable : 4201) /* nonstandard extension: nameless struct/union */
#pragma warning(disable : 4209) /* nonstandard extension: benign re-typedef */
#pragma warning(                                                               \
    disable : 4214) /* nonstandard extension: bitfield other than int */
#pragma warning(disable : 4226) /* nonstandard extension: __export */
// #pragma warning(disable:4242) /* 'return': conversion from '' to '', possible
// loss of data */ #pragma warning(disable:4244) /* integer conversion */
// #pragma warning(disable:4255) /* () change to (void) */
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

#define JPASTE_(a, b) a##b
#define JPASTE(a, b) JPASTE_(a, b)

#define JPASTE3_(a, b, c) a##b##c
#define JPASTE3(a, b, c) JPASTE3_(a, b, c)

#define AGGREGATE_TYPEDEF(aggr, x)                                             \
  aggr x;                                                                      \
  typedef aggr x x;
#define STRUCT_TYPEDEF(x) AGGREGATE_TYPEDEF(struct, x)
#define UNION_TYPEDEF(x) AGGREGATE_TYPEDEF(union, x)

typedef ptrdiff_t ssize_t;
#define ZeroMemory(a, b) (memset((a), 0, (b)))

#if __cplusplus
extern "C" {
#endif

#if 1 /* Some compilers don't like this, will adjust as needed. e.g. old SGI   \
       */

#if __GNUC__ /* gcc 4.8 warns about unused local typedefs. */
#define GCC_ATTRIBUTE_UNUSED __attribute__((unused))
#else
#define GCC_ATTRIBUTE_UNUSED /* nothing */
#endif

#define J_STATIC_ASSERT(expr)                                                  \
  GCC_ATTRIBUTE_UNUSED typedef char JPASTE(j_static_assert,                    \
                                           __LINE__)[(expr) ? 1 : -1]

#else
#define J_STATIC_ASSERT(expr) assert(expr)
#endif

#define J_FIELD_SIZE(type, field) (sizeof((type *)0)->field)
#define J_SIZE_THROUGH_FIELD(type, field)                                      \
  (offsetof(type, field) + J_FIELD_SIZE(type, field))

#if __cplusplus
} /* extern "C" */
#endif

// from m3c
// http://c.knowcoding.com/view/23699-portable-alloca.html
// Find a good version of alloca.
#ifndef alloca
#ifdef __GNUC__
#define alloca __builtin_alloca
// This was thought to handle VMS but has never been tested.
// # elif !defined(__osf__) && (defined(__DECC) || defined(__DECCXX))
// #  define alloca(x) __ALLOCA(x)
#elif defined(_MSC_VER)
#ifdef __cplusplus
extern "C" {
#endif
void *__cdecl _alloca(size_t size);
#ifdef __cplusplus
} /* extern "C" */
#endif
#define alloca _alloca
#else
/* This is verified correct for Solaris, Tru64/OSF1, HP-UX, documented correct
 * for AIX. TODO: Irix, etc. */
#include <alloca.h>
#endif
#endif

#endif
