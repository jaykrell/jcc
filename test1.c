/* tests */

#pragma warning(disable:4100) /* unused parameter */
#include "jvec.h"
#include "jlist.h"
#include "jbase_of.h"
#include "jmem.h"
#include "jhash.h"
#include <assert.h>
#include <stdio.h>

typedef struct jlist_int_t {
	jlong value;
	jlist_t list;
} jlist_int_t;

jlong list_enumerate_test1(voidp context, voidp element)
{
	printf("list_enumerate_test1: %d\n", (int)((jlist_int_t*)element)->value);
	return 1;
}

jlong list_enumerate_test2(voidp context, voidp element)
{
	printf("list_enumerate_test2: %d\n", (int)JBASE_OF(jlist_int_t, list, element)->value);
	return 1;
}

int main(void)
{
    jvec_double vd = {0};
    jvec_int vi = {0};
    jvec_int_iter vii = {0};
    int a = 1;
    double b = 1;
    jlong size = {0};
    jlong i = {0};
	jlist_t l1 = {0};
	jlist_int_t li1 = {1};
	jlist_int_t li2 = {2};
	jlist_int_t li3 = {3};
	jlist_int_t li4 = {4};

	/* test vector */

    jvec_int_init (&vi);
    jvec_double_init (&vd);

    size = vi.size(&vi);
    printf("%" JLONG_PRI "d\n", size);
    vi.push_back(&vi, &a, 1);
    size = vi.size(&vi);
    printf("%" JLONG_PRI "d\n", size);
    ++a;
    vi.push_back(&vi, &a, 1);
    vi.push_back(&vi, &a, 1);
    vi.push_back(&vi, &a, 1); a *= 2;
    vi.push_back(&vi, &a, 1);
    vi.push_back(&vi, &a, 1);
    vi.push_back(&vi, &a, 1); ++a;
    vi.push_back(&vi, &a, 1);
    vi.push_back(&vi, &a, 1); a *= 3;
    vi.push_back(&vi, &a, 1);
    vi.push_back(&vi, &a, 1);
    vi.push_back(&vi, &a, 1); ++a;
    vi.push_back(&vi, &a, 1);
    vi.push_back(&vi, &a, 1); a *= 4;
    vi.push_back(&vi, &a, 1);
    vi.push_back(&vi, &a, 1); ++a;
    vi.push_back(&vi, &a, 1);
    vi.push_back(&vi, &a, 1);
    printf("cap: %" JLONG_PRI "d\n", size = vi.capacity(&vi));
    printf("size: %" JLONG_PRI "d\n", size = vi.size(&vi));

    for (i = 0; i < size; ++i)
        printf("%d ", vi.begin[i]);

    for (vii = vi.beginiter(&vi); vii.cmp(vii, vi.enditer(&vi)); vii = vii.inc(vii))
        printf("%d ", *vii.get(vii));

    vd.push_back(&vd, &b, 1);
    size = vd.size(&vd);
    for (i = 0; i < size; ++i)
        printf("%f ", vd.begin[i]);

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
