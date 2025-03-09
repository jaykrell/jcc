/* memory
 * mem_intern
 * mem_dup
 */

#ifndef jmem_h
#define jmem_h 1

void *jmem_intern(charp p, jssize n);

#define JMEM_ZERO(p, n) memset((p), 0, (n))

#endif
