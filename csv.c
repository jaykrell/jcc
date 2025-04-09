/* comma separated value text file handling */

#define NOMINMAX 1
#define _CRT_SECURE_NO_WARNINGS 1
#if _MSC_VER
#pragma warning(disable : 4018) // unsigned/signed mismatch
#endif
#include "csv.h"
#include "jbool.h"
#include "jmax.h"
#include "jmem.h"
#include "jmisc.h"
#include "jvarint.h"
#include "jvec.h"
#include <stdio.h>
#include <string.h>

#if !_MSC_VER && !defined(__cdecl)
#define __cdecl
#endif

typedef JVEC(char) jvec_char_t;

/* temporary in memory form; file form is varint64 */
typedef struct field_t {
  int64_t size;
} field_t;

typedef JVEC(field_t) jvec_field_t;

/* temporary in memory form; file form is varint64s */
typedef struct line_t {
  jvec_field_t fields;
} line_t;

typedef struct csv_index_file_t {
  int ch;
  FILE *file_r;
  FILE *file_w;
  line_t line;
  field_t field;
  jbool start_of_field;
  jbool done;
} csv_index_file_t;

static int get_char(csv_index_file_t *self) {
  int ch = fgetc(self->file_r);
  if (ch != '\r')
    return self->ch = ch;
  ch = fgetc(self->file_r);
  if (ch == EOF)
    return self->ch = '\r';
  if (ch != '\n') {
    ungetc(ch, self->file_r);
    return self->ch = '\r';
  }
  return self->ch = '\n';
}

static int handle_of_field(csv_index_file_t *self) {
  int err;
  if ((err = JVEC_PUSH_BACK(&self->line.fields, &self->field)))
    return err;
  self->start_of_field = jtrue;
  self->field.size = 0;
  return 0;
}

static int csv_index_file_write_line(csv_index_file_t *self) {
  jvarint_encode_t encode = {0};
  int err = 0;
  encode.size = 64;
  int64_t i;

  /* Write how many fields line has. */
  jvarint_encode_unsigned(self->line.fields.size, &encode);
  if (encode.bytes_required !=
      fwrite(encode.buffer, 1, encode.bytes_required, self->file_w))
    goto exit;
  /* Write size of each field. */
  for (i = 0; i < self->line.fields.size; ++i) {
    jvarint_encode_unsigned(self->line.fields.data[i].size, &encode);
    if (encode.bytes_required !=
        fwrite(encode.buffer, 1, encode.bytes_required, self->file_w))
      goto exit;
  }

exit:
  return err;
}

static int csv_index_file_read_line(csv_index_file_t *self) {
  int err = 0;
  jbool quoted = jfalse;
  self->start_of_field = jtrue;
  self->done = jtrue;
  self->line.fields.size = 0;

  while (1) {
    int ch = get_char(self);
    if (ch == EOF)
      return 0; /* not an error but this function is done */
    self->done = jfalse;
    if (ch == '\n')
      return 0;

    if (self->start_of_field) {
      self->start_of_field = jfalse;
      if (ch == '"') {
        quoted = jtrue;
        continue;
      }
    } else {
      if (quoted) {
        if (ch == '"') { /* end of field or quoted quote */
          ch = get_char(self);
          if (ch != '"' && ch != EOF && ch != '\n' != ch != ',') {
            /* error */
          }
        }
      }
    }

    switch (ch) {
    case '\n':
    case EOF:
    case ',':
      if ((err = handle_of_field(self)) || ch != ',')
        return err;
      break;
    default:
      self->field.size += 1;
    }
  }
}

int csv_index_file(char *file_path) {
  csv_index_file_t xself = {0};
  csv_index_file_t *self = &xself;
  jvec_char_t index_file_path = {0};
  int err = 0;

  if ((err = JVEC_APPEND(&index_file_path, file_path, strlen(file_path))))
    goto exit;
  if ((err = JVEC_APPEND(&index_file_path, ".index", sizeof(".index"))))
    goto exit;

  if (!(self->file_r = fopen(file_path, "rb")))
    goto exit;
  if (!(self->file_w = fopen(index_file_path.data, "wb")))
    goto exit;

  while (!self->done) {
    if (err = csv_index_file_read_line(self))
      goto exit;
    if (self->done)
      break;
    if (err = csv_index_file_write_line(self))
      goto exit;
  }

exit:
  if (self->file_r)
    fclose(self->file_r);
  if (self->file_w)
    fclose(self->file_w);
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
