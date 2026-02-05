/**************************************************************************/
/*  random.hpp                                                            */
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

#include "../algebra/vector2.hpp"
#include "../algebra/vector3.hpp"
#include "../core/config.hpp"
#include "../core/constants.hpp"
#include "../utils/math.hpp"
#include <cmath>
#include <limits>
#include <random>
#include <type_traits>

namespace mml {

template <typename T>
struct Random {
	static_assert(std::is_arithmetic_v<T>, "Random requires arithmetic type");

private:
	// Thread-local random engine to avoid global state issues
	static std::mt19937 &engine() {
		thread_local std::mt19937 rng(std::random_device{}());
		return rng;
	}

	// Uniform distribution for [0, 1] range
	static std::uniform_real_distribution<T> &dist() {
		thread_local std::uniform_real_distribution<T> d(T(0), T(1));
		return d;
	}

public:
	MML_FORCE_INLINE static void seed(unsigned int value) {
		engine().seed(value);
	}

	MML_FORCE_INLINE static void seed_time() {
		std::random_device rd;
		engine().seed(rd());
	}

	MML_FORCE_INLINE static T range(T min_val, T max_val) {
		if constexpr (std::is_integral_v<T>) {
			std::uniform_int_distribution<T> d(min_val, max_val);
			return d(engine());
		} else {
			std::uniform_real_distribution<T> d(min_val, max_val);
			return d(engine());
		}
	}

	MML_FORCE_INLINE static T range_inclusive(T min_val, T max_val) {
		return range(min_val, max_val);
	}

	MML_FORCE_INLINE static int range_int(int min_val, int max_val) {
		std::uniform_int_distribution<int> d(min_val, max_val);
		return d(engine());
	}

	MML_FORCE_INLINE static T value() {
		if constexpr (std::is_integral_v<T>) {
			return range(T(0), std::numeric_limits<T>::max());
		} else {
			return dist()(engine());
		}
	}

	MML_FORCE_INLINE static T value_centered() {
		return value() * T(2) - T(1);
	}

	MML_FORCE_INLINE static bool chance(T probability) {
		return value() < probability;
	}

	MML_FORCE_INLINE static T normal(T mean = T(0), T std_dev = T(1)) {
		std::normal_distribution<T> d(mean, std_dev);
		return d(engine());
	}

	MML_FORCE_INLINE static Vector2<T> inside_unit_circle() {
		std::uniform_real_distribution<T> d(T(0), T(1));
		T angle = d(engine()) * Constants<T>::two_pi;
		T radius = std::sqrt(d(engine()));
		return Vector2<T>(std::cos(angle) * radius, std::sin(angle) * radius);
	}

	MML_FORCE_INLINE static Vector2<T> on_unit_circle() {
		std::uniform_real_distribution<T> d(T(0), T(1));
		T angle = d(engine()) * Constants<T>::two_pi;
		return Vector2<T>(std::cos(angle), std::sin(angle));
	}

	MML_FORCE_INLINE static Vector3<T> inside_unit_sphere() {
		std::uniform_real_distribution<T> d(T(0), T(1));
		T theta = d(engine()) * Constants<T>::two_pi;
		T phi = Math<T>::safe_acos(T(1) - d(engine()) * T(2));
		T radius = std::cbrt(d(engine()));

		T x = radius * std::sin(phi) * std::cos(theta);
		T y = radius * std::sin(phi) * std::sin(theta);
		T z = radius * std::cos(phi);

		return Vector3<T>(x, y, z);
	}

	MML_FORCE_INLINE static Vector3<T> on_unit_sphere() {
		std::uniform_real_distribution<T> d(T(0), T(1));
		T theta = d(engine()) * Constants<T>::two_pi;
		T phi = Math<T>::safe_acos(T(1) - d(engine()) * T(2));

		T x = std::sin(phi) * std::cos(theta);
		T y = std::sin(phi) * std::sin(theta);
		T z = std::cos(phi);

		return Vector3<T>(x, y, z);
	}

	MML_FORCE_INLINE static Vector2<T> range_2d(const Vector2<T> &min_val, const Vector2<T> &max_val) {
		return Vector2<T>(range(min_val.x, max_val.x), range(min_val.y, max_val.y));
	}

	MML_FORCE_INLINE static Vector3<T> range_3d(const Vector3<T> &min_val, const Vector3<T> &max_val) {
		return Vector3<T>(range(min_val.x, max_val.x), range(min_val.y, max_val.y), range(min_val.z, max_val.z));
	}

	MML_FORCE_INLINE static Vector3<T> inside_unit_cube() {
		return Vector3<T>(value(), value(), value());
	}

	MML_FORCE_INLINE static Vector3<T> on_unit_cube() {
		std::uniform_int_distribution<int> d(0, 1);
		return Vector3<T>(d(engine()) ? T(1) : T(-1), d(engine()) ? T(1) : T(-1), d(engine()) ? T(1) : T(-1));
	}

	MML_FORCE_INLINE static T perlin(T x, T y, T z) {
		static const T persistence = T(0.5);
		static const int octaves = 4;
		T total = T(0);
		T frequency = T(1);
		T amplitude = T(1);
		T maxValue = T(0);

		for (int i = 0; i < octaves; ++i) {
			total += std::sin(x * frequency + y * frequency + z * frequency) * amplitude;
			maxValue += amplitude;
			amplitude *= persistence;
			frequency *= T(2);
		}

		return total / maxValue;
	}
};

using Randomf = Random<float>;
using Randomd = Random<double>;
using Randomi = Random<int>;

} // namespace mml