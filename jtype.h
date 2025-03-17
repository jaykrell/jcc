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
  ptrdiff_t fixed_size;
  void (*init)(void *, ptrdiff_t);
  void (*cleanup)(void *, ptrdiff_t);
  jerr (*copy)(void *, void **);
  jerr (*copy_to)(void *, void *);
  jerr (*move)(void *, void *, jssize_t);
  ptrdiff_t (*valid)(void *);
  void (*begin)(void *, void **);
  void (*end)(void *, void **);
  void (*inc_in_place)(void *);
  void (*dec_in_place)(void *);
  ptrdiff_t (*size)(void *);
  void (*dump)(void *);
};

#endif
