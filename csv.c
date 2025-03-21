/* comma separated value text file handling */

#define NOMINMAX
#define _CRT_SECURE_NO_WARNINGS 1
#pragma warning(disable : 4018) // unsigned/signed mismatch
#include "csv.h"
#include "jmax.h"
#include "jmem.h"
#include "jvarint.h"
#include "jvec.h"
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
  jbool start_of_line = true;

  encode.size = 64;

  if (err = JVEC_APPEND(&index_file_path, file_path, strlen(file_path)))
    goto exit;
  if (err = JVEC_APPEND(&index_file_path, ".index", sizeof(".index")))
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
        in_quotes = false;
        goto increment_field_size;
      }
    }

    if (ch == '\n') {
      /* Write how many fields line has. */
      size = jvarint_encode(fields.size, &encode);
      if (encode.bytes_required !=
          fwrite(encode.buffer, 1, encode.bytes_required, file_w))
        goto error;
      /* Write size of each field. */
      for (i = 0; i < fields.size; ++i) {
        size = jvarint_encode(fields.data[i].size, &encode);
        if (encode.bytes_required !=
            fwrite(encode.buffer, 1, encode.bytes_required, file_w))
          goto error;
      }
      fields.size = 0;
    } else if (ch == ',') {
    end_of_field:
      if (err = JVEC_PUSH_BACK(&fields, &field))
        goto exit;
      field.size = 0;
    } else {
    increment_field_size:
      ++field.size;
    }
  }

  fclose(file_r);
  fclose(file_w);
}

#pragma warning(disable : 4100) /* unused parameter */

int main(int argc, char **argv) {
  if (strcmp(argv[1], "index") == 0) {
    csv_index_file(argv[2]);
  }
}
