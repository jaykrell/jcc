/* jsprintf.c */

/* How do I printf a size_t? */

#include <stdint.h>
#include <stddef.h>
#include "jpstr.h"
#include "juint.h"

#if __cplusplus
extern "C" {
#endif

void j_append_char (char c, PSTR* p, PSTR limit);

void j_revstr (PSTR a, size_t len);

/* 16 characters always */
void j_uint64_to_hex_full (uint64_t a, PSTR buf);

/* trim leading zeros */
void j_uint64_to_hex_shortest (uint64_t a, PSTR buf);

/* trim leading zeros */
void j_uint64_to_dec_shortest (uint64_t a, PSTR buf);

/* trim leading zeros, but positive must start 0 or 7 */
void j_int64_to_hex_shortest (int64_t a, PSTR buf);

void j_fill_hex_value (int64_t value, PSTR* p, PSTR limit);

#if __cplusplus
} /* extern "C" */
#endif
