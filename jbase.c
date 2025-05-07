
#include "jbase.h"

char* jbase(char* value, size_t offset)
{
    return value - (value ? offset : 0);
}
