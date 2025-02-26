
#ifndef jstatic_assert_h
#define jstatic_assert_h 1

#define JSTATIC_ASSERT(x) typedef char jstatic_assert[(x)?1:-1]

#endif
