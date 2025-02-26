#include "jstr.h"
#include "jvec.h"
#include <stdlib.h>
#include "jbase_of.h"

typedef struct jstr_internal
{
    jvec_char vec;
    jstr   str;
} jstr_internal;

jstr* jstr_new(void)
{
    jstr_internal* str = (jstr_internal*)calloc(1, sizeof(*str));
    jvec_char_init(&str->vec);
    return &str->str;
}

jerr jstr_append(jstr* str, const char* s, jssize len)
{
    jstr_internal* i = JBASE_OF(jstr_internal, str, str);
    jerr err = i->vec.push_back(&i->vec, (char*)s, len);
    if (err < 0) return err;
    str->chars = i->vec.begin;
    str->len   = i->vec.end - i->vec.begin;
    return 0;
}
