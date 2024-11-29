/* Based on Windows CONTAINING_RECORD and EFI CR. */

#ifndef JBASE_OF_H
#define JBASE_OF_H

#include <stddef.h>

#define JBASE_OF(container, member, value) \
    ((container*)(((char*)value) - offsetof(container, member)))

#endif
