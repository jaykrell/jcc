#ifndef JHASHCODE_H
#define JHASHCODE_H 1
/*TODO: This should be the largest integer available, such as 64bits on 32bit systems.
*
* Currently it is 64bits on Unix64, Win32, and Win64, but not Unix32.
* autoconf for:
*   unsigned long long
*   unsigned __int64
*   uint64_t
*   uintmax_t
*/
#include "julong.h"
typedef julong jhashcode_t;

#endif
