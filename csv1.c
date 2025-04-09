/* comma separated value text file handling */

#define NOMINMAX
#define _CRT_SECURE_NO_WARNINGS 1
#if _MSC_VER
#pragma warning(disable : 4018) /* unsigned/signed mismatch */
#endif
#include "csv1.h"
#include "jbool.h"
#include "jmax.h"
#include "jmem.h"
#include "jvec.h"
#include "read_entire_file.h"
#include <stdio.h>
#include <stdlib.h> /* TODO: replace qsort */
#include <string.h>

#if !_MSC_VER && !defined(__cdecl)
#define __cdecl
#endif

typedef JVEC(char) jvec_char_t;

int64_t round_up(int64_t a, int64_t b) {
  int64_t mod = (a % b);
  return (mod ? (a + b - mod) : a);
}

int8_t bytes_for_value(int64_t a) {
  /* Off by one on purpose, just out of a gut feeling. */
  if (a <= INT32_MIN || a >= INT32_MAX)
    return 8;
  if (a <= INT16_MIN || a >= INT16_MAX)
    return 4;
  if (a <= INT8_MIN || a >= INT8_MAX)
    return 2;
  return 1;
}

int8_t bits_for_value(int64_t a) { return bytes_for_value(a) * 8; }

void csv_indexing_line_work(csv_indexing_line_t *self) {
  int64_t field_size = 0;
  int64_t field_offset = 0;
  int64_t i = 0;
  for (i = 0; i < self->line_size; ++i) {
    if (self->indexer->contents[i + self->line_offset] == ',') {

      csv_indexing_field_t indexing_field = {0};
      indexing_field.offset = field_offset;
      indexing_field.size = field_size;

      JVEC_PUSH_BACK(&self->fields, &indexing_field);

      self->max_field_offset = JMAX(self->max_field_offset, field_offset);
      self->max_field_size = JMAX(self->max_field_size, field_size);
      field_offset = i + 1;
      field_size = 0;
    } else {
      ++field_size;
    }
  }

  JVEC_PUSH_BACK(&self->indexer->lines, self);
}

/* like std::max_element */
void *max_element(void *begin, size_t count, size_t size,
                  jbool (*less)(void *a, void *b)) {
  char *cbegin = (char *)begin;
  char *cend = cbegin + count * size;
  char *m = cbegin;
  char *iter = cbegin;

  for (iter = cbegin; iter != cend; iter += size) {
    if (less(m, iter))
      m = iter;
  }

  return iter;
}

jbool line_less_by_field_size(void *va, void *vb)
/* Compare two lines by the number of fields they have. */
{
  csv_indexing_line_t *a = (csv_indexing_line_t *)va;
  csv_indexing_line_t *b = (csv_indexing_line_t *)vb;
  return a->fields.size < b->fields.size;
}

jbool line_less_by_max_field_size(void *va, void *vb)
/* Compare two lines by their maximum field size. */
{
  csv_indexing_line_t *a = (csv_indexing_line_t *)va;
  csv_indexing_line_t *b = (csv_indexing_line_t *)vb;
  return a->max_field_size < b->max_field_size;
}

jbool line_less_by_max_field_offset(void *va, void *vb)
/* Compare two lines by their maximum field offset.
For example max_field_size + max_field_offset is buffer size to hold a line,
though it is an exaggeration and could better stated.
*/
{
  csv_indexing_line_t *a = (csv_indexing_line_t *)va;
  csv_indexing_line_t *b = (csv_indexing_line_t *)vb;
  return a->max_field_offset < b->max_field_offset;
}

int csv_indexing_line_compare(csv_indexing_line_t *a, csv_indexing_line_t *b) {
  if (a->line_offset < b->line_offset)
    return -1;
  if (a->line_offset > b->line_offset)
    return 1;
  return 0;
}

int __cdecl csv_indexing_line_compare_v(void const *a, void const *b) {
  return csv_indexing_line_compare((csv_indexing_line_t *)a,
                                   (csv_indexing_line_t *)b);
}

static int csv_index_write_int(jvec_char_t *index_contents, int64_t a,
                               int size) {
  int err = 0;
  int64_t offset = 0;

  offset = index_contents->size;
  if ((err = JVEC_RESIZE(index_contents, offset + 8)))
    return err;
  *(int64_t *)(&index_contents->data[offset]) = a;
  return JVEC_RESIZE(index_contents, offset + size);
}

void csv_index_file(csv_indexer_t *self, char *file_path) {
  int8_t field_count_size = 0;
  int8_t field_offset_size = 0;
  int8_t field_size_size = 0;
  int64_t file_position = 0;
  jvec_char_t index_file_path;
  csv_persistant_index_t index_header = {0};
  FILE *file_w = 0;
  int64_t line_start = 0;
  int64_t line_size = 0;
  int64_t line_iter = 0;
  jvec_char_t index_contents = {0};
  csv_indexing_line_t *maxelem = 0;
  read_entire_file_t file = {0};

  JMEMSET0_VALUE(file_position);
  JMEMSET0_VALUE(index_file_path);
  JMEMSET0_VALUE(index_header);

  index_header.s.version[0] = 1;
  JVEC_APPEND(&index_file_path, file_path, strlen(file_path));
  JVEC_APPEND(&index_file_path, ".index", sizeof(".index"));

  file.file_path = file_path;
  file.malloc = &jmalloc_default;
  file.stdio = &jstdio_default;
  read_entire_file(&file);
  self->contents = file.contents;

  /* Split each line rapidly and hand off line parsing to line workers. */

  for (file_position = 0; file_position < file.size; ++file_position) {
    char ch = file.contents[file_position];
    if (ch == '\r' || ch == '\n') {
      csv_indexing_line_t indexing_line = {0};
      indexing_line.indexer = self;
      indexing_line.line_offset = line_start;
      indexing_line.line_size = line_size;

      file_position += (ch == '\r' && (file_position + 1) < file.size &&
                        file.contents[file_position + 1] == '\n');

      index_header.s.max_line_contents_size =
          JMAX(index_header.s.max_line_contents_size, line_size);

      index_header.s.max_line_contents_offset =
          JMAX(index_header.s.max_line_contents_offset, file_position);

      csv_indexing_line_work(&indexing_line);

      line_start = file_position + 1;
      line_size = 0;
      continue;
    }
    ++line_size;
  }

  qsort(self->lines.data, self->lines.size, sizeof(csv_indexing_line_t),
        csv_indexing_line_compare_v);

  maxelem = (csv_indexing_line_t *)max_element(
      self->lines.data, self->lines.size, sizeof(*maxelem),
      line_less_by_field_size);

  index_header.s.max_field_count = maxelem->fields.size;

  maxelem = (csv_indexing_line_t *)max_element(
      self->lines.data, self->lines.size, sizeof(*maxelem),
      line_less_by_max_field_size);
  index_header.s.max_field_size = maxelem->max_field_size;

  maxelem = (csv_indexing_line_t *)max_element(
      self->lines.data, self->lines.size, sizeof(*maxelem),
      line_less_by_max_field_offset);
  index_header.s.max_field_offset = maxelem->max_field_offset;

  JVEC_RESIZE(&index_contents, sizeof(index_header));

  index_header.s.path_size = strlen(file_path);
  index_header.s.offset_to_path = index_contents.size;
  JVEC_INSERT(&index_contents, JVEC_END(&index_contents), file_path,
              strlen(file_path) + 1);

  JVEC_RESIZE(&index_contents, round_up(index_contents.size, 8));
  index_header.s.offset_to_lines = index_contents.size;

  for (line_iter = 0; line_iter < self->lines.size; ++line_iter) {
    ptrdiff_t field_iter = {0};
    csv_indexing_line_t *line = &self->lines.data[line_iter];
    JVEC_RESIZE(&index_contents, round_up(index_contents.size, 8));

    field_count_size = bytes_for_value(line->fields.size);
    field_offset_size = bytes_for_value(line->max_field_offset);
    field_size_size = bytes_for_value(line->max_field_size);

    csv_index_write_int(&index_contents, field_count_size, 1);
    csv_index_write_int(&index_contents, field_offset_size, 1);
    csv_index_write_int(&index_contents, field_size_size, 1);

    csv_index_write_int(&index_contents, line->fields.size, field_count_size);

    for (field_iter = 0; field_iter < line->fields.size; ++field_iter) {
      csv_indexing_field_t *field = &line->fields.data[field_iter];
      csv_index_write_int(&index_contents, field->offset, field_offset_size);
    }

    for (field_iter = 0; field_iter < line->fields.size; ++field_iter) {
      csv_indexing_field_t *field = &line->fields.data[field_iter];
      csv_index_write_int(&index_contents, field->size, field_size_size);
    }
  }
  index_header.s.total_size = index_contents.size;

  *(csv_persistant_index_t *)(&index_contents.data[0]) = index_header;

  file_w = fopen(index_file_path.data, "wb");
  fwrite(&index_contents.data[0], 1, index_contents.size, file_w);
  fclose(file_w);
}

#if _MSC_VER
#pragma warning(disable : 4100) /* unused parameter */
#endif

int main(int argc, char **argv) {
  if (strcmp(argv[1], "index") == 0) {
    csv_indexer_t indexer = {0};
    csv_index_file(&indexer, argv[2]);
  }
  return 0;
}
