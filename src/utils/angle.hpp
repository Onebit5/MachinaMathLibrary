/**************************************************************************/
/*  angle.hpp                                                             */
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

#include "../core/config.hpp"
#include "../core/constants.hpp"
#include <cmath>
#include <type_traits>

namespace mml {

template <typename T>
struct Angle {
	static_assert(std::is_floating_point_v<T>, "Angle requires floating-point type");

	MML_FORCE_INLINE static constexpr T to_radians(T degrees) {
		return degrees * Constants<T>::pi / T(180);
	}

	MML_FORCE_INLINE static constexpr T to_degrees(T radians) {
		return radians * T(180) / Constants<T>::pi;
	}

	MML_FORCE_INLINE static T normalize(T radians) {
		T result = std::fmod(radians, Constants<T>::two_pi);
		if (result < T(0)) {
			result += Constants<T>::two_pi;
		}
		return result;
	}

	MML_FORCE_INLINE static T normalize_signed(T radians) {
		T result = normalize(radians);
		if (result >= Constants<T>::pi) {
			result -= Constants<T>::two_pi;
		}
		return result;
	}

	MML_FORCE_INLINE static T shortest_distance(T from, T to) {
		T diff = normalize_signed(to - from);
		return diff;
	}

	MML_FORCE_INLINE static T lerp(T from, T to, T t) {
		T diff = shortest_distance(from, to);
		return normalize(from + diff * t);
	}
};

using Anglef = Angle<float>;
using Angled = Angle<double>;

} //namespace mml