#include "jcc.h"

void jcc_phase1_unget(jcc_t *jcc, int ch)
/* Unget next character, using newline/carriage_returns unget buffer.
 */
{
    jcc_unget_unget(&jcc->phase1_unget, ch);
}

int jcc_phase1_getchar(jcc_t *jcc, int *pch)
/* Get next character, handling newline/carriage_returns.
 * Specifically: \n returns as \n
 * \r\n returns as \n
 * \r not followed by \n returns as \n
 * Everything else is itself (which covers \n).
 * Upon seeing \r we have to readahead 1.
 * If the readahead is \n, it accepted.
 * If the readhead is not \n, it is put back ("unget").
 * As such, no other layer should have to handle or see \r.
 *
 * Translation phase 1.
 *
 * Physical source file multibyte characters are mapped, in an implementation-
 * defined manner, to the source character set (introducing new-line characters for
 * end-of-line indicators) if necessary. Trigraph sequences are replaced by
 * corresponding single-character internal representations
 */
{
    size_t actual = 0;
    int err = 0;
    unsigned char ch = 0;

    if (jcc_unget_get(&jcc->phase1_unget, pch))
        return 0;

    if ((err = jcc->cfile->file->err)) {
        *pch = JCC_CHAR_ERROR;
        return err;
    }

    if (jcc->cfile->file->eof) {
        *pch = JCC_CHAR_END_OF_FILE;
        return 0;
    }

    if ((err = jfile_read(jcc->cfile->file, &ch, 1, &actual))) {
        *pch = JCC_CHAR_ERROR;
        return err;
    }

    if (actual == 0) {
        jcc->cfile->file->eof = 1;
        *pch = JCC_CHAR_END_OF_FILE;
        return 0;
    }

    *pch = ch;

    if (ch != '\r')
        return 0;

    /* Try to read one past \r, which could be end of file. */

    err = jfile_read(jcc->cfile->file, &ch, 1, &actual);
    if (err)
        goto return_newline;
    if (actual == 0) {
        jcc->cfile->file->eof = 1;
        goto return_newline;
    }

    if (ch != '\n')
        jcc_unget_unget(&jcc->phase1_unget, ch);
    return_newline:
    *pch = '\n';
    return 0;
}
