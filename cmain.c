#include "jvec.h"
#include <stdio.h>

int main()
{
    jvec_double_iter vdi = {0};
    jvec_double_iter vdend = {0};
    jvec_double vd = {0};
    jvec_int vi = {0};
    jvec_int_iter vii = {0};
    jvec_int_iter viend = {0};
    int a = 1;
    double b = 1;
    jlong size = {0};
    jlong i = {0};

    jvec_int_init (&vi);
    jvec_double_init (&vd);

    size = vi.t->size(&vi);
    printf("%" JLONG_PRI "d\n", size);
    vi.t->push_back(&vi, &a);
    size = vi.t->size(&vi);
    printf("%" JLONG_PRI "d\n", size);
    ++a;
    vi.t->push_back(&vi, &a);
    vi.t->push_back(&vi, &a);
    vi.t->push_back(&vi, &a); a *= 2;
    vi.t->push_back(&vi, &a);
    vi.t->push_back(&vi, &a);
    vi.t->push_back(&vi, &a); ++a;
    vi.t->push_back(&vi, &a);
    vi.t->push_back(&vi, &a); a *= 3;
    vi.t->push_back(&vi, &a);
    vi.t->push_back(&vi, &a);
    vi.t->push_back(&vi, &a); ++a;
    vi.t->push_back(&vi, &a);
    vi.t->push_back(&vi, &a); a *= 4;
    vi.t->push_back(&vi, &a);
    vi.t->push_back(&vi, &a); ++a;
    vi.t->push_back(&vi, &a);
    vi.t->push_back(&vi, &a);
    printf("cap: %" JLONG_PRI "d\n", size = vi.t->capacity(&vi));
    printf("size: %" JLONG_PRI "d\n", size = vi.t->size(&vi));

    for (i = 0; i < size; ++i)
        printf("%d ", vi.begin[i]);

    viend = vi.t->end(&vi);
    for (vii = vi.t->begin(&vi); vii.t->cmp(&vii, &viend); vii = vii.t->inc(&vii))
        printf("%d ", *vii.t->get(&vii));

    vd.t->push_back(&vd, &b);
    size = vd.t->size(&vd);
    for (i = 0; i < size; ++i)
        printf("%f ", vd.begin[i]);
}
