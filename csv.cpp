/* comma separated value text file handling */

#define NOMINMAX
#define _CRT_SECURE_NO_WARNINGS 1
#pragma warning(disable : 4018) // unsigned/signed mismatch
#include "csv.h"
#include "read_entire_file.h"
#include <algorithm>
#include <stdio.h>
#include <string.h>
#include <string>
#include <thread>
#include <windows.h> // TODO: C++20 std::thread and portable mmap

namespace csv {

int64_t round_up(int64_t a, int64_t b) {
  int64_t mod = (a % b);
  return (mod ? (a + b - mod) : a);
}

int8_t bytes_for_value(int64_t a) {
  if (a < INT32_MIN || a > INT32_MAX)
    return 8;
  if (a < INT16_MIN || a > INT16_MAX)
    return 4;
  if (a < INT8_MIN || a > INT8_MAX)
    return 2;
  return 1;
}

int8_t bits_for_value(int64_t a) { return bytes_for_value(a) * 8; }

void indexing_line_t::work() {
  int64_t field_size{};
  int64_t field_offset{};
  for (int64_t i = 0; i < line_size; ++i) {
    if (indexer->contents[i + line_offset] == ',') {
      fields.push_back(indexing_field_t{field_offset, field_size});
      max_field_offset = std::max(max_field_offset, field_offset);
      max_field_size = std::max(max_field_size, field_size);
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

unsigned long indexing_line_t::static_work(void *p) {
  indexing_line_t *self = static_cast<indexing_line_t *>(p);
  self->work();
  delete self;
  return 0;
}

void indexer_t::index_file(const char *file_path) {
  persistant_index_t index_header{};
  index_header.version[0] = 1;
  std::string index_file_path = std::string(file_path) + ".index";
  read_entire_file_t file = {0};
  file.file_path = file_path;
  file.malloc = &jmalloc_default;
  file.stdio = &jstdio_default;
  this->contents = file.contents;

  read_entire_file(&file);

  /* Split each line rapidly and hand off line parsing to line workers. */
  int64_t line_start = 0;
  int64_t line_size = 0;

  for (int64_t file_position = 0; file_position < file.size;
       ++file_position) {
    char ch = file.contents[file_position];
    if (ch == '\r' || ch == '\n') {
      file_position += (ch == '\r' && (file_position + 1) < file.size &&
                        file.contents[file_position + 1] == '\n');

      index_header.max_line_contents_size =
          std::max(index_header.max_line_contents_size, line_size);
      index_header.max_line_contents_offset =
          std::max(index_header.max_line_contents_offset, file_position);

      auto work =
          std::make_unique<indexing_line_t>(this, line_start, line_size);

      {
        std::unique_lock<std::mutex> lock(mutex);
        QueueUserWorkItem(&indexing_line_t::static_work, work.get(), 0);
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

  std::sort(lines.begin(), lines.end());

  index_header.max_field_count =
      std::max_element(lines.begin(), lines.end(),
                       [](const auto &a, const auto &b) {
                         return a.fields.size() < b.fields.size();
                       })
          ->fields.size();

  index_header.max_field_size =
      std::max_element(lines.begin(), lines.end(),
                       [](const auto &a, const auto &b) {
                         return a.max_field_size < b.max_field_size;
                       })
          ->max_field_size;

  index_header.max_field_offset =
      std::max_element(lines.begin(), lines.end(),
                       [](const auto &a, const auto &b) {
                         return a.max_field_offset < b.max_field_offset;
                       })
          ->max_field_offset;

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

  *reinterpret_cast<persistant_index_t *>(&index_contents[0]) = index_header;

  FILE *file_w = fopen(index_file_path.c_str(), "wb");
  fwrite(&index_contents[0], 1, index_contents.size(), file_w);
  fclose(file_w);
}

} // namespace csv

int main(int /*argc*/, char **argv) {
  if (strcmp(argv[1], "index") == 0) {
    csv::indexer_t().index_file(argv[2]);
  }
}
