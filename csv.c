/* comma separated value text file handling */

#include "csv.h"
#include "jbool.h"
#include "jcommon.h"
#include "jfile.h"
#include "jmax.h"
#include "jmem.h"
#include "jmisc.h"
#include "jsprintf.h"
#include "jstdio_file.h"
#include "jvarint.h"
#include "jvec.h"

/* temporary in memory form; file form is varint64 */
typedef JVEC(int64_t) jvec_field_t;

/* temporary in memory form; file form is varint64s */
typedef struct line_t {
  jvec_field_t fields;
} line_t;

typedef struct csv_index_file_t {
  jfile_t *file_r;
  jfile_t *file_w;
  line_t line;
  jvec_char_t index_file_path;
  char *file_path;
  int64_t field_size;
  int64_t total;
  int64_t debug_line;
  int done;
  int debug;
  int unget_size;
  int unget_value[4];

  jstdio_file_t stdio_file_r;
  jstdio_file_t stdio_file_w;
  char buffer_r[4096];
  char buffer_w[4096];

} csv_index_file_t;

static int get_char(csv_index_file_t *self, char *ch)
/* Get a character. This handles \r\n sequences returning them as \n.
   And \r is returned as \n.
   field size is tracked here. */
{
  size_t actual = 0;
  int err = 0;
  *ch = 0;
  if ((err = jfile_read(self->file_r, ch, 1, &actual)) || !actual)
    return err;
  self->field_size += actual;
  if (*ch != '\r')
    return 0;

  if ((err = jfile_read(self->file_r, ch, 1, &actual)) || !actual)
    return err;
  if (actual && *ch != '\n') {
    jfile_unget(self->file_r, *ch);
  } else {
    self->field_size += actual;
  }
  return 0;
}

/* TODO: rename "self" to "csv" or "csv_index" or "indexer" */
static int csv_index_file_write_line(csv_index_file_t *self)
/* Write a line, as a count of fields and field lengths.
TODO: What is a field length really, given quoting? */
{
  size_t actual = 0;
  jvarint_encode_t encode = {0};
  int err = 0;
  int64_t i = 0;
  encode.size = 64;

  if (self->debug)
    printf("csv: line %ld has %ld fields\n", (long)++(self->debug_line),
           (long)self->line.fields.size);

  /* Write how many fields line has. */
  jvarint_encode_unsigned(self->line.fields.size, &encode);
  if ((err = jfile_write(self->file_w, encode.buffer, encode.encoded_size,
                         &actual)))
    goto exit;
  if (encode.encoded_size != actual)
    goto exit;
  ++(self->total);
  /* Write size of each field. */
  for (i = 0; i < self->line.fields.size; ++i) {
    jvarint_encode_unsigned(self->line.fields.data[i], &encode);
    actual = 0;
    if ((err = jfile_write(self->file_w, encode.buffer, encode.encoded_size,
                           &actual)))
      goto exit;
    if (encode.encoded_size != actual)
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
  jbool quoted = FALSE;

  /* An initially size=0 field. */
  self->field_size = 0;

  /* Assume nothing is read and reading is all done. */
  self->done = TRUE;

  /* Reset number of fields in line. */
  self->line.fields.size = 0;

  while (1) {
    char ch = 0;
    if ((err = get_char(self, &ch)))
      return err;

    /* End file means end of reading this line, and the file, with possibly
     * still this line to write. */
    if (self->file_r->eof)
      goto handle_end_of_field;

    /* There is at least an empty line, so processing should continue. */
    self->done = FALSE;
    any = 1; /* completely empty lines have zero fields */

    /* Fields can be quoted. Quotes are at the start of field. */
    if (ch == '"' && self->field_size == 0) {
      quoted = TRUE;
      continue;
    } else if (quoted && ch != '"') {
      /* Anything while quoted, except quote and EOF, means just keep reading
       * the field. */
      continue;
    } else if (quoted && ch == '"') {
      /* When quoting, quote means end of field or a quoted quote. */
      ch = 0;
      if ((err = get_char(self, &ch)))
        return err;
      if (self->file_r->eof)
        goto handle_end_of_field;
      if (ch == '"') {
        /* TODO: The next character must be a comma or newline or EOF.
         * Verify this when we have more than one character of unget. */
        goto handle_end_of_field;
      }
      /* Ending quote terminates field. */
      if (ch != '\n' && ch != ',') {
        fprintf(stderr, "ERROR: non-terminated CVS quote in %s.\n",
                self->file_path);
      }
    }

    if (self->file_r->eof)
      goto handle_end_of_field;

    switch (ch) {
    case '\n':
    case ',':
    handle_end_of_field:
      quoted = 0;
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
  jfile_close(self->file_r);
  jfile_close(self->file_w);
  self->file_w = self->file_r = 0;
  free(self);
}

int csv_index_file(csv_index_file_t *self, char *file_path) {
  int err = 0;
  while (!self->done) {
    if ((err = csv_index_file_read_line(self)))
      goto exit;
    if (self->done || ((err = csv_index_file_write_line(self))))
      goto exit;
  }
exit:
  return err;
}

int csv_index_file_open(csv_index_file_t *self, char *file_path) {
  int err = 0;
  self->file_path = file_path;
  if ((err = JVEC_APPEND(&self->index_file_path, file_path, strlen(file_path))))
    goto exit;
  if ((err = JVEC_APPEND(&self->index_file_path, ".index", sizeof(".index"))))
    goto exit;
  if ((err = jstdio_file_open(&self->stdio_file_r, file_path, "rb")))
    goto exit;
  if ((err = jstdio_file_open(&self->stdio_file_w, self->index_file_path.data,
                              "wb")))
    goto exit;
  /* Disable stdio buffering. We have our own. */
  setvbuf(self->stdio_file_r.file, 0, _IONBF, 0);
  setvbuf(self->stdio_file_w.file, 0, _IONBF, 0);
  self->file_r = &self->stdio_file_r.base;
  self->file_w = &self->stdio_file_w.base;

  self->file_r->buffer.capacity = sizeof(self->buffer_r);
  self->file_r->buffer.data = self->buffer_r;
  self->file_r->buffer.capacity = sizeof(self->buffer_w);
  self->file_r->buffer.data = self->buffer_w;

exit:
  return err;
}

#if _MSC_VER
#pragma warning(disable : 4100) /* unused parameter */
#endif

int main(int argc, char **argv) {
  char i64buf[256] = {0};
  int err = 0;
  csv_index_file_t *self = (csv_index_file_t *)calloc(1, sizeof(*self));

  if (strcmp(argv[1], "debug") == 0) {
    self->debug = 1;
    ++argv;
  }
  if (strcmp(argv[1], "index") == 0) {
    if ((err = csv_index_file_open(self, argv[2])))
      goto exit;
    csv_index_file(self, argv[2]);
    j_uint64_to_hex_shortest(self->total, i64buf);
    printf("total: %s\n", i64buf);
  }

exit:
  csv_index_cleanup(self);
  return err;
}
