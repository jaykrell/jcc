/* jsprintf.c */

/* How do I printf a size_t?
 * This is difficult portably.
 * instead: j_uint64_to_hex_shortest etc. to a separate
 * buffer and then sprintf %s
 */

#include "jpstr.h"
#include "juint.h"
#include <stddef.h>
#include <stdint.h>

#if __cplusplus
extern "C" {
#endif

/*useful?
void j_append_char(char c, char **p, char *limit);
*/

void j_revstr(char *a, size_t len);

/* 16 characters always */
void j_uint64_to_hex_full(uint64_t a, char *buf);

/* trim leading zeros */
void j_uint64_to_hex_shortest(uint64_t a, char *buf);

/* trim leading zeros */
void j_uint64_to_dec_shortest(uint64_t a, char *buf);
void j_int64_to_dec_shortest(int64_t a, char *buf);

/* trim leading zeros, but positive must start 0 or 7 */
void j_int64_to_hex_shortest(int64_t a, PSTR buf);

/*TODO: Useful?
 * void j_fill_hex_value(int64_t value, PSTR *p, PSTR limit);
 */

#if __cplusplus
} /* extern "C" */
#endif
