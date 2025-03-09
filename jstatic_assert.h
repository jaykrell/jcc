#ifndef JSTATIC_ASSERT_H
#define JSTATIC_ASSERT_H 1

#define JSTATIC_ASSERT(x) typedef char jstatic_assert[(x)?1:-1]

#endif
