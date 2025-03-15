/* comma separated value text file handling */

#ifndef CVS_H
#define CVS_H 1

#pragma warning(disable : 4201) // anonymous

#include "jvec.h"
#include <stdint.h>

struct csv_indexer_t;
union csv_persistant_index_t;
struct csv_persistant_index_line_t;

// temporary in memory form while indexing
typedef struct csv_indexing_field_t {
  int64_t offset;
  int64_t size;
} csv_indexing_field_t;

typedef struct csv_indexing_line_t csv_indexing_line_t;

/* temporary in memory form while indexing */

struct csv_indexing_line_t {

  csv_indexing_line_t(csv_indexer_t *a, int64_t b, int64_t c)
      : indexer(a), line_offset(b), line_size(c) {}

  csv_indexer_t *indexer{};
  int64_t line_offset{};
  int64_t line_size{};
  int64_t max_field_offset{};
  int64_t max_field_size{};
  JVEC(csv_indexing_field_t) fields {};

  void work();
};

int csv_indexing_line_compare_v(void *a, void *b);
int csv_indexing_line_compare(csv_indexing_line_t *a, csv_indexing_line_t *b);

typedef union csv_persistant_index_t {
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
  uint8_t page[0x1000];
} csv_persistant_index_t;

typedef struct csv_persistant_index_line_t {
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
} csv_persistant_index_line_t;

typedef struct csv_indexer_t {
  JVEC(csv_indexing_line_t) lines;
  char *contents;
} csv_indexer_t;

#endif
