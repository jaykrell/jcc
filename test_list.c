/* test_list.c */

#pragma warning(disable:4100) /* unused parameter */
#include <string.h>
#include "jvoidp.h"
#include "jlist.h"
#include "jbase_of.h"
#include "jmem.h"
#include <assert.h>
#include <stdio.h>

typedef struct jlist_int_t {
    jlong value;
    jlist_t list;
} jlist_int_t;

jlong list_enumerate_test1(jvoidp context, jvoidp element)
{
    printf("list_enumerate_test1: %d\n", (int)((jlist_int_t*)element)->value);
    return 1;
}

jlong list_enumerate_test2(jvoidp context, jvoidp element)
{
    printf("list_enumerate_test2: %d\n", (int)JBASE_OF(jlist_int_t, list, element)->value);
    return 1;
}

/*************************************************************************************************/

int main(void)
{
    int a = 1;
    double b = 1;
    jlong size = {0};
    jlong i = {0};
    jlist_t l1 = {0};
    jlist_int_t li1 = {1};
    jlist_int_t li2 = {2};
    jlist_int_t li3 = {3};
    jlist_int_t li4 = {4};

    /* test list */

    {
        assert(0 == jlist_size(&l1));
        jlist_append(&l1, &li1.list);
        assert(1 == jlist_size(&l1));
        jlist_append(&l1, &li2.list);
        assert(2 == jlist_size(&l1));
        jlist_append(&l1, &li3.list);
        assert(3 == jlist_size(&l1));
        jlist_append(&l1, &li4.list);
        assert(4 == jlist_size(&l1));

        jlist_iterate(&l1, list_enumerate_test1, 0, offsetof(jlist_int_t, list));
        jlist_iterate(&l1, list_enumerate_test2, 0, 0);

        jlist_remove_last(&l1);
        assert(3 == jlist_size(&l1));
        printf("%d\n", (int)jlist_size(&l1));

        assert(1 == JBASE_OF(jlist_int_t, list, l1.flink)->value);
        assert(2 == JBASE_OF(jlist_int_t, list, l1.flink->flink)->value);
        assert(3 == JBASE_OF(jlist_int_t, list, l1.flink->flink->flink)->value);

        assert(3 == JBASE_OF(jlist_int_t, list, l1.blink)->value);
        assert(2 == JBASE_OF(jlist_int_t, list, l1.blink->blink)->value);
        assert(1 == JBASE_OF(jlist_int_t, list, l1.blink->blink->blink)->value);

        jlist_remove_first(&l1);
        assert(2 == jlist_size(&l1));
        printf("%d\n", (int)jlist_size(&l1));
        printf("%d\n", (int)JBASE_OF(jlist_int_t, list, l1.flink)->value);
        assert(2 == JBASE_OF(jlist_int_t, list, l1.flink)->value);
    }

    {
        JMEM_ZERO(&l1, sizeof(l1));
        assert(0 == jlist_size(&l1));
        jlist_prepend(&l1, &li1.list);
        assert(1 == jlist_size(&l1));
        jlist_prepend(&l1, &li2.list);
        assert(2 == jlist_size(&l1));
        jlist_prepend(&l1, &li3.list);
        assert(3 == jlist_size(&l1));
        jlist_prepend(&l1, &li4.list);
        assert(4 == jlist_size(&l1));

        jlist_iterate(&l1, list_enumerate_test1, 0, offsetof(jlist_int_t, list));
        jlist_iterate(&l1, list_enumerate_test2, 0, 0);

        jlist_remove_last(&l1);
        assert(3 == jlist_size(&l1));
        printf("%d\n", (int)jlist_size(&l1));

        assert(4 == JBASE_OF(jlist_int_t, list, l1.flink)->value);
        assert(3 == JBASE_OF(jlist_int_t, list, l1.flink->flink)->value);
        assert(2 == JBASE_OF(jlist_int_t, list, l1.flink->flink->flink)->value);

        assert(2 == JBASE_OF(jlist_int_t, list, l1.blink)->value);
        assert(3 == JBASE_OF(jlist_int_t, list, l1.blink->blink)->value);
        assert(4 == JBASE_OF(jlist_int_t, list, l1.blink->blink->blink)->value);

        jlist_remove_first(&l1);
        assert(2 == jlist_size(&l1));
        printf("%d\n", (int)jlist_size(&l1));
        printf("%d\n", (int)JBASE_OF(jlist_int_t, list, l1.flink)->value);
        assert(3 == JBASE_OF(jlist_int_t, list, l1.flink)->value);
    }
}
