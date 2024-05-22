#include "jbase.h"
#include "jrtype.h"

struct jarrayt
{
    jrtype t;
    void (*push_back)(voidp);
    void (*pop_back)(voidp);
};

struct jarray
{
    jrtype* type;
    jrtype* telem;
    charp begin, end, cap;
    Int size;
    Int capacity;
};
