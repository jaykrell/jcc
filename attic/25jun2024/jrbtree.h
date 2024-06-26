/* red black (balanced) tree */

#ifndef JRBTREE_H
#define JRBTREE_H

#include "jbase.h"
#include "jrtype.h"
#include "jarray.h"

struct jrbtree_entry
{
    voidp key;
    voidp value;
};

struct jrbtree
{
    jrtype* tkey;
    jrtype* tvalue;
};

#endif
