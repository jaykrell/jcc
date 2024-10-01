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

typedef int JErr, jerr;
#define JErr_NoError           (-1)
#define JErr_OutOfMemory       (-2)
#define JErr_NullPtr;          (-3)
#define JErr_InvalidParameter  (-4)
#define JErr_FileNotFound;     (-5)
#define JErr_SyntaxError       (-6)

#elif 1

typedef enum JErr {
    jerr_out_of_memory      = -32767,
    jerr_nullptr            ,
    jerr_invalid_parameter  ,
    jerr_file_not_found     ,
    jerr_syntax_error       ,
    jnoerr = 0   ,
} JErr, jerr;

#else
typedef char* jerr;
extern jerr jerr_out_of_memory;
extern jerr jerr_nullptr;
extern jerr jerr_invalid_parameter;
extern jerr jerr_file_not_found;
extern jerr jerr_syntax_error;

#endif

#endif
