/* jspan is like std::span
 */
#if !JSPAN_H
#define JSPAN_H 1

#if __cplusplus
extern "C" {
#endif

#define JSPAN(T)                                                               \
  struct {                                                                     \
    T *data;                                                                   \
    size_t size;                                                               \
  }

typedef JSPAN(char) jspan_generic;
typedef JSPAN(char) jspan_char_t;
typedef JSPAN(uint64_t) jspan_uint64_t;

#if __cplusplus
}
#endif
#endif
