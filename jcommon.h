#define NOMINMAX 1
#define _CRT_SECURE_NO_WARNINGS 1
#if _MSC_VER
#pragma warning(disable : 4018) /* unsigned/signed mismatch */
#endif

#if !_MSC_VER && !defined(__cdecl)
#define __cdecl
#endif

#include <assert.h>
#include <errno.h>
#include <limits.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "jhash.h"
#include "jlist.h"
#include "jstring_constant.h"
#include "jvec.h"
