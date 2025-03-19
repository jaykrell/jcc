#pragma once

#include <stddef.h>
#include <stdint.h>

#if _MSC_VER
#pragma warning(disable : 4668) /* #if vs. #ifdef */
#pragma warning(disable : 4201) /* nameless union */
#endif

#if __cplusplus
extern "C" {
#endif

typedef struct jvarint_encode_t {
  union {
    uint64_t unsigned_value;
    int64_t signed_value;
  } u;
  size_t size; /* bits */
  size_t is_signed;
  size_t bytes_required;
  uint8_t buffer[16];
} jvarint_encode_t;

typedef struct jvarint_decode_t {
  union {
    int64_t signed_value;
    uint64_t unsigned_value;
  } u;
  int (*read_byte)(void *);
  void *read_byte_context;
  uint8_t *buffer;
  size_t size; /* bits */
  size_t buffer_size;
  size_t bytes_consumed;
  size_t is_signed;
  size_t invalid;
  int err;

  /* private */
  int byte;
  unsigned shift;
  int pad;
} jvarint_decode_t;

void jvarint_encode(jvarint_encode_t *args);
void jvarint_decode(jvarint_decode_t *args);

#if __cplusplus
} /* extern C */
#endif
