/* comma separated value text file handling */
/* TODO:
   There are mistakes here, it will be rewritten.
   And port to C.
*/

#define NOMINMAX
#define _CRT_SECURE_NO_WARNINGS 1
#pragma warning(disable : 4018) // unsigned/signed mismatch
#include "csv.h"
#include "jbool.h"
#include "jmax.h"
#include "read_entire_file.h"
#include <stdio.h>
#include <string.h>
#include <thread>

#if !_MSC_VER && !defined(__cdecl)
#define __cdecl
#endif

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

void csv_indexing_line_t::work() {
  int64_t field_size{};
  int64_t field_offset{};
  for (int64_t i = 0; i < line_size; ++i) {
    if (indexer->contents[i + line_offset] == ',') {

      csv_indexing_field_t indexing_field = {field_offset, field_size};

      JVEC_PUSH_BACK(&this->fields, &indexing_field);

      max_field_offset = JMAX(max_field_offset, field_offset);
      max_field_size = JMAX(max_field_size, field_size);
      field_offset = i + 1;
      field_size = 0;
    } else {
      ++field_size;
    }
  }

  JVEC_PUSH_BACK(&indexer->lines, this);
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

void csv_index_file(csv_indexer_t *self, char *file_path) {

  int64_t file_position = {0};
  csv_persistant_index_t index_header = {0};
  index_header.version[0] = 1;

  JVEC(char) index_file_path = {0};

  JVEC_APPEND(&index_file_path, file_path, strlen(file_path));
  JVEC_APPEND(&index_file_path, ".index", sizeof(".index"));

  read_entire_file_t file = {0};
  file.file_path = file_path;
  file.malloc = &jmalloc_default;
  file.stdio = &jstdio_default;
  read_entire_file(&file);
  self->contents = file.contents;

  /* Split each line rapidly and hand off line parsing to line workers. */
  int64_t line_start = 0;
  int64_t line_size = 0;

  for (file_position = 0; file_position < file.size; ++file_position) {
    char ch = file.contents[file_position];
    if (ch == '\r' || ch == '\n') {
      file_position += (ch == '\r' && (file_position + 1) < file.size &&
                        file.contents[file_position + 1] == '\n');

      index_header.max_line_contents_size =
          JMAX(index_header.max_line_contents_size, line_size);

      index_header.max_line_contents_offset =
          JMAX(index_header.max_line_contents_offset, file_position);

      csv_indexing_line_t indexing_line(self, line_start, line_size);

      indexing_line.work();

      line_start = file_position + 1;
      line_size = 0;
      continue;
    }
    ++line_size;
  }

  qsort(self->lines.data, self->lines.size, sizeof(csv_indexing_line_t),
        csv_indexing_line_compare_v);

  csv_indexing_line_t *maxelem = (csv_indexing_line_t *)max_element(
      self->lines.data, self->lines.size, sizeof(*maxelem),
      line_less_by_field_size);

  index_header.max_field_count = maxelem->fields.size;

  maxelem = (csv_indexing_line_t *)max_element(
      self->lines.data, self->lines.size, sizeof(*maxelem),
      line_less_by_max_field_size);
  index_header.max_field_size = maxelem->max_field_size;

  maxelem = (csv_indexing_line_t *)max_element(
      self->lines.data, self->lines.size, sizeof(*maxelem),
      line_less_by_max_field_offset);
  index_header.max_field_offset = maxelem->max_field_offset;

  JVEC(char) index_contents = {0};
  JVEC_RESIZE(&index_contents, sizeof(index_header));

  index_header.path_size = strlen(file_path);
  index_header.offset_to_path = index_contents.size;
  JVEC_INSERT(&index_contents, JVEC_END(&index_contents), file_path,
              strlen(file_path) + 1);

  auto put_int = [&](int64_t a, int size) {
    int64_t offset = index_contents.size;
    JVEC_RESIZE(&index_contents, offset + 8);
    *reinterpret_cast<int64_t *>(&index_contents.data[offset]) = a;
    JVEC_RESIZE(&index_contents, offset + size);
  };

  JVEC_RESIZE(&index_contents, round_up(index_contents.size, 8));
  index_header.offset_to_lines = index_contents.size;

  size_t line_iter;

  for (line_iter = 0; line_iter < self->lines.size; ++line_iter) {
    csv_indexing_line_t *line = &self->lines.data[line_iter];
    JVEC_RESIZE(&index_contents, round_up(index_contents.size, 8));

    int8_t const field_count_size = bytes_for_value(line->fields.size);
    int8_t const field_offset_size = bytes_for_value(line->max_field_offset);
    int8_t const field_size_size = bytes_for_value(line->max_field_size);

    put_int(field_count_size, 1);
    put_int(field_offset_size, 1);
    put_int(field_size_size, 1);

    put_int(line->fields.size, field_count_size);

    size_t field_iter = {0};

    for (field_iter = 0; field_iter < line->fields.size; ++field_iter) {
      csv_indexing_field_t *field = &line->fields.data[field_iter];
      field->offset;
      field->size;
    }
    /*
        for (auto &field : line.fields)
          put_int(field.offset, field_offset_size);

        for (auto &field : line.fields)
          put_int(field.size, field_size_size);
    */
  }
  index_header.total_size = index_contents.size;

  *(csv_persistant_index_t *)(&index_contents.data[0]) = index_header;

  FILE *file_w = fopen(index_file_path.data, "wb");
  fwrite(&index_contents.data[0], 1, index_contents.size, file_w);
  fclose(file_w);
}

int main(int /*argc*/, char **argv) {
  if (strcmp(argv[1], "index") == 0) {
    csv_indexer_t indexer = {0};
    csv_index_file(&indexer, argv[2]);
  }
}
