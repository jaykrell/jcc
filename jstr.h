/* use instead an array of chars */

#ifndef jstr_H
#define jstr_H

#include "jbase.h"
#include "jtype.h"
//#include "jvec.h"

/* jstr is a jvec(char) with terminal nul */

extern jtype jtype_string;

/* runtime type */
typedef struct jstr
{
    jtype* type;
    jssize len;
    char* chars;
} jstr;

jstr* jstr_new(void);
jerr jstr_append(jstr* str, const char* s, jssize len);

#endif
