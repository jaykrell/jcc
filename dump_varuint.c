/* dump_varuint.c */

#if _MSC_VER
#pragma warning(disable : 4100) /* unused parameter */
#endif
#include "jvarint.h"
#include <assert.h>
#include <limits.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <inttypes.h>
#include "jint64.h"
/*************************************************************************************************/
static int read_byte(void * v)
{
  FILE* f=(FILE*)v;
  int ch = fgetc(f);
  return (ch != EOF) ? ch : -1;
}

int main(int argc, char** argv) {
  FILE* file=0;
  unsigned count=0;
  jvarint_decode_t decode={0};
  if (!argv[1]) {
    fprintf(stderr, "ERROR: no file specified\n");
    return 1;
  }
  file = fopen(argv[1], "rb");
  if (!file) {
    int err = errno;
    fprintf(stderr, "ERROR: fopen(%s) failed %d (%s)\n", argv[0], err, strerror(err));
    return err;
  }
  decode.read_byte = read_byte;
  decode.read_byte_context = file;
  while (1) {
    uint64_t i = jvarint_decode_unsigned(&decode);
    if (decode.err) break;
    printf("%" JINT64_PRI "x ", i);
    if (++count > 3) printf("\n");
  }
  return 0;
}
