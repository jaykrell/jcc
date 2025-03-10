
#ifndef JINT_H
#define JINT_H 1

#include "jlong.h"

/* todo
figure out what we really need

julong is useful for a hashcode, however large it is
*/

/*#include <limits.h>*/

/*jtatic_assert(CHAR_BIT == 8);*/
/*jstatic_assert(sizeof(short) == 2);*/
/*jstatic_assert(sizeof(int) == 4);*/

/*typedef signed char jint8;*/
/*typedef short       jint16;*/
/*typedef int         jint32;*/

/*typedef unsigned char  juint8;*/
/*typedef unsigned short juint16;*/

/* todo depend there being an int32? probe via limits.h? */
/*typedef unsigned int   juint32;*/

typedef long jlong;
typedef unsigned long julong;

/*todo: depend on there being an int64? */

#endif
