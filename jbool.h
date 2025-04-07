#ifndef JBOOL_H
#define JBOOL_H 1

#include <stdbool.h>

#if 0

typedef char jbool;

#ifndef __cplusplus
#define true 1
#define false 0
#endif

#else

typedef bool jbool;

#endif

#endif
