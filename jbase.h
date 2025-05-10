#if !JBASE_H
#define JBASE_H 1

/* Based on Windows CONTAINING_RECORD and EFI CR. */

#include <stddef.h>

char *jbase(char *value, size_t offset);

#define JBASE(container, member, value)                                        \
  ((container *)jbase(((char *)value), offsetof(container, member)))

#endif
