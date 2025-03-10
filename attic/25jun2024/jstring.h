/* use instead an array of chars */

#ifndef JSTRING_H
#define JSTRING_H

#include "jbase.h"

extern jrtype jtype_string;

/* runtime type */
typedef struct jstring {
  jtype *type;
  Int len;
  char *chars;
} jstring;

#endif
