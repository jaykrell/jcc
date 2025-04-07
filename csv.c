/* comma separated value text file handling */

#define NOMINMAX 1
#define _CRT_SECURE_NO_WARNINGS 1
#if _MSC_VER
#pragma warning(disable : 4018) // unsigned/signed mismatch
#endif
#include "csv.h"
#include "jmax.h"
#include "jmem.h"
#include "jmisc.h"
#include "jvarint.h"
#include "jvec.h"
#include "jbool.h"
#include <stdio.h>
#include <string.h>

#if !_MSC_VER && !defined(__cdecl)
#define __cdecl
#endif

typedef JVEC(char) jvec_char_t;

int csv_index_file(char *file_path) {

  /* temporary in memory form; file form is varint64 */
  typedef struct field_t {
    int64_t size;
  } field_t;

  typedef JVEC(field_t) jvec_field_t;

  /* temporary in memory form; file form is varint64s */
  typedef struct line_t {
    jvec_field_t fields;
  } line_t;

  field_t field = {0};
  line_t line = {0};
  int i = {0};
  jvec_char_t index_file_path = {0};
  FILE *file_r = 0;
  FILE *file_w = 0;
  jvarint_encode_t encode = {0};
  int err = 0;
  jbool in_quotes = jfalse;

  encode.size = 64;

  if ((err = JVEC_APPEND(&index_file_path, file_path, strlen(file_path))))
    goto exit;
  if ((err = JVEC_APPEND(&index_file_path, ".index", sizeof(".index"))))
    goto exit;

  if (!(file_r = fopen(file_path, "rb")))
    goto exit;
  if (!(file_w = fopen(index_file_path.data, "wb")))
    goto exit;

  while (1) {
    char ch = fgetc(file_r);
    /* TODO: Should only consume \r if next character is \n. */
    if (ch == '\r') {
      ch = fgetc(file_r);
    }
    if (ch == EOF)
      break;

    if (ch == '"') {
      ch = fgetc(file_r);
      if (in_quotes && ch == '"') {
        in_quotes = jfalse;
        goto increment_field_size;
      }
    }

    if (ch == '\n') {
      /* Write how many fields line has. */
      jvarint_encode_unsigned(line.fields.size, &encode);
      if (encode.bytes_required !=
          fwrite(encode.buffer, 1, encode.bytes_required, file_w))
        goto error;
      /* Write size of each field. */
      for (i = 0; i < line.fields.size; ++i) {
        jvarint_encode_unsigned(line.fields.data[i].size, &encode);
        if (encode.bytes_required !=
            fwrite(encode.buffer, 1, encode.bytes_required, file_w))
          goto error;
      }
      line.fields.size = 0;
    } else if (ch == ',') {
      if ((err = JVEC_PUSH_BACK(&line.fields, &field)))
        goto exit;
      field.size = 0;
    } else {
    increment_field_size:
      ++field.size;
    }
  }

error:
exit:
  if (file_r)
    fclose(file_r);
  if (file_w)
    fclose(file_w);
  return err;
}

#if _MSC_VER
#pragma warning(disable : 4100) /* unused parameter */
#endif

int main(int argc, char **argv) {
  if (strcmp(argv[1], "index") == 0) {
    csv_index_file(argv[2]);
  }
  return 0;
}
