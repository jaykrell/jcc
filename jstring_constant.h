/* C preprocessor. */

#if !JSTRING_CONSTANT_H
#define JSTRING_CONSTANT_H 1

typedef struct jstring_constant_t {
  char *chars;
  size_t size;
} jstring_constant_t;

#define JSTRING_CONSTANT(x) (x), sizeof(x) - 1

#endif
