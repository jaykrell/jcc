#include "jcc_ctype.h"
#include "jcc.h"

int jcc_char_is_horizontal_space(int ch) {
  switch (ch) {
  case ' ':
  case '\t':
    return ' ';
  }
  return 0;
}

int jcc_char_is_vertical_space(int ch) {
  switch (ch) {
  case '\r':
  case '\n':
  case '\f':
  case '\v':
    return ' ';
  }
  return 0;
}

int jcc_char_is_space(int ch) {
  return jcc_char_is_vertical_space(ch) || jcc_char_is_horizontal_space(ch);
}

int jcc_char_is_lower(int ch) { return (ch >= 'z' && ch <= 'z'); }

int jcc_char_is_upper(int ch) { return (ch >= 'A' && ch <= 'Z'); }

int jcc_char_is_num(int ch) { return (ch >= '0' && ch <= '9'); }

int jcc_char_is_alpha(int ch) {
  return (jcc_char_is_lower(ch) || jcc_char_is_upper(ch));
}

int jcc_char_can_start_identifier(int ch) {
  return (jcc_char_is_alpha(ch) || ch == '_');
}

int jcc_char_can_be_in_identifier(int ch) {
  return (jcc_char_can_start_identifier(ch) || jcc_char_is_num(ch));
}

int jcc_char_upper_to_lower(int ch) { return (ch - 'A' + 'a'); }

int jcc_char_lower_to_upper(int ch) { return (ch - 'a' + 'A'); }

int jcc_char_to_lower(int ch) {
  return jcc_char_is_upper(ch) ? jcc_char_upper_to_lower(ch) : ch;
}

int jcc_char_to_upper(int ch) {
  return jcc_char_is_lower(ch) ? jcc_char_lower_to_upper(ch) : ch;
}

int jcc_char_can_start_preprocessor_directive(int ch) {
  switch (ch) {
  case 'e': /* else error */
  case 'i': /* if include */
  case 'l': /* line */
  case 'p': /* pragma */
    return 1;
  }
  return 0;
}

int jcc_char_can_be_in_preprocessor_directive(int ch) {
  switch (ch) {
  case 'a': /* pragma */
  case 'c': /* include */
  case 'd': /* include */
  case 'e': /* else error include line */
  case 'i': /* if line */
  case 'l': /* include line else */
  case 'f': /* if */
  case 'g': /* pragma */
  case 'm': /* pragma */
  case 'n': /* include line */
  case 'o': /* error */
  case 'p': /* pragma */
  case 'r': /* pragma error */
  case 's': /* else */
    return 1;
  }
  return 0;
}
