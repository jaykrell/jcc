/* memory
 * mem_intern
 * mem_dup
 */

#include "jcharp.h"
#include "jssize.h"

void *jmem_intern(jcharp p, jssize n) { return 0; }

void jmemset0(void* p, size_t n)
{
	memset(p, 0, n);
}
