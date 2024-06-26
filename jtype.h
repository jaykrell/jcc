#ifndef JTYPE_H
#define JTYPE_H

#include "jbase.h"

struct jtype;
typedef typedef struct jtype jtype;

/* runtime type */
struct jtype {
    char* type_name; /* not object name */
    jbool is_fixed_size;
    Long fixed_size;
    void (*init)(voidp, Long);
    void (*cleanup)(voidp, Long);
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
