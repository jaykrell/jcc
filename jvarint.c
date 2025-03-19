#include "jvarint.h"

/* sign bit of byte is second high-order bit (0x40) */
uint8_t const jvarint_sign_bit = 0x40;

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

int jvarint_decode_adapt_buffer_to_function_pointer(void *vcontext) {
  jvarint_decode_t *context = (jvarint_decode_t *)vcontext;
  return context->buffer_size ? context->buffer[context->buffer_size++] : -1;
}

void jvarint_decode_unsigned(jvarint_decode_t *args) {
  /* TODO: What if shift goes beyond 64? */
  args->shift = 0;

  if (args->buffer) {
    args->read_byte = jvarint_decode_adapt_buffer_to_function_pointer;
    args->read_byte_context = args;
  }

  do {
    args->byte = args->read_byte(args->read_byte_context);
    if (args->byte < 0) {
      args->err = args->byte;
      return;
    }
    ++(args->bytes_consumed);
    args->u.unsigned_value |= ((args->byte & 0x7F) << args->shift);
    args->shift += 7;
  } while (args->byte & 0x80);
}

void jvarint_decode_signed(jvarint_decode_t *args) {
  /* TODO: What if shift goes beyond 64? */
  int64_t zero = 0;
  int64_t signed_value;

  jvarint_decode_unsigned(args);
  signed_value = args->u.unsigned_value;

  if ((args->shift < args->size) && (args->byte & jvarint_sign_bit))
    signed_value |= (~zero << args->shift); /* sign extend */

  args->u.signed_value = signed_value;
}
