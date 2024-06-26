#ifndef JTYPE_H
#define JTYPE_H

#include "jbase.h"

/* runtime type */
typedef struct jrtype
{
    char* name;
    jbool is_fixed_size;
    Long fixed_size;
    void (*free)(voidp);
    Long (*dup)(voidp, voidp*);
    Long (*dupto)(voidp, voidp);
    Long (*valid)(voidp);
    void (*begin)(voidp, voidp*);
    void (*end)(voidp, voidp*);
    void (*inc)(voidp);
    Long (*size)(voidp);
    void (*dump)(voidp);
} jrtype;

typedef struct jobject
{
    jrtype* t;

} jobject;

#endif
