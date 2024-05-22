#pragma once

#include "jbase.h"

/* runtime type */
typedef struct jrtype
{
    char* name;
    jbool is_fixed_size;
    Int fixed_size;
    void (*free)(voidp);
    Int (*dup)(voidp, voidp*);
    Int (*valid)(voidp);
    void (*begin)(voidp, voidp*);
    void (*end)(voidp, voidp*);
    void (*inc)(voidp);
    Int (*size)(voidp);
    void (*dump)(voidp);
} jrtype;
