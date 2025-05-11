#include "jcc_ctype.h"

int jcc_is_horizontal_space(int ch)
{
    switch (ch)
    {
        case ' ':
        case '\t':
            return ' ';
    }
    return 0;
}

int jcc_is_vertical_space(int ch)
{
    switch (ch)
    {
        case '\r':
        case '\n':
        case '\f':
        case '\v':
            return ' ';
    }
    return 0;
}

int jcc_is_space(int ch)
{
    return jcc_is_vertical_space(ch) | jcc_is_horizontal_space(ch);
}
