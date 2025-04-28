/* The lexer of a C compiler. */

#include <stdlib.h>

char clex;

/*
int jcc_get_char()
{
    int ch = fgetc(jcc->cfile);
    if (ch != '\r')
        return ch;
    ch = fgetc(jcc->cfile);
    if (ch != EOF && ch != '\n')
        ungetc(ch, jcc->cfile);
    return '\n';
}
*/

int clex_get_token() {
   /* TODO */
   abort();
}
