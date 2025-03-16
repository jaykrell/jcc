/* Based on Windows CONTAINING_RECORD and EFI CR. */

#ifndef JBASE_OF_H
#define JBASE_OF_H 1

#include <stddef.h>

#define JBASE(container, member, value)                                     \
  ((container *)(((char *)value) - offsetof(container, member)))

#define JBASE_OF(container, member, value)                                     \
  ((container *)(((char *)value) - offsetof(container, member)))

#endif
