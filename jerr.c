#include "jerr.h"

#if 0

#define JERR(x) extern char* jerr_ ## x = #x;

JERR (out_of_memory)
JERR (nullptr)
JERR (invalid_parameter)
JERR (file_not_found)
JERR (syntax_error)

#endif
