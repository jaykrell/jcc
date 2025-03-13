/* comma separated value text file handling */
/* TODO:
   There are mistakes here, it will be rewritten.
   And port to C.
*/

#if _WIN32
#define NOMINMAX
#define _CRT_SECURE_NO_WARNINGS 1
#pragma warning(disable : 4018) // unsigned/signed mismatch
#include "csv.h"
#include "jmax.h"
#include "read_entire_file.h"
#include <algorithm>
#include <stdio.h>
#include <string.h>
#include <string>
#include <thread>
#include <windows.h> // TODO: C++20 std::thread and portable mmap

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

void csv_indexing_line_init(csv_indexing_line_t *self) {
  jvec_csv_indexing_field_t_init(&self->fields2);
}

void csv_indexing_line_t::work() {
  int64_t field_size{};
  int64_t field_offset{};
  for (int64_t i = 0; i < line_size; ++i) {
    if (indexer->contents[i + line_offset] == ',') {

      csv_indexing_field_t indexing_field = {field_offset, field_size};

      fields.push_back(indexing_field);
      this->fields2.push_back(&this->fields2, &indexing_field, 1);

      max_field_offset = JMAX(max_field_offset, field_offset);
      max_field_size = JMAX(max_field_size, field_size);
      field_offset = i + 1;
      field_size = 0;
    } else {
      ++field_size;
    }
  }

  // Indexing each line will finish unordered.
  // Queue the data to master to sort and write.
  std::unique_lock<std::mutex> lock(indexer->mutex);
  indexer->lines.emplace_back(*this);
  --(indexer->queue_size);
  indexer->condition.notify_one();
}

unsigned long csv_indexing_line_t::static_work(void *p) {
  csv_indexing_line_t *self = static_cast<csv_indexing_line_t *>(p);
  self->work();
  delete self;
  return 0;
}

/* like std::max_element */
void *max_element(void *begin, void *end, size_t size,
                  bool (*less)(void *a, void *b)) {
  char *cbegin = (char *)begin;
  char *cend = (char *)end;
  char *m = cbegin;
  char *iter = cbegin;

  for (iter = cbegin; iter != cend; iter += size) {
    if (less(m, iter))
      m = iter;
  }

  return iter;
}

bool line_less_by_field_size(void *va, void *vb) {
  csv_indexing_line_t *a = (csv_indexing_line_t *)va;
  csv_indexing_line_t *b = (csv_indexing_line_t *)vb;
  return a->fields.size() < b->fields.size();
}

bool line_less_by_max_field_size(void *va, void *vb) {
  csv_indexing_line_t *a = (csv_indexing_line_t *)va;
  csv_indexing_line_t *b = (csv_indexing_line_t *)vb;
  return a->max_field_size < b->max_field_size;
}

bool line_less_by_max_field_offset(void *va, void *vb) {
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

void csv_indexer_t::index_file(const char *file_path) {

  csv_persistant_index_t index_header{};
  index_header.version[0] = 1;
  std::string index_file_path = std::string(file_path) + ".index";

  read_entire_file_t file = {0};
  file.file_path = file_path;
  file.malloc = &jmalloc_default;
  file.stdio = &jstdio_default;
  read_entire_file(&file);
  this->contents = file.contents;

  /* Split each line rapidly and hand off line parsing to line workers. */
  int64_t line_start = 0;
  int64_t line_size = 0;

  for (int64_t file_position = 0; file_position < file.size; ++file_position) {
    char ch = file.contents[file_position];
    if (ch == '\r' || ch == '\n') {
      file_position += (ch == '\r' && (file_position + 1) < file.size &&
                        file.contents[file_position + 1] == '\n');

      index_header.max_line_contents_size =
          JMAX(index_header.max_line_contents_size, line_size);

      index_header.max_line_contents_offset =
          JMAX(index_header.max_line_contents_offset, file_position);

      auto work =
          std::make_unique<csv_indexing_line_t>(this, line_start, line_size);

      csv_indexing_line_init(work.get());

      {
        std::unique_lock<std::mutex> lock(mutex);
        QueueUserWorkItem(&csv_indexing_line_t::static_work, work.get(), 0);
        work.release();
        ++queue_size;
      }
      line_start = file_position + 1;
      line_size = 0;
      continue;
    }
    ++line_size;
  }

  {
    std::unique_lock<std::mutex> lock(mutex);
    condition.wait(mutex, [&] { return queue_size == 0; });
  }

  qsort(&lines.front(), lines.size(), sizeof(csv_indexing_line_t), csv_indexing_line_compare_v);

  csv_indexing_line_t *maxelem = (csv_indexing_line_t *)max_element(
      &lines.front(), &lines.back(), sizeof(*maxelem), line_less_by_field_size);
  index_header.max_field_count = maxelem->fields2.size(&maxelem->fields2);

  maxelem = (csv_indexing_line_t *)max_element(&lines.front(), &lines.back(),
                                               sizeof(*maxelem),
                                               line_less_by_max_field_size);
  index_header.max_field_size = maxelem->max_field_size;

  maxelem = (csv_indexing_line_t *)max_element(&lines.front(), &lines.back(),
                                               sizeof(*maxelem),
                                               line_less_by_max_field_offset);
  index_header.max_field_offset = maxelem->max_field_offset;

  std::vector<char> index_contents;
  index_contents.resize(sizeof(index_header));

  index_header.path_size = strlen(file_path);
  index_header.offset_to_path = index_contents.size();
  index_contents.insert(index_contents.end(), file_path,
                        file_path + strlen(file_path) + 1);

  auto put_int = [&](int64_t a, int size) {
    int64_t offset = index_contents.size();
    index_contents.resize(offset + 8);
    *reinterpret_cast<int64_t *>(&index_contents[offset]) = a;
    index_contents.resize(offset + size);
  };

  index_contents.resize(round_up(index_contents.size(), 8), 0);
  index_header.offset_to_lines = index_contents.size();

  for (auto &line : lines) {
    index_contents.resize(round_up(index_contents.size(), 8), 0);

    int8_t const field_count_size = bytes_for_value(line.fields.size());
    int8_t const field_offset_size = bytes_for_value(line.max_field_offset);
    int8_t const field_size_size = bytes_for_value(line.max_field_size);

    put_int(field_count_size, 1);
    put_int(field_offset_size, 1);
    put_int(field_size_size, 1);

    put_int(line.fields.size(), field_count_size);

    for (auto &field : line.fields)
      put_int(field.offset, field_offset_size);

    for (auto &field : line.fields)
      put_int(field.size, field_size_size);
  }

  index_header.total_size = index_contents.size();

  *reinterpret_cast<csv_persistant_index_t *>(&index_contents[0]) =
      index_header;

  FILE *file_w = fopen(index_file_path.c_str(), "wb");
  fwrite(&index_contents[0], 1, index_contents.size(), file_w);
  fclose(file_w);
}

#endif

int main(int /*argc*/, char **argv) {
#if _WIN32
  if (strcmp(argv[1], "index") == 0) {
    csv_indexer_t().index_file(argv[2]);
  }
#endif
}

#if _WIN32

#include "jvec_deps.h"
#define T csv_indexing_field_t
#include "jvec.c"

#endif
