/* Jay miscellanous TODO: reorganize */

#if !JMISC_H
#define JMISC_H 1

#define J_COUNT(x) (sizeof(x) / sizeof((x)[0]))
#define J_AND_SIZE(x) (x), sizeof(x)
#define J_AND_COUNT(x) (x), J_COUNT(x)

#endif
