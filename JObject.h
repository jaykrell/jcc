#ifndef JOBJECT_H
#define JOBJECT_H

#include "jbase.h"
#include "JType.h"

struct JObject;
typedef struct JObject JObject;

typedef struct JObject {
    JType* t;
} JObject;

#endif
