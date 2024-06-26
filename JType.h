#ifndef JTYPE_H
#define JTYPE_H

#include "jbase.h"

struct JType;
typedef typedef struct JType JType;

/* runtime type */
struct JType {
    char* type_name; /* not object name */
    jbool is_fixed_size;
    Long fixed_size;
    void (*free)(voidp);
    Long (*dup)(voidp, voidp*);
    Long (*dupto)(voidp, voidp);
    Long (*valid)(voidp);
    void (*begin)(voidp, voidp*);
    void (*end)(voidp, voidp*);
    void (*inc_in_place)(voidp);
    void (*dec_in_place)(voidp);
    Long (*size)(voidp);
    void (*dump)(voidp);
};

#endif
