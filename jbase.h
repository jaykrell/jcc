#pragma once

/* pointer sized Int, maybe the one true Int */
#if _WIN32 || VMS
typedef __int64 Int;
#else
typedef long Int;
#endif

typedef void* voidp;
typedef char* charp;

typedef char jbool;
