#include "jarray.h"

void jarray_free(voidp av)
{
    jarray_t* a = (jarray_t*)av;
    jarray_iter iter = {0};
    jarray_iter end = {0};

    a->t->begin(&iter);
    a->t->end(&end);

    while (!iter.eq(&end))
    {
        jobject_t* data = {0};

        iter.get(&data);
        data->t->free(data);
        iter.inc(&iter);
    }
}

jlong jarray_dup(voidp, voidp*)
{
}

jlong jarray_dupto(voidp, voidp)
{
}

jlong jarray_valid(voidp)
{
}

void jarray_begin(voidp, voidp*);

void jarray_end(voidp av, voidp* iter)
{
}


void jarray_inc(voidp pv)
{
}

jlong jarray_size(voidp pv)
{
}

void jarray_dump(voidp pv)
{
}


jarrayt Jarrayt =
{
    "array",
    0, /* fixed */
    0, /* fixed */
};

int jarray_init(jarray_t* ary)
{

}
