/* use instead an array of chars */

#ifndef JSTRING_H
#define JSTRING_H

#include "jbase.h"
#include "jtype.h"
//#include "jvec.h"

/* jstring is a jvec(char) with terminal nul */

extern jtype jtype_string;

/* runtime type */
typedef struct jstring
{
    jtype* type;
    jlong len;
    char* chars;
} jstring;

jstring* jstring_new(void);
jerr jstring_append(jstring* str, const char* s, jlong len);

#endif
