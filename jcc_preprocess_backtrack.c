typedef struct jcc_t jcc_t;
#if _MSC_VER
#pragma warning(disable : 4100) /* unused parameters */
#endif

jcc_token_t *jcc_preprocess_backtrack_prepare(jcc_t *jcc, jcc_token_t *) {}

void jcc_preprocess_backtrack_cancel(jcc_t *jcc, jcc_token_t *) {}

void jcc_preprocess_backtrack(jcc_t *jcc, jcc_token_t *token) {
  jcc->tokens = token;
}
