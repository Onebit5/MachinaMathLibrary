/**************************************************************************/
/*  epsilon.hpp                                                           */
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
#include <cmath>
#include <limits>
#include <type_traits>

namespace mml {

template <typename T>
struct Epsilon {
	static_assert(std::is_floating_point_v<T>, "Epsilon only supports floating-point types");

	static constexpr T value = T(MML_EPSILON);

	static constexpr T high = std::is_same_v<T, float> ? T(1e-6f) : T(1e-12);

	static constexpr T medium = std::is_same_v<T, float> ? T(1e-4f) : T(1e-8);

	static constexpr T low = std::is_same_v<T, float> ? T(1e-2f) : T(1e-4);

	static constexpr T ultra = std::is_same_v<T, float> ? T(1e-7f) : T(1e-14);

	static constexpr T machine = std::numeric_limits<T>::epsilon();

	MML_FORCE_INLINE static bool equal(T a, T b, T eps = value) {
		return std::abs(a - b) <= eps;
	}

	MML_FORCE_INLINE static bool equal_high(T a, T b) {
		return equal(a, b, high);
	}

	MML_FORCE_INLINE static bool equal_medium(T a, T b) {
		return equal(a, b, medium);
	}

	MML_FORCE_INLINE static bool equal_low(T a, T b) {
		return equal(a, b, low);
	}

	MML_FORCE_INLINE static bool is_zero(T a, T eps = value) {
		return std::abs(a) <= eps;
	}

	MML_FORCE_INLINE static bool is_zero_high(T a) {
		return is_zero(a, high);
	}

	MML_FORCE_INLINE static bool is_zero_medium(T a) {
		return is_zero(a, medium);
	}

	MML_FORCE_INLINE static bool is_zero_low(T a) {
		return is_zero(a, low);
	}

	MML_FORCE_INLINE static bool less(T a, T b, T eps = value) {
		return (b - a) > eps;
	}

	MML_FORCE_INLINE static bool greater(T a, T b, T eps = value) {
		return (a - b) > eps;
	}

	MML_FORCE_INLINE static bool less_equal(T a, T b, T eps = value) {
		return !greater(a, b, eps);
	}

	MML_FORCE_INLINE static bool greater_equal(T a, T b, T eps = value) {
		return !less(a, b, eps);
	}

	MML_FORCE_INLINE static bool nearly_equal(T a, T b, T relative_eps = value) {
		T diff = std::abs(a - b);
		T abs_a = std::abs(a);
		T abs_b = std::abs(b);
		T max_abs = (abs_a > abs_b) ? abs_a : abs_b;
		T scale = (T(1) > max_abs) ? T(1) : max_abs;
		return diff <= scale * relative_eps;
	}

	MML_FORCE_INLINE static bool nearly_equal_abs(T a, T b, T absolute_eps = value) {
		return std::abs(a - b) <= absolute_eps;
	}

	MML_FORCE_INLINE static bool is_within(T val, T target, T tolerance = value) {
		return std::abs(val - target) <= tolerance;
	}

	MML_FORCE_INLINE static bool approx_equal(T a, T b, T relative_eps = value) {
		return nearly_equal(a, b, relative_eps);
	}

	MML_FORCE_INLINE static bool approx_zero(T a, T relative_eps = value) {
		T abs_a = std::abs(a);
		T scale = (T(1) > abs_a) ? T(1) : abs_a;
		return abs_a <= scale * relative_eps;
	}

	MML_FORCE_INLINE static bool approx_less_equal(T a, T b, T relative_eps = value) {
		return (a - b) <= relative_eps * (std::abs(a) > T(1) ? std::abs(a) : T(1));
	}

	MML_FORCE_INLINE static bool approx_greater_equal(T a, T b, T relative_eps = value) {
		return (b - a) <= relative_eps * (std::abs(b) > T(1) ? std::abs(b) : T(1));
	}
};

using Epsilonf = Epsilon<float>;
using Epsilond = Epsilon<double>;

} //namespace mml