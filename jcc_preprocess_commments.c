#include <assert.h>

typedef struct jcc_t jcc_t;
int jcc_phase2_getchar(jcc_t *jcc, int *ch);
int jcc_phase3_getchar(jcc_t *jcc, int *ch);
int jcc_getchar(jcc_t *jcc, int *ch);
void jcc_phase2_unget(jcc_t *jcc, int ch);

int jcc_getchar(jcc_t *jcc, int *ch) { return jcc_phase3_getchar(jcc, ch); }

int jcc_phase3_getchar(jcc_t *jcc, int *pch)
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
  int err;
  int ch;

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
  jcc_phase3_getchar_star:
    /* After star, check for slash. */
    err = jcc_phase2_getchar(jcc, pch);
    if (err)
      return err;
    /* If star slash, return space. */
    ch = *pch;
    if (ch == '/') {
      *pch = ' ';
      return 0;
    }
    /* Otherwise it was star not-slash, continue. */
    if (ch == '*')
      goto jcc_phase3_getchar_star;
  }
}
