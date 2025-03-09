/* memory
 * mem_intern
 * mem_dup
 * MEM_ZERO (Win32 ZeroMemory)
 */

#ifndef JMEM_H
#define JMEM_H 1

#include "jssize.h"
#include "jvoidp.h"

void *jmem_intern(jvoidp p, jssize n);

#define JMEM_ZERO(p, n) (memset((p), 0, (n)))

#endif
