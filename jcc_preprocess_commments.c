#include "jcc.h"
#include <assert.h>

typedef struct jcc_t jcc_t;
int jcc_phase2_getchar(jcc_t *jcc, int *ch);
int jcc_getchar(jcc_t *jcc, int *ch);
void jcc_phase2_unget(jcc_t *jcc, int ch);
int jcc_getchar(jcc_t *jcc, int *pch);

int jcc_getchar(jcc_t *jcc, int *pch)
/* C preprocessor scanning.
 * Read, at the phase that handles comments, turning them into spaces.
 *
 * Read a character. If it is not '/', return it.
 * If it is '/', read ahead another. If that is not '*' push it back and return
 * '/'. If it is '*', then read until closing '*' and '/', which is similar to
 * opening. Matters such as line continuation with backward slashes are handled
 * at a different layer.
 * TODO: C99/C++ comments (optional, subject to command line switches)
 */
{
  int err = 0;
  int ch = 0;
  size_t size;

  size = jcc->queued_chars.size;
  if (size) {
    jcc->queued_chars.size = --size;
    *pch = jcc->queued_chars.data[size];
    return 0;
  }

  /* If next character is definitely not opening a comment, return it. */
  err = jcc_phase2_getchar(jcc, pch);
  if (err || *pch != '/')
    return err;
  /* If the second character does not complete the comment start, push it back
   * and return the first character. */
  err = jcc_phase2_getchar(jcc, pch);
  if (err)
    return err;
  ch = *pch;
  if (ch != '*') {
    jcc_phase2_unget(jcc, ch);
    *pch = '/';
    return 0;
  }
  while (1) {
    /* Read until end of comment, i.e. star slash, yielding a space. */
    err = jcc_phase2_getchar(jcc, pch);
    if (err)
      return err;
    if (*pch != '*')
      continue;
  jcc_getchar_star:
    /* After star, check for slash. */
    err = jcc_phase2_getchar(jcc, pch);
    if (err)
      return err;
    /* If star slash, return space. */
    ch = *pch;
    if (ch == '/') {
      ch = ' ';
      err = JDEC_PUSH_FRONT(&jcc->queued_chars, &ch);
      if (err)
        return err;
      *pch = ch;
      return 0;
    }
    /* Otherwise it was star not-slash, continue. */
    if (ch == '*')
      goto jcc_getchar_star;
  }
}
