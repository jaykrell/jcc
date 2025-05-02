/* Jay C compiler. */

#include "jcc_unget.h"
#include <assert.h>

int jcc_unget_get(jcc_unget_t *unget, int *value)
/* Generic get from unget buffer (i.e. lookahead of length 1). */
{
  if (unget->valid) {
    unget->valid = 0;
    *value = unget->value;
    return 1;
  }
  return 0;
}

void jcc_unget_unget(jcc_unget_t *unget, int value)
/* Generic unget. */
{
  assert(!unget->valid);
  unget->valid = 1;
  unget->value = value;
}
