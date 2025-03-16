/* memory
 * mem_intern
 * mem_dup
 * MEM_ZERO (Win32 ZeroMemory)
 */

#ifndef JMEM_H
#define JMEM_H 1

#include <stddef.h>

#if __cplusplus
extern "C" {
#endif

// memoize arbitrary data
// void *jmem_intern(void* pointer, size_t n);

/* memset(p, 0, n) */
void jmemset0(void *p, size_t n);

/* memset0 a pointer */
#define JMEMSET0_POINTER(pointer) (jmemset0(pointer, sizeof(*(pointer))))

/* memset0 a value. */
#define JMEMSET0_VALUE(value) JMEMSET0_POINTER(&(value))

#if __cplusplus
}
#endif

#endif
