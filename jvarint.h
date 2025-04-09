#ifndef JVARINT_H
#define JVARINT_H 1

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
  size_t size; /* bits */
  size_t bytes_required;
  uint8_t buffer[16];
} jvarint_encode_t;

typedef struct jvarint_decode_t {
  size_t size; /* bits */
  /* use should set read_byte or buffer */
  int (*read_byte)(void *);
  void *read_byte_context;
  uint8_t *buffer;
  size_t buffer_size;
  size_t bytes_consumed;
  int err;

  /* private */
  int byte;
  unsigned shift;
  int pad;
  size_t index;
} jvarint_decode_t;

void jvarint_encode_signed(int64_t, jvarint_encode_t *args);
int64_t jvarint_decode_signed(jvarint_decode_t *args);

void jvarint_encode_unsigned(uint64_t, jvarint_encode_t *args);
uint64_t jvarint_decode_unsigned(jvarint_decode_t *args);

#if __cplusplus
} /* extern C */
#endif

#endif /* JVARINT_H */
