/* varint handling in the fashion of wasm and presumably protobuffers and LLVM
 * also known as LEB/ULEB
 *
 * - output in little endian order
 * - output 7 bits at a time
 * - high bit set for all but last byte
 * - sign bit for signed values is second to last bit
 *
 * Implications:
 *  - unsigned numbers <  0x0000000000000080 are one byte
 *  - unsigned numbers <  0x0000000000004000 are two bytes
 *  - unsigned numbers <  0x0000000000200000 are three bytes
 *  - unsigned numbers <  0x0000000010000000 are four bytes
 *  - unsigned numbers <  0x0000000800000000 are five bytes
 *  - unsigned numbers <  0x0000040000000000 are six bytes
 *  - unsigned numbers <  0x0002000000000000 are seven bytes
 *  - unsigned numbers <  0x0100000000000000 are eight bytes
 *  - unsigned numbers <  0x8000000000000000 are nine bytes
 *  - unsigned numbers >= 0x8000000000000000 are ten bytes
 */

#include "jvarint.h"

/* sign bit of byte is second high-order bit (0x40) */
static uint8_t const jvarint_sign_bit = 0x40;

void jvarint_encode_signed(int64_t value, jvarint_encode_t *args)
/* https://en.wikipedia.org/wiki/LEB128 */
{
  uint8_t more = 0x80;
  int64_t const zero = 0;
  size_t const size = args->size;
  int64_t const negative = (value < 0);

  /* as the value is right shifted 7, be sure to sign fill the upper 7 bits, */
  int64_t const sign_extend = (negative ? (~zero << (size - 7)) : 0);

  args->bytes_required = 0;

  while (more) {
    uint8_t byte = (0x7f & value);
    value = ((value >> 7) | sign_extend);
    /* When the value has become 0 or -1, and the sign bit is correct, done, and
     * stop setting high bit. */
    if ((value == 0 && !(byte & jvarint_sign_bit)) ||
        (value == -1 && (byte & jvarint_sign_bit)))
      more = 0;
    args->buffer[args->bytes_required++] = (byte | more);
  }
}

void jvarint_encode_unsigned(uint64_t value, jvarint_encode_t *args) {
  uint8_t more = 0x80;

  while (more) {
    uint8_t byte = (0x7f & value);
    value >>= 7;
    more = (value ? more : 0);
    args->buffer[args->bytes_required++] = (byte | more);
  }
}

int jvarint_decode_adapt_buffer_to_function_pointer(void *void_context) {
  jvarint_decode_t *args = (jvarint_decode_t *)void_context;
  if (args->index >= args->buffer_size)
    return -1;
  return args->buffer[args->index++];
}

uint64_t jvarint_decode_unsigned(jvarint_decode_t *args) {
  uint64_t unsigned_value = 0;
  /* TODO: What if shift goes beyond 64? */
  args->shift = 0;

  if (args->buffer) {
    args->read_byte = jvarint_decode_adapt_buffer_to_function_pointer;
    args->read_byte_context = args;
    args->index = 0;
  }

  do {
    args->byte = args->read_byte(args->read_byte_context);
    if (args->byte < 0) {
      args->err = args->byte;
      return unsigned_value;
    }
    ++(args->bytes_consumed);
    unsigned_value |= ((args->byte & 0x7F) << args->shift);
    args->shift += 7;
  } while (args->byte & 0x80);

  return unsigned_value;
}

int64_t jvarint_decode_signed(jvarint_decode_t *args) {
  /* TODO: What if shift goes beyond 64? */
  int64_t zero = 0;
  int64_t signed_value = jvarint_decode_unsigned(args);

  if ((args->shift < args->size) && (args->byte & jvarint_sign_bit))
    signed_value |= (~zero << args->shift); /* sign extend */

  return signed_value;
}
