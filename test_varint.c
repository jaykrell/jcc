/* test_varint.c */

#if _MSC_VER
#pragma warning(disable : 4100) /* unused parameter */
#endif
#include "jvarint.h"
#include <assert.h>
#include <limits.h>
#include <stdio.h>

/*************************************************************************************************/

int main(void) {

  jvarint_encode_t encode = {0};
  jvarint_decode_t decode = {0};
  int64_t i = 0;
  int64_t signed_value = {0};
  uint64_t unsigned_value = {0};

  encode.bytes_required = 0;
  jvarint_encode_unsigned(0, &encode);
  assert(encode.bytes_required == 1);

  encode.bytes_required = 0;
  jvarint_encode_unsigned(1, &encode);
  assert(encode.bytes_required == 1);

  encode.bytes_required = 0;
  jvarint_encode_unsigned(0x7f, &encode);
  assert(encode.bytes_required == 1);

  encode.bytes_required = 0;
  jvarint_encode_unsigned(0x80, &encode);
  assert(encode.bytes_required == 2);

  encode.size = 64;
  decode.size = 64;
  decode.buffer = encode.buffer;

  for (i = INT_MIN; i <= INT_MAX; ++i) {
    encode.bytes_required = 0;
    jvarint_encode_signed(i, &encode);
    decode.buffer_size = encode.bytes_required;
    signed_value = jvarint_decode_signed(&decode);
    assert(signed_value == i);

    if (i >= 0) {
      encode.bytes_required = 0;
      jvarint_encode_unsigned((uint64_t)i, &encode);
      decode.buffer_size = encode.bytes_required;
      unsigned_value = jvarint_decode_unsigned(&decode);
      assert(unsigned_value == i);
    }
  }
  return 0;
}
