#ifndef JTYPE_H
#define JTYPE_H

#include "jbase.h"

struct jtype;
typedef struct jtype jtype;

/* runtime type */
struct jtype {
    char* type_name; /* not object name */
    jbool is_fixed_size;
    jlong fixed_size;
    void  (*init)(voidp, jlong);
    void  (*cleanup)(voidp, jlong);
    jerr  (*copy)(voidp, voidp*);
    jerr  (*copy_to)(voidp, voidp);
    jerr  (*move)(voidp, voidp, long);
    jlong (*valid)(voidp);
    void  (*begin)(voidp, voidp*);
    void  (*end)(voidp, voidp*);
    void  (*inc_in_place)(voidp);
    void  (*dec_in_place)(voidp);
    jlong (*size)(voidp);
    void  (*dump)(voidp);
};

#endif
