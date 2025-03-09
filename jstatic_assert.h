#ifndef JSTATIC_ASSERT_H
#define JSTATIC_ASSERT_H 1
/* Based on windows.h.
 * Negative array sizes are illegal.
 * This works with most compilers, but not all.
 * TODO: Disable it for known bad compilers.
 */
#define JSTATIC_ASSERT(x) typedef char jstatic_assert[(x)?1:-1]

#endif
