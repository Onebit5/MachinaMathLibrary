/**************************************************************************/
/*  vector2.hpp                                                           */
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
#include "../core/epsilon.hpp"
#include "../utils/math.hpp"
#include <cmath>
#include <type_traits>

namespace mml {

template <typename T>
struct Vector2 {
	static_assert(std::is_floating_point_v<T>, "Vector2 requires floating-point type");

	T x, y;

	Vector2() = default;

	explicit constexpr Vector2(T scalar) :
			x(scalar), y(scalar) {}

	explicit constexpr Vector2(T x_, T y_) :
			x(x_), y(y_) {}

	constexpr Vector2(const Vector2 &other) = default;
	Vector2 &operator=(const Vector2 &other) = default;

	MML_FORCE_INLINE T &operator[](size_t i) { return (&x)[i]; }
	MML_FORCE_INLINE const T &operator[](size_t i) const { return (&x)[i]; }

	[[nodiscard]] constexpr MML_FORCE_INLINE Vector2 operator+(const Vector2 &rhs) const {
		return Vector2(x + rhs.x, y + rhs.y);
	}

	constexpr MML_FORCE_INLINE Vector2 &operator+=(const Vector2 &rhs) {
		x += rhs.x;
		y += rhs.y;
		return *this;
	}

	[[nodiscard]] constexpr MML_FORCE_INLINE Vector2 operator-(const Vector2 &rhs) const {
		return Vector2(x - rhs.x, y - rhs.y);
	}

	constexpr MML_FORCE_INLINE Vector2 &operator-=(const Vector2 &rhs) {
		x -= rhs.x;
		y -= rhs.y;
		return *this;
	}

	[[nodiscard]] constexpr MML_FORCE_INLINE Vector2 operator*(T scalar) const {
		return Vector2(x * scalar, y * scalar);
	}

	constexpr MML_FORCE_INLINE Vector2 &operator*=(T scalar) {
		x *= scalar;
		y *= scalar;
		return *this;
	}

	[[nodiscard]] constexpr MML_FORCE_INLINE Vector2 operator/(T scalar) const {
		T inv = T(1) / scalar;
		return Vector2(x * inv, y * inv);
	}

	constexpr MML_FORCE_INLINE Vector2 &operator/=(T scalar) {
		T inv = T(1) / scalar;
		x *= inv;
		y *= inv;
		return *this;
	}

	[[nodiscard]] constexpr MML_FORCE_INLINE Vector2 operator-() const {
		return Vector2(-x, -y);
	}

	[[nodiscard]] constexpr MML_FORCE_INLINE bool operator==(const Vector2 &rhs) const {
		return Epsilon<T>::equal(x, rhs.x) && Epsilon<T>::equal(y, rhs.y);
	}

	[[nodiscard]] constexpr MML_FORCE_INLINE bool operator!=(const Vector2 &rhs) const {
		return !(*this == rhs);
	}

	[[nodiscard]] constexpr MML_FORCE_INLINE T dot(const Vector2 &rhs) const {
		return x * rhs.x + y * rhs.y;
	}

	[[nodiscard]] constexpr MML_FORCE_INLINE T length_squared() const {
		return x * x + y * y;
	}

	[[nodiscard]] MML_FORCE_INLINE T length() const {
		return std::sqrt(length_squared());
	}

	[[nodiscard]] MML_FORCE_INLINE Vector2 normalized() const {
		T len = length();
		if (Epsilon<T>::approx_zero(len)) {
			return Vector2(T(0), T(0));
		}
		return *this / len;
	}

	MML_FORCE_INLINE Vector2 &normalize() {
		T len = length();
		if (!Epsilon<T>::approx_zero(len)) {
			*this /= len;
		}
		return *this;
	}

	[[nodiscard]] MML_FORCE_INLINE bool is_zero() const {
		return Epsilon<T>::approx_zero(length_squared());
	}

	[[nodiscard]] MML_FORCE_INLINE bool is_unit() const {
		return Epsilon<T>::equal(length_squared(), T(1));
	}

	[[nodiscard]] MML_FORCE_INLINE bool is_finite() const {
		return std::isfinite(x) && std::isfinite(y);
	}

	[[nodiscard]] MML_FORCE_INLINE T distance(const Vector2 &rhs) const {
		return (*this - rhs).length();
	}

	[[nodiscard]] MML_FORCE_INLINE T distance_squared(const Vector2 &rhs) const {
		return (*this - rhs).length_squared();
	}

	[[nodiscard]] MML_FORCE_INLINE T angle(const Vector2 &rhs) const {
		T len_sq = length_squared() * rhs.length_squared();
		if (Epsilon<T>::approx_zero(len_sq)) {
			return T(0);
		}
		T dot_val = dot(rhs);
		T value = dot_val / Math<T>::safe_sqrt(len_sq);
		return Math<T>::safe_acos(value);
	}

	[[nodiscard]] constexpr MML_FORCE_INLINE Vector2 lerp(const Vector2 &rhs, T t) const {
		return Vector2(
				x + (rhs.x - x) * t,
				y + (rhs.y - y) * t);
	}

	[[nodiscard]] constexpr MML_FORCE_INLINE Vector2 perpendicular() const {
		return Vector2(-y, x);
	}

	[[nodiscard]] constexpr MML_FORCE_INLINE Vector2 perpendicular_cw() const {
		return Vector2(y, -x);
	}

	[[nodiscard]] constexpr MML_FORCE_INLINE Vector2 reflect(const Vector2 &normal) const {
		return *this - normal * (T(2) * this->dot(normal));
	}

	[[nodiscard]] MML_FORCE_INLINE Vector2 refract(const Vector2 &normal, T eta) const {
		T dot_n = this->dot(normal);
		T k = T(1) - eta * eta * (T(1) - dot_n * dot_n);
		if (k < T(0)) {
			return Vector2(T(0), T(0));
		}
		return *this * eta - normal * (eta * dot_n + std::sqrt(k));
	}

	[[nodiscard]] constexpr MML_FORCE_INLINE Vector2 project(const Vector2 &onto) const {
		T onto_len_sq = onto.length_squared();
		if (Epsilon<T>::approx_zero(onto_len_sq)) {
			return Vector2(T(0), T(0));
		}
		return onto * (this->dot(onto) / onto_len_sq);
	}

	[[nodiscard]] constexpr MML_FORCE_INLINE Vector2 hadamard(const Vector2 &rhs) const {
		return Vector2(x * rhs.x, y * rhs.y);
	}

	[[nodiscard]] constexpr MML_FORCE_INLINE Vector2 hadamard_div(const Vector2 &rhs) const {
		return Vector2(x / rhs.x, y / rhs.y);
	}

	[[nodiscard]] constexpr MML_FORCE_INLINE T cross(const Vector2 &rhs) const {
		return x * rhs.y - y * rhs.x;
	}

	[[nodiscard]] constexpr MML_FORCE_INLINE Vector2 min(const Vector2 &rhs) const {
		return Vector2(x < rhs.x ? x : rhs.x, y < rhs.y ? y : rhs.y);
	}

	[[nodiscard]] constexpr MML_FORCE_INLINE Vector2 max(const Vector2 &rhs) const {
		return Vector2(x > rhs.x ? x : rhs.x, y > rhs.y ? y : rhs.y);
	}

	[[nodiscard]] constexpr MML_FORCE_INLINE Vector2 clamp(T min_val, T max_val) const {
		T clamped_x = x < min_val ? min_val : (x > max_val ? max_val : x);
		T clamped_y = y < min_val ? min_val : (y > max_val ? max_val : y);
		return Vector2(clamped_x, clamped_y);
	}

	[[nodiscard]] constexpr MML_FORCE_INLINE Vector2 clamp(const Vector2 &min_vec, const Vector2 &max_vec) const {
		T clamped_x = x < min_vec.x ? min_vec.x : (x > max_vec.x ? max_vec.x : x);
		T clamped_y = y < min_vec.y ? min_vec.y : (y > max_vec.y ? max_vec.y : y);
		return Vector2(clamped_x, clamped_y);
	}

	[[nodiscard]] constexpr MML_FORCE_INLINE Vector2 abs() const {
		return Vector2(x < T(0) ? -x : x, y < T(0) ? -y : y);
	}

	[[nodiscard]] constexpr MML_FORCE_INLINE Vector2 sign() const {
		return Vector2(
				x > T(0) ? T(1) : (x < T(0) ? T(-1) : T(0)),
				y > T(0) ? T(1) : (y < T(0) ? T(-1) : T(0)));
	}

	[[nodiscard]] MML_FORCE_INLINE Vector2 floor() const {
		return Vector2(std::floor(x), std::floor(y));
	}

	[[nodiscard]] MML_FORCE_INLINE Vector2 ceil() const {
		return Vector2(std::ceil(x), std::ceil(y));
	}

	[[nodiscard]] MML_FORCE_INLINE Vector2 round() const {
		return Vector2(std::round(x), std::round(y));
	}

	[[nodiscard]] static MML_FORCE_INLINE Vector2 slerp(const Vector2 &a, const Vector2 &b, T t) {
		T theta = a.angle(b);
		if (Epsilon<T>::approx_zero(theta)) {
			return a.lerp(b, t);
		}
		T sin_theta = std::sin(theta);
		T wa = std::sin((T(1) - t) * theta) / sin_theta;
		T wb = std::sin(t * theta) / sin_theta;
		return a * wa + b * wb;
	}

	[[nodiscard]] static constexpr Vector2 zero() {
		return Vector2(T(0), T(0));
	}

	[[nodiscard]] static constexpr Vector2 unit_x() {
		return Vector2(T(1), T(0));
	}

	[[nodiscard]] static constexpr Vector2 unit_y() {
		return Vector2(T(0), T(1));
	}

	[[nodiscard]] static constexpr Vector2 one() {
		return Vector2(T(1), T(1));
	}

	[[nodiscard]] static constexpr MML_FORCE_INLINE T distance(const Vector2 &a, const Vector2 &b) {
		return a.distance(b);
	}

	[[nodiscard]] static constexpr MML_FORCE_INLINE T dot(const Vector2 &a, const Vector2 &b) {
		return a.dot(b);
	}

	[[nodiscard]] static MML_FORCE_INLINE T angle_between(const Vector2 &a, const Vector2 &b) {
		return a.angle(b);
	}

	[[nodiscard]] static constexpr MML_FORCE_INLINE Vector2 lerp(const Vector2 &a, const Vector2 &b, T t) {
		return a.lerp(b, t);
	}

	[[nodiscard]] static constexpr MML_FORCE_INLINE Vector2 min(const Vector2 &a, const Vector2 &b) {
		return a.min(b);
	}

	[[nodiscard]] static constexpr MML_FORCE_INLINE Vector2 max(const Vector2 &a, const Vector2 &b) {
		return a.max(b);
	}
};

template <typename T>
[[nodiscard]] MML_FORCE_INLINE Vector2<T> operator*(T scalar, const Vector2<T> &vec) {
	return vec * scalar;
}

using Vector2f = Vector2<float>;
using Vector2d = Vector2<double>;

} //namespace mml