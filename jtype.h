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
  void (*init)(void *, jlong);
  void (*cleanup)(void *, jlong);
  jerr (*copy)(void *, void **);
  jerr (*copy_to)(void *, void *);
  jerr (*move)(void *, void *, jssize_t);
  jlong (*valid)(void *);
  void (*begin)(void *, void **);
  void (*end)(void *, void **);
  void (*inc_in_place)(void *);
  void (*dec_in_place)(void *);
  jlong (*size)(void *);
  void (*dump)(void *);
};

#endif
