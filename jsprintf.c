/* jsprintf.c */

/* How do I printf a size_t, int64_t, etc?
 * Use these functions to double buffer to a char[] and then printf %s.
 */

#include "jswap.h"
#include "juint.h"
#include <assert.h>
#include <limits.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

/* useful?
void (*fatal_error)(const char *);

void j_append_char(char c, char* *p, char* limit) {
  char* q = *p;
  if (q >= limit)
    fatal_error("buffer overflow\n");
  *q = c;
  *p = (q + 1);voidvoid
}
*/

char *j_revstr(char *a, size_t len) {
  size_t i = 0;
  if (len < 2)
    return a;
  len -= 1;
  i = 0;
  while (i < len) {
    JSWAP(char, a[i], a[len]);
    i += 1;
    len -= 1;
  }
  return a;
}

char *jstring_terminate_and_reverse(char *a, size_t len) {
  a[len] = 0;
  return j_revstr(a, len);
}

char *j_uint64_to_hex_full(uint64_t a, char *buf)
/* 16 characters always
 * TODO: Provider _upper and _lower forms.
 */
{
  juint i = 0;
  for (i = 0; i < sizeof(a) * CHAR_BIT / 4; a >>= 4)
    buf[i++] = "0123456789ABCDEF"[a & 0xF];
  return jstring_terminate_and_reverse(buf, i);
}

char *j_uint64_to_hex_shortest(uint64_t a, char *buf)
/* Trim leading zeros.
 * TODO: Provider _upper and _lower forms.
 * CONSIDER: return length instead of buffer.
 * Really need to get away from nul terminated strings!
 */
{
  juint i = 0;
  do /* do/while necessary to handle 0 */
    buf[i++] = "0123456789ABCDEF"[a & 0xF];
  while (a >>= 4);
  return jstring_terminate_and_reverse(buf, i);
}

char *j_uint64_to_dec_shortest(uint64_t a, char *buf)
/* Trim leading zeros.
 * CONSIDER: return length instead of buffer.
 * Really need to get away from nul terminated strings!
 */
{
  juint i = 0;
  do /* do/while necessary to handle 0 */
    buf[i++] = "0123456789"[a % 10];
  while (a /= 10);
  return jstring_terminate_and_reverse(buf, i);
}

char *j_int64_to_hex_shortest(int64_t a, char *buf)
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
  int neg = (a < 0);
  char trim = (neg ? 'F' : '0');
  for (i = 0; i < sizeof(a) * CHAR_BIT / 4; a >>= 4)
    buf[i++] = "0123456789ABCDEF"[a & 0xF];
  while (i >= 2 && (buf[i - 1] == trim) && (neg == (buf[i - 2] > '7')))
    i -= 1;
  return jstring_terminate_and_reverse(buf, i);
}

/* TODO: What is this good for?
 * Well, it is for incremental concatenation, clearly.
void j_fill_hex_value(int64_t value, char**p, char* limit) {
  j_append_char('0', p, limit);
  j_append_char('x', p, limit);
  j_int64_to_hex_shortest(value, *p);
  *p += strlen(*p);
  assert(*p < limit);
}
*/
