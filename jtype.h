#if !JTYPE_H
#define JTYPE_H 1

#include "jbool.h"
#include "jerr.h"
#include "jssize.h"

struct jtype;
typedef struct jtype jtype;

/* runtime type */
struct jtype {
  char *type_name; /* not object name */
  jbool is_fixed_size;
  jbool pad[7];
  ssize_t fixed_size;
  void (*init)(void *, ssize_t);
  void (*cleanup)(void *, ssize_t);
  jerr (*copy)(void *, void **);
  jerr (*copy_to)(void *, void *);
  jerr (*move)(void *, void *, ssize_t);
  ssize_t (*valid)(void *);
  void (*begin)(void *, void **);
  void (*end)(void *, void **);
  void (*inc_in_place)(void *);
  void (*dec_in_place)(void *);
  ssize_t (*size)(void *);
  void (*dump)(void *);
};

#endif
