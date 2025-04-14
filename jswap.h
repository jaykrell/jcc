#ifndef JSWAP_H
#define JSWAP_H 1

#define JSWAP(type, a, b)                                                      \
  {                                                                            \
    type temp = (a);                                                           \
    (a) = (b);                                                                 \
    (b) = temp;                                                                \
  }

#endif
