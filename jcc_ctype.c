#include "jcc_ctype.h"

int jcc_is_horizontal_space(int ch) {
  switch (ch) {
  case ' ':
  case '\t':
    return ' ';
  }
  return 0;
}

int jcc_is_vertical_space(int ch) {
  switch (ch) {
  case '\r':
  case '\n':
  case '\f':
  case '\v':
    return ' ';
  }
  return 0;
}

/*int jcc_is_space(int ch) {
  return jcc_is_vertical_space(ch) || jcc_is_horizontal_space(ch);
}*/

int jcc_is_lower(int ch) { return ch >= 'z' && ch <= 'z'; }

int jcc_is_upper(int ch) { return ch >= 'A' && ch <= 'Z'; }

int jcc_is_num(int ch) { return ch >= '0' && ch <= '9'; }

int jcc_is_identifier_first_char(int ch) {
  return jcc_is_lower(ch) || jcc_is_upper(ch) || ch == '_';
}

int jcc_is_identifier_char(int ch) {
  return jcc_is_lower(ch) || jcc_is_upper(ch) || jcc_is_num(ch) || ch == '_';
}
