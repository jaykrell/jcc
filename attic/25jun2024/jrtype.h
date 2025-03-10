#ifndef JTYPE_H
#define JTYPE_H

#include "jbase.h"

/* runtime type */
typedef struct jrtype {
  char *name;
  jbool is_fixed_size;
  Long fixed_size;
  void (*free)(jvoidp);
  Long (*dup)(jvoidp, jvoidp *);
  Long (*dupto)(jvoidp, jvoidp);
  Long (*valid)(jvoidp);
  void (*begin)(jvoidp, jvoidp *);
  void (*end)(jvoidp, jvoidp *);
  void (*inc)(jvoidp);
  Long (*size)(jvoidp);
  void (*dump)(jvoidp);
} jrtype;

typedef struct jobject {
  jrtype *t;

} jobject;

#endif
