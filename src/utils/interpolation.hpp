/**************************************************************************/
/*  interpolation.hpp                                                     */
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

// Interpolation utilities
template <typename T>
struct Interpolation {
	static_assert(std::is_floating_point_v<T>, "Interpolation requires floating-point type");

	// Linear interpolation
	MML_FORCE_INLINE static T lerp(T a, T b, T t) {
		return a + (b - a) * t;
	}

	// Inverse linear interpolation (find t given a, b, and value)
	MML_FORCE_INLINE static T inverse_lerp(T a, T b, T value) {
		if (a == b) {
			return T(0);
		}
		return (value - a) / (b - a);
	}

	// Clamp value between min and max
	MML_FORCE_INLINE static T clamp(T value, T min_val, T max_val) {
		return value < min_val ? min_val : (value > max_val ? max_val : value);
	}

	// Clamp t to [0, 1] and perform lerp
	MML_FORCE_INLINE static T lerp_clamped(T a, T b, T t) {
		return lerp(a, b, clamp(t, T(0), T(1)));
	}

	// Smoothstep interpolation (smooth cubic Hermite)
	MML_FORCE_INLINE static T smoothstep(T a, T b, T t) {
		T x = clamp((t - a) / (b - a), T(0), T(1));
		return x * x * (T(3) - T(2) * x);
	}

	// Smootherstep interpolation (smoother than smoothstep)
	MML_FORCE_INLINE static T smootherstep(T a, T b, T t) {
		T x = clamp((t - a) / (b - a), T(0), T(1));
		return x * x * x * (x * (x * T(6) - T(15)) + T(10));
	}

	// Cosine interpolation
	MML_FORCE_INLINE static T cosine(T a, T b, T t) {
		T mu = (T(1) - std::cos(t * Constants<T>::pi)) * T(0.5);
		return lerp(a, b, mu);
	}

	// Cubic interpolation (Catmull-Rom spline)
	MML_FORCE_INLINE static T cubic(T p0, T p1, T p2, T p3, T t) {
		T t2 = t * t;
		T t3 = t2 * t;

		T a0 = p3 - p2 - p0 + p1;
		T a1 = p0 - p1 - a0;
		T a2 = p2 - p0;
		T a3 = p1;

		return a0 * t3 + a1 * t2 + a2 * t + a3;
	}

	// Hermite interpolation
	MML_FORCE_INLINE static T hermite(T p0, T m0, T p1, T m1, T t) {
		T t2 = t * t;
		T t3 = t2 * t;

		T h00 = T(2) * t3 - T(3) * t2 + T(1);
		T h10 = t3 - T(2) * t2 + t;
		T h01 = -T(2) * t3 + T(3) * t2;
		T h11 = t3 - t2;

		return h00 * p0 + h10 * m0 + h01 * p1 + h11 * m1;
	}

	// Bezier interpolation (quadratic)
	MML_FORCE_INLINE static T bezier_quadratic(T p0, T p1, T p2, T t) {
		T u = T(1) - t;
		return u * u * p0 + T(2) * u * t * p1 + t * t * p2;
	}

	// Bezier interpolation (cubic)
	MML_FORCE_INLINE static T bezier_cubic(T p0, T p1, T p2, T p3, T t) {
		T u = T(1) - t;
		T u2 = u * u;
		T u3 = u2 * u;
		T t2 = t * t;
		T t3 = t2 * t;

		return u3 * p0 + T(3) * u2 * t * p1 + T(3) * u * t2 * p2 + t3 * p3;
	}

	// Exponential ease in
	MML_FORCE_INLINE static T ease_in_expo(T t) {
		return t <= T(0) ? T(0) : std::pow(T(2), T(10) * (t - T(1)));
	}

	// Exponential ease out
	MML_FORCE_INLINE static T ease_out_expo(T t) {
		return t >= T(1) ? T(1) : T(1) - std::pow(T(2), -T(10) * t);
	}

	// Exponential ease in-out
	MML_FORCE_INLINE static T ease_in_out_expo(T t) {
		if (t <= T(0)) {
			return T(0);
		}
		if (t >= T(1)) {
			return T(1);
		}
		if (t < T(0.5)) {
			return std::pow(T(2), T(20) * t - T(10)) * T(0.5);
		} else {
			return (T(2) - std::pow(T(2), -T(20) * t + T(10))) * T(0.5);
		}
	}

	// Map value from one range to another
	MML_FORCE_INLINE static T remap(T value, T in_min, T in_max, T out_min, T out_max) {
		T t = inverse_lerp(in_min, in_max, value);
		return lerp(out_min, out_max, t);
	}

	// Map and clamp value from one range to another
	MML_FORCE_INLINE static T remap_clamped(T value, T in_min, T in_max, T out_min, T out_max) {
		T t = clamp(inverse_lerp(in_min, in_max, value), T(0), T(1));
		return lerp(out_min, out_max, t);
	}
};

// Type aliases
using Interpolationf = Interpolation<float>;
using Interpolationd = Interpolation<double>;

} // namespace mml