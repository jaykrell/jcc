#ifndef JTYPE_H
#define JTYPE_H

#include "jbase.h"
#include "jerr.h"
#include "jssize.h"

struct jtype;
typedef struct jtype jtype;

/* runtime type */
struct jtype {
  char *type_name; /* not object name */
  jbool is_fixed_size;
  jbool pad[7];
  jlong fixed_size;
  void (*init)(jvoidp, jlong);
  void (*cleanup)(jvoidp, jlong);
  jerr (*copy)(jvoidp, jvoidp *);
  jerr (*copy_to)(jvoidp, jvoidp);
  jerr (*move)(jvoidp, jvoidp, jssize_t);
  jlong (*valid)(jvoidp);
  void (*begin)(jvoidp, jvoidp *);
  void (*end)(jvoidp, jvoidp *);
  void (*inc_in_place)(jvoidp);
  void (*dec_in_place)(jvoidp);
  jlong (*size)(jvoidp);
  void (*dump)(jvoidp);
};

#endif
