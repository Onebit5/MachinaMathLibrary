/**************************************************************************/
/*  traits.hpp                                                            */
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

#include "config.hpp"
#include <type_traits>

namespace mml {

template <typename T>
struct is_floating_point : std::is_floating_point<T> {};

template <typename T>
inline constexpr bool is_floating_point_v = is_floating_point<T>::value;

template <typename T>
struct is_scalar : std::is_arithmetic<T> {};

template <typename T>
inline constexpr bool is_scalar_v = is_scalar<T>::value;

struct SIMDTraits {
	static constexpr bool has_sse = (MML_SIMD_SSE != 0);
	static constexpr bool has_sse2 = (MML_SIMD_SSE2 != 0);
	static constexpr bool has_sse3 = (MML_SIMD_SSE3 != 0);
	static constexpr bool has_sse4_1 = (MML_SIMD_SSE4_1 != 0);
	static constexpr bool has_avx = (MML_SIMD_AVX != 0);
	static constexpr bool has_avx2 = (MML_SIMD_AVX2 != 0);
	static constexpr bool has_neon = (MML_SIMD_NEON != 0);

	static constexpr bool enabled = (MML_USE_SIMD != 0) && (has_sse2 || has_avx || has_neon);

	static constexpr int vector_width_float = has_avx ? 8 : (has_sse2 ? 4 : 1);
	static constexpr int vector_width_double = has_avx ? 4 : (has_sse2 ? 2 : 1);
};

template <bool UseSimd>
struct Implementation {
	static constexpr bool use_simd = UseSimd && SIMDTraits::enabled;
};

} //namespace mml