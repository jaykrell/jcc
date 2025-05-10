#include <assert.h>

typedef struct jcc_t jcc_t;
int jcc_phase2_getchar(jcc_t *jcc, int *ch);
int jcc_phase3_getchar(jcc_t *jcc, int *ch);
int jcc_getchar(jcc_t *jcc, int *ch);
void jcc_phase2_unget(jcc_t *jcc, int ch);

int jcc_getchar(jcc_t *jcc, int *ch) { return jcc_phase3_getchar(jcc, ch); }

int jcc_phase3_getchar(jcc_t *jcc, int *ch)
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

  /* If next character is definitely not opening a comment, return it. */
  if (((err = jcc_phase2_getchar(jcc, ch))) || *ch != '/')
    return err;
  /* If the second character does not complete the comment start, push it back
   * and return the first character. */
  if (((err = jcc_phase2_getchar(jcc, ch))) || *ch != '*') {
    jcc_phase2_unget(jcc, *ch);
    *ch = '/';
    return 0;
  }
  while (1) {
    /* Read until end of comment, yielding a space. */
    while (!((err = jcc_phase2_getchar(jcc, ch))) && *ch != '*')
      ; /* nothing */
    if (err)
      return err;
    assert(*ch == '*');
    if ((err = jcc_phase2_getchar(jcc, ch)))
      return err;
    if (*ch == '/') {
      *ch = ' ';
      return 0;
    }
    jcc_phase2_unget(jcc, *ch); /* put back in case it is star */
  }
}
