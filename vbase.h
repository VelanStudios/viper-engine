#pragma once

/*
** Viper Engine - Copyright (C) 2016 Velan Studios - All Rights Reserved
**
** Permission is hereby granted, free of charge, to any person obtaining a copy
** of this software and associated documentation files (the "Software"), to
** deal in the Software without restriction, including without limitation the
** rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
** sell copies of the Software, and to permit persons to whom the Software is
** furnished to do so, subject to the following conditions:
**
** The above copyright notice and this permission notice shall be included in
** all copies or substantial portions of the Software.
**
** THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
** IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
** FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
** AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
** LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
** FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
** IN THE SOFTWARE.
**
** Catchall base header.
*/

#include <stdbool.h>
#if !defined(VPREPROCESS)
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#else
#define int8_t signed char
#define int16_t short
#define int32_t int
#define int64_t long long
#define uint8_t unsigned char
#define uint16_t unsigned short
#define uint32_t unsigned int
#define uint64_t unsigned long long
#endif

#if defined(__clang__) || defined(__GNUC__)
#define force_inline __attribute__((always_inline)) inline
#define no_inline __attribute__((noinline))
#elif defined(_MSC_VER)
#define force_inline __forceinline
#define no_inline __declspec(noinline)
#endif

#if !defined(_countof)
#define _countof(A) ((int)(sizeof(A) / sizeof(*A)))
#endif

#if !defined(__min)
#define __min(A, B) ((A) < (B) ? (A) : (B))
#endif

#if !defined(__max)
#define __max(A, B) ((A) > (B) ? (A) : (B))
#endif

#if !defined(_MSC_VER) && !defined(__STDC_LIB_EXT1__)
#define strcpy_s(dest, len, src) strcpy(dest, src)
#endif

#if defined(__GNUC__)
/* __thread is built-in */
#elif __STDC_VERSION__ >= 201112L
#define __thread _Thread_local
#elif defined(_MSC_VER)
#define __thread __declspec(thread)
#else
#error Cannot define __thread
#endif

#define CAT2(a,b) a##b
#define CAT(a,b) CAT2(a,b)
#define UNIQUE_ID(PREFIX) CAT(PREFIX,CAT(__LINE__,__COUNTER__))
