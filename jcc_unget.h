#if !JCC_UNGET_H
#define JCC_UNGET_H 1

/* Some phases require lookahead of a single character.
 * This is like stdio unget.
 * TODO: expand to arbitrary size? jvec?
 * TODO: Rename to lookahead?
 * TODO: Expand to contain tokens?
 * Probably we need a jvec of union of pptoken, token, char.
 * For example, perhaps, we tokenize an entire file at a time,
 * then parse it. (Rather, pptokenize, then tokenize, then parse.
 * And even then, sometimes tokenization is context-dependent
 * such as if we implement C++11 templates.)
 * Nevertheless, this suffices for now.
 */
typedef struct jcc_unget_t {
  int value;
  int valid;
} jcc_unget_t;

/* Generic get from unget buffer (i.e. lookahead of length 1). */
int jcc_unget_get(jcc_unget_t *unget, int *value);

void jcc_unget_unget(jcc_unget_t *unget, int value);

#endif
