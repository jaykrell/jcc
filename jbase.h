#if !JBASE_H
#define JBASE_H 1

/* Based on Windows CONTAINING_RECORD and EFI CR. */

#include <stddef.h>

#define JBASE(container, member, value)                                        \
  ((container *)(((char *)value) - offsetof(container, member)))

#endif
