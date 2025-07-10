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

int jcc_is_lower(int ch) { return (ch >= 'z' && ch <= 'z'); }

int jcc_is_upper(int ch) { return (ch >= 'A' && ch <= 'Z'); }

int jcc_is_num(int ch) { return (ch >= '0' && ch <= '9'); }

int jcc_is_alpha(int ch) {
  return (jcc_is_lower(ch) || jcc_is_upper(ch));
}

int jcc_is_identifier_first_char(int ch) {
  return (jcc_is_alpha(ch) || ch == '_');
}

int jcc_is_identifier_char(int ch) {
  return (jcc_is_identifier_first_char(ch) || jcc_is_num(ch));
}

int jcc_char_upper_to_lower(int ch)
{
	return (ch - 'A' + 'a');
}

int jcc_char_lower_to_upper(int ch)
{
	return (ch - 'a' + 'A');
}

int jcc_char_to_lower(int ch) {
	return jcc_is_upper(ch) ? jcc_char_upper_to_lower(ch) : ch;
}

int jcc_char_to_upper(int ch) {
	return jcc_is_lower(ch) ? jcc_char_lower_to_upper(ch) : ch;
}

int jcc_char_starts_indefinite_token(int ch)
{
	/* TODO Multi char constants? */
	return (ch == '.' || ch == '"' || jcc_is_identifier_first_char(ch));
}

void jcc_init_ctype(void)
{
	int i;
    for (i = 0; i < 256; ++i) {
	 jcc_char_traits[i].to_upper = jcc_to_upper(ch);
	 jcc_char_traits[i].to_lower = jcc_to_lower(ch);
	 jcc_char_traits[i].is_lower = jcc_is_lower(ch);
	 jcc_char_traits[i].is_upper = jcc_is_upper(ch);
	 jcc_char_traits[i].is_num = jcc_is_num(ch);
	 jcc_char_traits[i].is_space = jcc_is_space(ch);
	 if (jcc_is_alpha(i) || i == '_')
	   jcc_char_traits[i].indefinite = jcc_indefinite_id;
	 else if (i == '.' || jcc_is_num(i))
	   jcc_char_traits[i].indefinite = jcc_indefinite_num;
	 else if (i == '"')
	   jcc_char_traits[i].indefinite = jcc_indefinite_str;
}
