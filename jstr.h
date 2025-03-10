/* It remains to be seen if "string" is useful, vs. vector */
/* use instead an array of chars, maybe */

#ifndef JSTR_H
#define JSTR_H 1

#include "jbase.h"
#include "jtype.h"
/*#include "jvec.h"*/

/* jstr is a jvec(char) with terminal nul */

extern jtype jtype_string;

/* runtime type */
typedef struct jstr {
  jtype *type;
  jlong len;
  char *chars;
} jstr;

jstr *jstr_new(void);
jerr jstr_append(jstr *str, const char *s, jlong len);

#endif
