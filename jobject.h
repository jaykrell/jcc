#if !JOBJECT_H
#define JOBJECT_H

#include "jbase.h"
#include "jtype.h"

struct JObject;
typedef struct jobject jobject;

typedef struct jobject {
  jtype *t;
} jobject;

#endif
