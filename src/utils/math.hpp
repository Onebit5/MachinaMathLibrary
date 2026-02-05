/**************************************************************************/
/*  math.hpp                                                              */
/**************************************************************************/
/*                         This file is part of:                          */
/*                         MACHINA MATH LIBRARY                           */
/**************************************************************************/
/* Copyright (c) 2026-present Jose A. Perez                               */
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

#include "../core/config.hpp"
#include "../core/constants.hpp"
#include <algorithm>
#include <cmath>
#include <type_traits>

namespace mml {

template <typename T>
struct Math {
	static_assert(std::is_arithmetic_v<T>, "Math requires arithmetic type");

	MML_FORCE_INLINE static constexpr T abs(T value) {
		return value < T(0) ? -value : value;
	}

	MML_FORCE_INLINE static constexpr T min(T a, T b) {
		return a < b ? a : b;
	}

	MML_FORCE_INLINE static constexpr T max(T a, T b) {
		return a > b ? a : b;
	}

	MML_FORCE_INLINE static constexpr T clamp(T value, T min_val, T max_val) {
		return value < min_val ? min_val : (value > max_val ? max_val : value);
	}

	MML_FORCE_INLINE static constexpr T lerp(T a, T b, T t) {
		return a + (b - a) * t;
	}

	MML_FORCE_INLINE static constexpr T inverse_lerp(T a, T b, T value) {
		if (a == b) {
			return T(0);
		}
		return (value - a) / (b - a);
	}

	MML_FORCE_INLINE static constexpr T remap(T value, T in_min, T in_max, T out_min, T out_max) {
		T t = inverse_lerp(in_min, in_max, value);
		return lerp(out_min, out_max, t);
	}

	MML_FORCE_INLINE static T sqrt(T value) {
		return std::sqrt(value);
	}

	MML_FORCE_INLINE static T safe_sqrt(T value) {
		if (value < T(0)) {
			return T(0);
		}
		return std::sqrt(value);
	}

	MML_FORCE_INLINE static T sqrt_fast(T value) {
		return std::sqrt(value);
	}

	MML_FORCE_INLINE static T inv_sqrt(T value) {
		return T(1) / std::sqrt(value);
	}

	MML_FORCE_INLINE static T pow(T base, T exp) {
		return std::pow(base, exp);
	}

	MML_FORCE_INLINE static T exp(T value) {
		return std::exp(value);
	}

	MML_FORCE_INLINE static T log(T value) {
		return std::log(value);
	}

	MML_FORCE_INLINE static T log2(T value) {
		return std::log2(value);
	}

	MML_FORCE_INLINE static T log10(T value) {
		return std::log10(value);
	}

	MML_FORCE_INLINE static T sin(T radians) {
		return std::sin(radians);
	}

	MML_FORCE_INLINE static T cos(T radians) {
		return std::cos(radians);
	}

	MML_FORCE_INLINE static T tan(T radians) {
		return std::tan(radians);
	}

	MML_FORCE_INLINE static T asin(T value) {
		return std::asin(value);
	}

	MML_FORCE_INLINE static T acos(T value) {
		return std::acos(value);
	}

	MML_FORCE_INLINE static T safe_acos(T value) {
		T clamped = value > T(1) ? T(1) : (value < T(-1) ? T(-1) : value);
		return std::acos(clamped);
	}

	MML_FORCE_INLINE static T safe_asin(T value) {
		T clamped = value > T(1) ? T(1) : (value < T(-1) ? T(-1) : value);
		return std::asin(clamped);
	}

	MML_FORCE_INLINE static T atan(T value) {
		return std::atan(value);
	}

	MML_FORCE_INLINE static T atan2(T y, T x) {
		return std::atan2(y, x);
	}

	MML_FORCE_INLINE static T sinh(T value) {
		return std::sinh(value);
	}

	MML_FORCE_INLINE static T cosh(T value) {
		return std::cosh(value);
	}

	MML_FORCE_INLINE static T tanh(T value) {
		return std::tanh(value);
	}

	MML_FORCE_INLINE static T floor(T value) {
		return std::floor(value);
	}

	MML_FORCE_INLINE static T ceil(T value) {
		return std::ceil(value);
	}

	MML_FORCE_INLINE static T round(T value) {
		return std::round(value);
	}

	MML_FORCE_INLINE static T trunc(T value) {
		return std::trunc(value);
	}

	MML_FORCE_INLINE static T frac(T value) {
		return value - std::floor(value);
	}

	MML_FORCE_INLINE static T fmod(T x, T y) {
		return std::fmod(x, y);
	}

	MML_FORCE_INLINE static T sign(T value) {
		return value > T(0) ? T(1) : (value < T(0) ? T(-1) : T(0));
	}

	MML_FORCE_INLINE static T step(T edge, T value) {
		return value < edge ? T(0) : T(1);
	}

	MML_FORCE_INLINE static T smoothstep(T edge0, T edge1, T x) {
		T t = clamp((x - edge0) / (edge1 - edge0), T(0), T(1));
		return t * t * (T(3) - T(2) * t);
	}

	MML_FORCE_INLINE static T mix(T x, T y, T a) {
		return x * (T(1) - a) + y * a;
	}

	MML_FORCE_INLINE static constexpr T degrees_to_radians(T degrees) {
		return degrees * Constants<T>::pi / T(180);
	}

	MML_FORCE_INLINE static constexpr T radians_to_degrees(T radians) {
		return radians * T(180) / Constants<T>::pi;
	}

	MML_FORCE_INLINE static T normalize_angle(T radians) {
		T result = fmod(radians, Constants<T>::two_pi);
		if (result < T(0)) {
			result += Constants<T>::two_pi;
		}
		return result;
	}

	MML_FORCE_INLINE static T shortest_angle_distance(T from, T to) {
		T diff = normalize_angle(to - from);
		if (diff > Constants<T>::pi) {
			diff -= Constants<T>::two_pi;
		}
		return diff;
	}

	MML_FORCE_INLINE static T lerp_angle(T from, T to, T t) {
		return normalize_angle(from + shortest_angle_distance(from, to) * t);
	}

	MML_FORCE_INLINE static constexpr bool is_even(T value) {
		return (static_cast<long long>(value) & 1LL) == 0;
	}

	MML_FORCE_INLINE static constexpr bool is_odd(T value) {
		return !is_even(value);
	}

	MML_FORCE_INLINE static constexpr T repeat(T value, T length) {
		return value - floor(value / length) * length;
	}

	MML_FORCE_INLINE static constexpr T ping_pong(T value, T length) {
		T t = repeat(value, length * T(2));
		return length - abs(t - length);
	}

	MML_FORCE_INLINE static T damp(T current, T target, T smoothing, T dt) {
		return lerp(current, target, T(1) - pow(smoothing, dt));
	}

	MML_FORCE_INLINE static T damp_angle(T current, T target, T smoothing, T dt) {
		return lerp_angle(current, target, T(1) - pow(smoothing, dt));
	}

	MML_FORCE_INLINE static T map_range(T value, T in_min, T in_max, T out_min, T out_max) {
		return out_min + (out_max - out_min) * ((value - in_min) / (in_max - in_min));
	}

	MML_FORCE_INLINE static T approach(T current, T target, T max_delta) {
		T diff = target - current;
		return (abs(diff) <= max_delta) ? target : (current + sign(diff) * max_delta);
	}

	MML_FORCE_INLINE static T move_towards(T current, T target, T max_delta) {
		T diff = target - current;
		if (abs(diff) <= max_delta) {
			return target;
		}
		return current + sign(diff) * max_delta;
	}

	MML_FORCE_INLINE static T wrap(T value, T min_val, T max_val) {
		T range = max_val - min_val;
		if (range == T(0)) {
			return min_val;
		}
		return min_val + fmod(value - min_val, range + range);
	}

	MML_FORCE_INLINE static bool approximately(T a, T b, T epsilon = T(0.0001)) {
		return abs(a - b) < epsilon;
	}

	MML_FORCE_INLINE static bool is_nan(T value) {
		if constexpr (std::is_floating_point_v<T>) {
			return value != value;
		}
		return false;
	}

	MML_FORCE_INLINE static bool is_inf(T value) {
		if constexpr (std::is_floating_point_v<T>) {
			return value > Constants<T>::infinity || value < -Constants<T>::infinity;
		}
		return false;
	}

	MML_FORCE_INLINE static bool is_finite(T value) {
		if constexpr (std::is_floating_point_v<T>) {
			return !is_nan(value) && !is_inf(value);
		}
		return true;
	}
};

using Mathf = Math<float>;
using Mathd = Math<double>;
using Mathi = Math<int>;

} //namespace mml