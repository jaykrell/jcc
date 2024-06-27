#include "jbase.h"

#ifndef JERR_H
#define JERR_H

#if 0

struct JErrType; typedef struct JErrType JErrType;
struct JErr;     typedef struct JErr JErr, jerr;

struct JJErrType {
    void (*free)(JErr*);
}

struct JErr {
    int i;
    charp str;
    charp strdetail[2];
    jlong idetail[2]; /* e.g. invalid parameter index */
};

#define JERR_SUCCESS(x) (((x)->i) == 0)

extern JErr JErr_NoError;
extern JErr JErr_OutOfMemory;
extern JErr JErr_NullPtr;
extern JErr JErr_InvalidParameter;
extern JErr JErr_FileNotFound;
extern JErr JErr_SyntaxError;

#elif 0

typedef jlong JErr, jerr;
#define JErr_NoError           (-1)
#define JErr_OutOfMemory       (-2)
#define JErr_NullPtr;          (-3)
#define JErr_InvalidParameter  (-4)
#define JErr_FileNotFound;     (-5)
#define JErr_SyntaxError       (-6)

#else

typedef enum JErr {
    JErr_OutOfMemory       = 1 << 31,
    JErr_NullPtr           ,
    JErr_InvalidParameter  ,
    JErr_FileNotFound      ,
    JErr_SyntaxError       ,
} JErr, jerr;

#endif

#endif
