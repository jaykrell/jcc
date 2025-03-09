/* red black (balanced) tree */

#ifndef JRBTREE_H
#define JRBTREE_H

#include "jbase.h"
#include "jtype.h"
#include "jarray.h"

typedef struct jrbtree_entry jrbtree_entry;
typedef struct jrbtree jrbtree;

struct jrbtree_entry
{
    jvoidp key;
    jvoidp value;
};

struct jrbtree
{
    jtype* tkey;
    jtype* tvalue;
};

#endif
