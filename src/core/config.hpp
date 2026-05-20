/**************************************************************************/
/*  config.hpp                                                            */
/**************************************************************************/
/*                         This file is part of:                          */
/*                         MACHINA MATH LIBRARY                           */
/**************************************************************************/
/* Copyright (c) 2026-present Jose A. Perez de Azpillaga                  */
/*                                                                        */
/* Permission is hereby granted, free of charge, to any person obtaining  */
/* a copy of this software and associated documentation files (the        */
/* "Software"), to deal in the Software without restriction, including    */
/* without limitation, to use, copy, modify, merge, publish,              */
/* distribute, sublicense, and/or sell copies of the Software, and to     */
/* permit persons to whom the Software is furnished to do so, subject to  */
/* the following conditions:                                              */
/*                                                                        */
/* The above copyright notice and this permission notice shall be         */
/* included in all copies or substantial portions of the Software.        */
/*                                                                        */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,        */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF     */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. */
/* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY   */
/* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,   */
/* OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE           */
/* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                 */
/**************************************************************************/

#pragma once

#ifndef MML_USE_SIMD
#define MML_USE_SIMD 0
#endif

#if MML_USE_SIMD
#if defined(__AVX2__)
#define MML_SIMD_AVX2 1
#define MML_SIMD_AVX 1
#define MML_SIMD_SSE4_1 1
#define MML_SIMD_SSE3 1
#define MML_SIMD_SSE2 1
#define MML_SIMD_SSE 1
#elif defined(__AVX__)
#define MML_SIMD_AVX 1
#define MML_SIMD_SSE4_1 1
#define MML_SIMD_SSE3 1
#define MML_SIMD_SSE2 1
#define MML_SIMD_SSE 1
#elif defined(__SSE4_1__)
#define MML_SIMD_SSE4_1 1
#define MML_SIMD_SSE3 1
#define MML_SIMD_SSE2 1
#define MML_SIMD_SSE 1
#elif defined(__SSE3__)
#define MML_SIMD_SSE3 1
#define MML_SIMD_SSE2 1
#define MML_SIMD_SSE 1
#elif defined(__SSE2__)
#define MML_SIMD_SSE2 1
#define MML_SIMD_SSE 1
#elif defined(_M_X64) || defined(__x86_64__)
#define MML_SIMD_SSE2 1
#define MML_SIMD_SSE 1
#elif defined(_M_IX86) || defined(__i386__)
#define MML_SIMD_SSE 1
#elif defined(__ARM_NEON) || defined(__aarch64__)
#define MML_SIMD_NEON 1
#endif
#endif

#ifndef MML_SIMD_SSE
#define MML_SIMD_SSE 0
#endif
#ifndef MML_SIMD_SSE2
#define MML_SIMD_SSE2 0
#endif
#ifndef MML_SIMD_SSE3
#define MML_SIMD_SSE3 0
#endif
#ifndef MML_SIMD_SSE4_1
#define MML_SIMD_SSE4_1 0
#endif
#ifndef MML_SIMD_AVX
#define MML_SIMD_AVX 0
#endif
#ifndef MML_SIMD_AVX2
#define MML_SIMD_AVX2 0
#endif
#ifndef MML_SIMD_NEON
#define MML_SIMD_NEON 0
#endif

#if defined(_MSC_VER)
#define MML_FORCE_INLINE __forceinline
#else
#define MML_FORCE_INLINE inline __attribute__((always_inline))
#endif

#if defined(_MSC_VER) && _MSC_VER >= 1910
#pragma warning(disable : 4514 4179)
#pragma warning(disable : 4457)
#endif

#ifndef MML_EPSILON
#define MML_EPSILON 1e-6
#endif