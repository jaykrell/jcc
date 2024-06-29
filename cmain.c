#include "jvec.h"

int main()
{
    jvec_double jd = {0};
    jvec_int ji = {0};

    jvec_int_init (&ji);
    jvec_double_init (&jd);

    int a = 1;
    ji.t->push_back(&ji, &a);

    double b = 1;
    jd.t->push_back(&jd, &b);
}

