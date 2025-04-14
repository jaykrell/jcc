/* comma separated value text file handling */

#define NOMINMAX 1
#define _CRT_SECURE_NO_WARNINGS 1
#if _MSC_VER
#pragma warning(disable : 4018) /* unsigned/signed mismatch */
#endif
#include "csv.h"
#include "jbool.h"
#include "jmax.h"
#include "jmem.h"
#include "jmisc.h"
#include "jsprintf.h"
#include "jvarint.h"
#include "jvec.h"
#include <stdio.h>
#include <string.h>

#if !_MSC_VER && !defined(__cdecl)
#define __cdecl
#endif

typedef JVEC(char) jvec_char_t;

/* temporary in memory form; file form is varint64 */
typedef JVEC(int64_t) jvec_field_t;

/* temporary in memory form; file form is varint64s */
typedef struct line_t {
  jvec_field_t fields;
} line_t;

typedef struct csv_index_file_t {
  FILE *file_r;
  FILE *file_w;
  line_t line;
  jvec_char_t index_file_path;
  char *file_path;
  int64_t field_size;
  int64_t total;
  int64_t debug_line;
  jbool done;
  jbool debug;
} csv_index_file_t;

static int get_char(csv_index_file_t *self)
/* Get a character. This handles \r\n sequences returning them as \n.
   And \r is returned as \n.
   field size is tracked here. */
{
  int ch = fgetc(self->file_r);
  self->field_size += (ch != EOF);
  if (ch != '\r')
    return ch;
  ch = fgetc(self->file_r);
  if (ch != EOF && ch != '\n') {
    ungetc(ch, self->file_r);
  } else {
    self->field_size += 1;
  }
  return '\n';
}

/* TODO: rename "self" to "csv" or "csv_index" or "indexer" */
static int csv_index_file_write_line(csv_index_file_t *self)
/* Write a line, as a count of fields and field lengths.
TODO: What is a field length really, given quoting? */
{
  jvarint_encode_t encode = {0};
  int err = 0;
  int64_t i = 0;
  encode.size = 64;

  if (self->debug)
    printf("csv: line %ld has %ld fields\n", (long)++(self->debug_line),
           (long)self->line.fields.size);

  /* Write how many fields line has. */
  jvarint_encode_unsigned(self->line.fields.size, &encode);
  if (encode.encoded_size !=
      fwrite(encode.buffer, 1, encode.encoded_size, self->file_w))
    goto exit;
  ++(self->total);
  /* Write size of each field. */
  for (i = 0; i < self->line.fields.size; ++i) {
    jvarint_encode_unsigned(self->line.fields.data[i], &encode);
    if (encode.encoded_size !=
        fwrite(encode.buffer, 1, encode.encoded_size, self->file_w))
      goto exit;
    ++(self->total);
  }

exit:
  return err;
}

static int csv_index_file_read_line(csv_index_file_t *self)
/* Read a line, recording field lengths and number of fields.
This handles quoting. For ease of later skipping fields,
commas and quotes do contribute to field size. */
{
  int any = 0;
  int err = 0;

  /* Initially we are not within quotes. */
  jbool quoted = jfalse;

  /* An initially size=0 field. */
  self->field_size = 0;

  /* Assume nothing is read and reading is all done. */
  self->done = jtrue;

  /* Reset number of fields in line. */
  self->line.fields.size = 0;

  while (1) {
    int ch = get_char(self);

    /* End file means end of reading this line, and the file, with possibly
     * still this line to write. */
    if (ch == EOF)
      goto handle_end_of_field;

    /* There is at least an empty line, so processing should continue. */
    self->done = jfalse;

    if (ch == '\n')
      goto handle_end_of_field;

    any = 1; /* completely empty lines have zero fields */

    /* Fields can be quoted. Quotes are at the start of field. */
    if (ch == '"' && self->field_size == 0) {
      quoted = jtrue;
      continue;
    } else if (quoted && ch != '"') {
      continue;
    } else if (quoted && ch == '"') {
      /* When quoting, quote means end of field or a quoted quote. */
      ch = get_char(self);
      if (ch == '"') {
        quoted = jtrue;
        goto handle_end_of_field;
      }
      /* Ending quote terminates field. */
      if (ch != EOF && ch != '\n' && ch != ',') {
        fprintf(stderr, "ERROR: non-terminated CVS quote is %s.\n",
                self->file_path);
      }
    }

    switch (ch) {
    case '\n':
    case EOF:
    case ',':
    handle_end_of_field:
      if (any && (err = JVEC_PUSH_BACK(&self->line.fields, &self->field_size)))
        return err;
      self->field_size = 0;
      if (ch != ',')
        return 0;
      break;
    }
  }
}

void csv_index_cleanup(csv_index_file_t *self) {
  JVEC_CLEANUP(&self->index_file_path);
  if (self->file_r)
    fclose(self->file_r);
  if (self->file_w)
    fclose(self->file_w);
  self->file_w = self->file_r = 0;
}

int csv_index_file(csv_index_file_t *self, char *file_path) {
  int err = 0;

  self->file_path = file_path;
  if ((err = JVEC_APPEND(&self->index_file_path, file_path, strlen(file_path))))
    goto exit;
  if ((err = JVEC_APPEND(&self->index_file_path, ".index", sizeof(".index"))))
    goto exit;

  if (!(self->file_r = fopen(file_path, "rb")))
    goto exit;
  if (!(self->file_w = fopen(self->index_file_path.data, "wb")))
    goto exit;

  while (!self->done) {
    if ((err = csv_index_file_read_line(self)))
      goto exit;
    if (self->done || ((err = csv_index_file_write_line(self))))
      goto exit;
  }

exit:
  return err;
}

#if _MSC_VER
#pragma warning(disable : 4100) /* unused parameter */
#endif

int main(int argc, char **argv) {
  csv_index_file_t xself = {0};
  csv_index_file_t *self = &xself;
  char i64buf[256] = {0};

  if (strcmp(argv[1], "debug") == 0) {
    self->debug = 1;
    ++argv;
  }
  if (strcmp(argv[1], "index") == 0) {
    csv_index_file(self, argv[2]);
    j_uint64_to_hex_shortest(self->total, i64buf);
    printf("total: %s\n", i64buf);
  }

  csv_index_cleanup(self);

  return 0;
}
