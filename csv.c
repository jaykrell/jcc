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
  uint8_t varint[1 + 64 / 7] = {0};

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
    if (ch == '\n') {
      size = encode_varint64(fields.size, buf);
      if (size != fwrite(varint, 1, size, file_w))
        goto error;
      for (i = 0; i < fields.size; ++i) {
        size = encode_varint64(fields.data[i].size, buf);
        if (size != fwrite(varint, 1, size, file_w))
          goto error;
      }
      fields.size = 0;
    } else if (ch == ',') {
      if (err = JVEC_PUSH_BACK(&fields, &field))
        goto exit;
      field.size = 0;
    } else {
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
