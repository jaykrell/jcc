#ifndef JTYPE_H
#define JTYPE_H

#include "jbase.h"
#include "jerr.h"

typedef struct jtype jtype;

/* runtime type */
struct jtype
{
    char* type_name; /* not object name */
    jbool is_fixed_size;
    jbool pad[7];
    jssize fixed_size;
    void  (*init)(voidp, jssize);
    void  (*cleanup)(voidp, jssize);
    jerr  (*copy)(voidp, voidp*);
    jerr  (*copy_to)(voidp, voidp);
    jerr  (*move)(voidp, voidp, long);
    jssize (*valid)(voidp);
    void  (*begin)(voidp, voidp*);
    void  (*end)(voidp, voidp*);
    void  (*inc_in_place)(voidp);
    void  (*dec_in_place)(voidp);
    jssize (*size)(voidp);
    void  (*dump)(voidp);
};

#endif
