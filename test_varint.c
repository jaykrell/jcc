/* test_varint.c */

#pragma warning(disable : 4100) /* unused parameter */
#include "jvarint.h"
#include <assert.h>
#include <limits.h>
#include <stdio.h>

/*************************************************************************************************/

int main(void) {

  jvarint_encode_t encode = {0};
  jvarint_decode_t decode = {0};
  int8_t signed8 = 0;
  uint8_t unsigned8 = 0;
  int16_t signed16 = 0;
  uint16_t unsigned16 = 0;
  int32_t signed32 = 0;
  uint32_t unsigned32 = 0;
  int64_t signed6 = 0;
  uint64_t unsigned64 = 0;
  int64_t i = 0;

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
    jvarint_decode_signed(&decode);
    assert(decode.signed_value == i);

    if (i >= 0) {
      encode.bytes_required = 0;
      jvarint_encode_unsigned((uint64_t)i, &encode);
      decode.buffer_size = encode.bytes_required;
      jvarint_decode_unsigned(&decode);
      assert(decode.unsigned_value == i);
    }
  }
}
