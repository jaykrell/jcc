/* tests */

#include "jvec.h"
#include "jlist.h"
#include "jbase_of.h"
#include <stdio.h>

typedef struct list_int_t {
	jlong value;
	jlist_t list;
} list_int_t;

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
	list_int_t li1 = {1};
	list_int_t li2 = {2};
	list_int_t li3 = {3};
	list_int_t li4 = {4};

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
	jlist_init(&l1);
    printf("%d\n", (int)jlist_size(&l1));
	jlist_append(&l1, &li1.list);
    printf("%d\n", (int)jlist_size(&l1));
	jlist_append(&l1, &li2.list);
    printf("%d\n", (int)jlist_size(&l1));
	jlist_append(&l1, &li3.list);
    printf("%d\n", (int)jlist_size(&l1));
	jlist_append(&l1, &li4.list);
    printf("%d\n", (int)jlist_size(&l1));

	jlist_remove_last(&l1);
    printf("%d\n", (int)jlist_size(&l1));

    printf("%d\n", JBASE_OF(list_int_t, list, l1.flink)->value);
    printf("%d\n", JBASE_OF(list_int_t, list, l1.flink->flink)->value);
    printf("%d\n", JBASE_OF(list_int_t, list, l1.flink->flink->flink)->value);

	jlist_remove_first(&l1);
    printf("%d\n", (int)jlist_size(&l1));
    printf("%d\n", JBASE_OF(list_int_t, list, l1.flink)->value);
}
