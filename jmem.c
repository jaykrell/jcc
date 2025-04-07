/* memory
 * mem_intern
 * mem_dup
 */

#include "jmem.h"
#include <string.h>

#if _MSC_VER
#pragma warning(disable : 4100) /* unused parameter */
#endif

void *jmem_intern(char *p, size_t n) { return 0; }

void jmemset0(void *p, size_t n) { memset(p, 0, n); }
