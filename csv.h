/* comma separated value text file handling */

#pragma once
#pragma warning(disable : 4201) // anonymous

#include <condition_variable>
#include <shared_mutex>
#include <stdint.h>
#include <thread>
#include <vector>

namespace csv {

struct indexer_t;

// temporary in memory form while indexing
struct indexing_field_t {
  int64_t offset{};
  int64_t size{};
};

// temporary in memory form while indexing
struct indexing_line_t {

  indexing_line_t(indexer_t *a, int64_t b, int64_t c)
      : indexer(a), line_offset(b), line_size(c) {}

  bool operator<(const indexing_line_t &other) const {
    return line_offset < other.line_offset;
  }
  indexer_t *indexer{};
  int64_t line_offset{};
  int64_t line_size{};
  int64_t max_field_offset{};
  int64_t max_field_size{};
  std::vector<indexing_field_t> fields{};

  void work();
  static unsigned long static_work(void *p);
};

struct indexer_t;
union persistant_index_t;
struct persistant_index_line_t;

union persistant_index_t {
  struct {
    int8_t version[8];
    int8_t endian;
    int8_t file_int_bits; /* 8, 16, 32, 64, etc. */
    int8_t pad[6];
    int64_t total_size;             /* including this header */
    int64_t max_line_contents_size; /* in bytes */
    int64_t max_line_contents_offset;
    int64_t max_field_count;
    int64_t max_field_offset;
    int64_t max_field_size;
    int64_t line_count;
    int64_t file_offset_bias; /* possibly useful if files get concatenated */
    int64_t offset_to_lines;  /* offset from start of file to index_line_t */
    int64_t path_size;
    int64_t offset_to_path;     /* 0 if not present */
    int64_t offset_to_contents; /* 0 if not present */
  };
  uint8_t page[0x10000];
};

struct persistant_index_line_t {
  /* Realistically there are only a few encodings and this could be
  an index into an array of them collected across all lines.
  Index 0 could be reserved for 64/64/64, then store one byte here
  to index into the encodings, and if we run it, fallback to 0.
  But really there's only 4x4x4=64 encodings, and that could be
  encoded here in 12 bits.
  */
  int8_t field_count_size;
  int8_t field_offset_size;
  int8_t field_size_size;
  /*
          int<field_count_size>_t  field_count;
          int<field_offset_size>_t field_offsets[field_count]; // from start of
     line int<field_size_size>_t   field_sizes[field_count];
  */
};

struct indexer_t {
  std::vector<indexing_line_t> lines{};
  std::mutex mutex{};
  std::vector<char> contents{};
  int64_t queue_size{};
  std::condition_variable_any condition{};
  void index_file(const char *file_path);
};

} // namespace csv