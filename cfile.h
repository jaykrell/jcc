#if !CFILE_H
#define CFILE_H 1

#include "jcommon.h"
#include "jlist.h"
#include "jvec.h"

typedef struct cposition_t {
  int col;
  int row;
} cposition_t;

typedef struct cfile_t {
  jlist_t stack;
  /*int ch;*/
  /*int unget;*/
  jvec_char_t name;
  /* include "" searches first in dir */
  jvec_char_t dir;
  cposition_t position;
} cfile_t;

#endif
