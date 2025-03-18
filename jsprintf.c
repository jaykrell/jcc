/* jsprintf.c */

/* How do I printf a size_t? */

#include "juint.h"
#include <limits.h>
#include <stdint.h>

typedef char *PSTR;
typedef unsigned juint;

void (*fatal_error)(const char *);

void j_append_char(char c, PSTR *p, PSTR limit) {
  PSTR q = *p;
  if (q >= limit)
    fatal_error("buffer overflow\n");
  *q = c;
  *p = (q + 1);
}

void j_revstr(PSTR a, size_t len) {
  if (len < 2)
    return;
  len -= 1;
  size_t i = {0};
  while (i < len) {
    char temp = a[i];
    a[i] = a[len];
    a[len] = temp;
    i += 1;
    len -= 1;
  }
}

static void j_uint64_to_hex_full(uint64_t a, PSTR buf)
/* 16 characters always */
{
  juint i = 0;
  for (i = 0; i < sizeof(a) * CHAR_BIT / 4; a >>= 4)
    buf[i++] = "0123456789ABCDEF"[a & 0xF];
  j_revstr(buf, i);
  buf[i] = 0;
}

void j_uint64_to_hex_shortest(uint64_t a, PSTR buf)
/* Trim leading zeros. */
{
  juint i = 0;
  do /* do/while necessary to handle 0 */
    buf[i++] = "0123456789ABCDEF"[a & 0xF];
  while (a >>= 4);
  j_revstr(buf, i);
  buf[i] = 0;
}

void j_uint64_to_dec_shortest(uint64_t a, PSTR buf)
/* Trim leading zeros. */
{
  juint i = 0;
  do /* do/while necessary to handle 0 */
    buf[i++] = "0123456789"[a % 10];
  while (a /= 10);
  j_revstr(buf, i);
  buf[i] = 0;
}

void j_int64_to_hex_shortest(int64_t a, PSTR buf)
/* if negative, first character must be >=8
 * if positive, first character must < 8;
 * skip leading characters otherwise
 * insert leading 0 or F if necessary
 * e.g. 127 => 7F
 *      255 => 0FF
 *     -255 => F01
 *        7 => 7
 *        8 => 08
 *       15 => 0F
 *     -128 => 80
 *
 * algorithm: do "full" and then trim digits
 * Positive numbers can have 0 trimmed as long as next is <= 7.
 * Negative numbers can have F trimmed as long as next is > 7.
 * Result must be at least one character.
 */
{
  juint i = 0;
  juint neg = (a < 0);
  char trim = (neg ? 'F' : '0');
  for (i = 0; i < sizeof(a) * CHAR_BIT / 4; a >>= 4)
    buf[i++] = "0123456789ABCDEF"[a & 0xF];
  while (i >= 2 && (buf[i - 1] == trim) && (neg == (buf[i - 2] > '7')))
    i -= 1;
  j_revstr(buf, i);
  buf[i] = 0;
}

void j_fill_hex_value(int64_t value, PSTR *p, PSTR limit) {
  j_append_char('0', p, limit);
  j_append_char('x', p, limit);
  j_int64_to_hex_shortest(value, *p);
  *p += strlen(*p);
  assert(*p < limit);
}
