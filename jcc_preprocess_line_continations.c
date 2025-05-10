#include "jcc.h"
#include "jcc_unget.h"

void jcc_phase2_unget(jcc_t *jcc, int ch) {
  jcc_unget_unget(&jcc->phase2_unget, ch);
}

int jcc_phase2_getchar(jcc_t *jcc, int *ch)
/* Get next character, handling line continuations.
 * This is translation phase 2.
 *
 * Each instance of a backslash character (\) immediately followed by a new-line
 * character is deleted, splicing physical source lines to form logical source
 * lines. Only the last backslash on any physical source line shall be eligible
 * for being part of such a splice. A source file that is not empty shall end in
 * a new-line character, which shall not be immediately preceded by a backslash
 * character before any such splicing takes place
 */
{
  int err = 0;
  while (1) {
    if (jcc_unget_get(&jcc->phase2_unget, ch))
      return 0;
    if (((err = jcc_phase1_getchar(jcc, ch))) || *ch != '\\')
      return err;
    if (((err = jcc_phase1_getchar(jcc, ch))) || *ch != '\n') {
      jcc_phase1_unget(jcc, *ch);
      return '\\';
    }
  }
}
