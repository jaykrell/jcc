#include "jstr.h"
#include "jvec.h"
#include <stdlib.h>
#include "jbase_of.h"

typedef struct jstring_internal
{
    jvec_char vec;
    jstring   str;
} jstring_internal;

jstring* jstring_new(void)
{
    jstring_internal* str = (jstring_internal*)calloc(1, sizeof(*str));
    jvec_char_init(&str->vec);
    return &str->str;
}

jerr jstring_append(jstring* str, const char* s, jlong len)
{
    jstring_internal* i = JBASE_OF(jstring_internal, str, str);
    jerr err = i->vec.push_back(&i->vec, (char*)s, len);
    if (err < 0) return err;
    str->chars = i->vec.begin;
    str->len   = i->vec.end - i->vec.begin;
    return 0;
}
