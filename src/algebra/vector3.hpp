/**************************************************************************/
/*  vector3.hpp                                                           */
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
#include "../core/epsilon.hpp"
#include "../utils/math.hpp"
#include <cmath>
#include <type_traits>

namespace mml {

// Three-dimensional vector
template <typename T>
struct Vector3 {
	static_assert(std::is_floating_point_v<T>, "Vector3 requires floating-point type");

	T x, y, z;

	// Default constructor
	Vector3() = default;

	// Explicit value constructor
	explicit constexpr Vector3(T scalar) :
			x(scalar), y(scalar), z(scalar) {}

	// Explicit component constructor
	explicit constexpr Vector3(T x_, T y_, T z_) :
			x(x_), y(y_), z(z_) {}

	// Explicit copy constructor
	constexpr Vector3(const Vector3 &other) = default;
	Vector3 &operator=(const Vector3 &other) = default;

	// Element access
	MML_FORCE_INLINE T &operator[](size_t i) { return (&x)[i]; }
	MML_FORCE_INLINE const T &operator[](size_t i) const { return (&x)[i]; }

	// Addition
	[[nodiscard]] constexpr MML_FORCE_INLINE Vector3 operator+(const Vector3 &rhs) const {
		return Vector3(x + rhs.x, y + rhs.y, z + rhs.z);
	}

	constexpr MML_FORCE_INLINE Vector3 &operator+=(const Vector3 &rhs) {
		x += rhs.x;
		y += rhs.y;
		z += rhs.z;
		return *this;
	}

	// Subtraction
	[[nodiscard]] constexpr MML_FORCE_INLINE Vector3 operator-(const Vector3 &rhs) const {
		return Vector3(x - rhs.x, y - rhs.y, z - rhs.z);
	}

	constexpr MML_FORCE_INLINE Vector3 &operator-=(const Vector3 &rhs) {
		x -= rhs.x;
		y -= rhs.y;
		z -= rhs.z;
		return *this;
	}

	// Scalar multiplication
	[[nodiscard]] constexpr MML_FORCE_INLINE Vector3 operator*(T scalar) const {
		return Vector3(x * scalar, y * scalar, z * scalar);
	}

	constexpr MML_FORCE_INLINE Vector3 &operator*=(T scalar) {
		x *= scalar;
		y *= scalar;
		z *= scalar;
		return *this;
	}

	// Scalar division
	[[nodiscard]] constexpr MML_FORCE_INLINE Vector3 operator/(T scalar) const {
		T inv = T(1) / scalar;
		return Vector3(x * inv, y * inv, z * inv);
	}

	constexpr MML_FORCE_INLINE Vector3 &operator/=(T scalar) {
		T inv = T(1) / scalar;
		x *= inv;
		y *= inv;
		z *= inv;
		return *this;
	}

	// Unary negation
	[[nodiscard]] constexpr MML_FORCE_INLINE Vector3 operator-() const {
		return Vector3(-x, -y, -z);
	}

	// Equality comparison
	[[nodiscard]] constexpr MML_FORCE_INLINE bool operator==(const Vector3 &rhs) const {
		return Epsilon<T>::equal(x, rhs.x) &&
				Epsilon<T>::equal(y, rhs.y) &&
				Epsilon<T>::equal(z, rhs.z);
	}

	[[nodiscard]] constexpr MML_FORCE_INLINE bool operator!=(const Vector3 &rhs) const {
		return !(*this == rhs);
	}

	// Dot product
	[[nodiscard]] constexpr MML_FORCE_INLINE T dot(const Vector3 &rhs) const {
		return x * rhs.x + y * rhs.y + z * rhs.z;
	}

	// Cross product (right-handed coordinate system)
	[[nodiscard]] constexpr MML_FORCE_INLINE Vector3 cross(const Vector3 &rhs) const {
		return Vector3(
				y * rhs.z - z * rhs.y, // x component: y1*z2 - z1*y2
				z * rhs.x - x * rhs.z, // y component: z1*x2 - x1*z2
				x * rhs.y - y * rhs.x); // z component: x1*y2 - y1*x2
	}

	// Length squared
	[[nodiscard]] constexpr MML_FORCE_INLINE T length_squared() const {
		return x * x + y * y + z * z;
	}

	// Length
	[[nodiscard]] MML_FORCE_INLINE T length() const {
		return std::sqrt(length_squared());
	}

	// Normalize (returns normalized copy)
	[[nodiscard]] MML_FORCE_INLINE Vector3 normalized() const {
		T len = length();
		if (Epsilon<T>::approx_zero(len)) {
			return Vector3(T(0), T(0), T(0));
		}
		return *this / len;
	}

	// Normalize in place
	MML_FORCE_INLINE Vector3 &normalize() {
		T len = length();
		if (!Epsilon<T>::approx_zero(len)) {
			*this /= len;
		}
		return *this;
	}

	// Check if vector is zero (near zero length)
	[[nodiscard]] MML_FORCE_INLINE bool is_zero() const {
		return Epsilon<T>::approx_zero(length_squared());
	}

	// Check if vector is unit length
	[[nodiscard]] MML_FORCE_INLINE bool is_unit() const {
		return Epsilon<T>::equal(length_squared(), T(1));
	}

	// Check if all components are finite
	[[nodiscard]] MML_FORCE_INLINE bool is_finite() const {
		return std::isfinite(x) && std::isfinite(y) && std::isfinite(z);
	}

	// Distance to another vector
	[[nodiscard]] MML_FORCE_INLINE T distance(const Vector3 &rhs) const {
		return (*this - rhs).length();
	}

	// Distance squared to another vector
	[[nodiscard]] constexpr MML_FORCE_INLINE T distance_squared(const Vector3 &rhs) const {
		return (*this - rhs).length_squared();
	}

	// Reflect vector across normal
	[[nodiscard]] constexpr MML_FORCE_INLINE Vector3 reflect(const Vector3 &normal) const {
		return *this - normal * (T(2) * this->dot(normal));
	}

	// Refract vector through normal with eta ratio (Snell's law)
	[[nodiscard]] MML_FORCE_INLINE Vector3 refract(const Vector3 &normal, T eta) const {
		T dot_n = this->dot(normal);
		T k = T(1) - eta * eta * (T(1) - dot_n * dot_n);
		if (k < T(0)) {
			return Vector3(T(0), T(0), T(0)); // Total internal reflection
		}
		return *this * eta - normal * (eta * dot_n + std::sqrt(k));
	}

	// Project this vector onto another
	[[nodiscard]] constexpr MML_FORCE_INLINE Vector3 project(const Vector3 &onto) const {
		T onto_len_sq = onto.length_squared();
		if (Epsilon<T>::approx_zero(onto_len_sq)) {
			return Vector3(T(0), T(0), T(0));
		}
		return onto * (this->dot(onto) / onto_len_sq);
	}

	// Component-wise multiplication (Hadamard product)
	[[nodiscard]] constexpr MML_FORCE_INLINE Vector3 hadamard(const Vector3 &rhs) const {
		return Vector3(x * rhs.x, y * rhs.y, z * rhs.z);
	}

	// Component-wise division
	[[nodiscard]] constexpr MML_FORCE_INLINE Vector3 hadamard_div(const Vector3 &rhs) const {
		return Vector3(x / rhs.x, y / rhs.y, z / rhs.z);
	}

	// Get angle between this vector and another (in radians)
	[[nodiscard]] MML_FORCE_INLINE T angle(const Vector3 &rhs) const {
		T len_sq = length_squared() * rhs.length_squared();
		if (Epsilon<T>::approx_zero(len_sq)) {
			return T(0);
		}
		T dot_val = dot(rhs);
		T value = dot_val / Math<T>::safe_sqrt(len_sq);
		return Math<T>::safe_acos(value);
	}

	// Linear interpolation between this vector and another
	[[nodiscard]] constexpr MML_FORCE_INLINE Vector3 lerp(const Vector3 &rhs, T t) const {
		return Vector3(
				x + (rhs.x - x) * t,
				y + (rhs.y - y) * t,
				z + (rhs.z - z) * t);
	}

	// Get triple scalar product (a . (b x c))
	[[nodiscard]] static constexpr MML_FORCE_INLINE T triple_scalar(const Vector3 &a, const Vector3 &b, const Vector3 &c) {
		return a.dot(b.cross(c)); // volume of parallelepiped
	}

	// Get triple vector product (a x (b x c))
	[[nodiscard]] static constexpr MML_FORCE_INLINE Vector3 triple_vector(const Vector3 &a, const Vector3 &b, const Vector3 &c) {
		return b * a.dot(c) - c * a.dot(b); // BAC-CAB rule
	}

	// Get minimum components
	[[nodiscard]] constexpr MML_FORCE_INLINE Vector3 min(const Vector3 &rhs) const {
		return Vector3(x < rhs.x ? x : rhs.x, y < rhs.y ? y : rhs.y, z < rhs.z ? z : rhs.z);
	}

	// Get maximum components
	[[nodiscard]] constexpr MML_FORCE_INLINE Vector3 max(const Vector3 &rhs) const {
		return Vector3(x > rhs.x ? x : rhs.x, y > rhs.y ? y : rhs.y, z > rhs.z ? z : rhs.z);
	}

	// Clamp components between min and max
	[[nodiscard]] constexpr MML_FORCE_INLINE Vector3 clamp(T min_val, T max_val) const {
		T clamped_x = x < min_val ? min_val : (x > max_val ? max_val : x);
		T clamped_y = y < min_val ? min_val : (y > max_val ? max_val : y);
		T clamped_z = z < min_val ? min_val : (z > max_val ? max_val : z);
		return Vector3(clamped_x, clamped_y, clamped_z);
	}

	// Clamp components between two vectors
	[[nodiscard]] constexpr MML_FORCE_INLINE Vector3 clamp(const Vector3 &min_vec, const Vector3 &max_vec) const {
		T clamped_x = x < min_vec.x ? min_vec.x : (x > max_vec.x ? max_vec.x : x);
		T clamped_y = y < min_vec.y ? min_vec.y : (y > max_vec.y ? max_vec.y : y);
		T clamped_z = z < min_vec.z ? min_vec.z : (z > max_vec.z ? max_vec.z : z);
		return Vector3(clamped_x, clamped_y, clamped_z);
	}

	// Get absolute value of components
	[[nodiscard]] constexpr MML_FORCE_INLINE Vector3 abs() const {
		return Vector3(x < T(0) ? -x : x, y < T(0) ? -y : y, z < T(0) ? -z : z);
	}

	// Get sign of components (-1, 0, or 1)
	[[nodiscard]] constexpr MML_FORCE_INLINE Vector3 sign() const {
		return Vector3(
				x > T(0) ? T(1) : (x < T(0) ? T(-1) : T(0)),
				y > T(0) ? T(1) : (y < T(0) ? T(-1) : T(0)),
				z > T(0) ? T(1) : (z < T(0) ? T(-1) : T(0)));
	}

	// Get floor of components
	[[nodiscard]] MML_FORCE_INLINE Vector3 floor() const {
		return Vector3(std::floor(x), std::floor(y), std::floor(z));
	}

	// Get ceil of components
	[[nodiscard]] MML_FORCE_INLINE Vector3 ceil() const {
		return Vector3(std::ceil(x), std::ceil(y), std::ceil(z));
	}

	// Get rounded components
	[[nodiscard]] MML_FORCE_INLINE Vector3 round() const {
		return Vector3(std::round(x), std::round(y), std::round(z));
	}

	// Get cross product of two vectors and normalize result
	[[nodiscard]] static MML_FORCE_INLINE Vector3 cross_normalized(const Vector3 &a, const Vector3 &b) {
		return a.cross(b).normalized();
	}

	// Spherical linear interpolation
	[[nodiscard]] static MML_FORCE_INLINE Vector3 slerp(const Vector3 &a, const Vector3 &b, T t) {
		T theta = a.angle(b);
		if (Epsilon<T>::approx_zero(theta)) {
			return a.lerp(b, t);
		}
		T sin_theta = std::sin(theta);
		T wa = std::sin((T(1) - t) * theta) / sin_theta;
		T wb = std::sin(t * theta) / sin_theta;
		return a * wa + b * wb;
	}

	// Static zero vector
	[[nodiscard]] static constexpr Vector3 zero() {
		return Vector3(T(0), T(0), T(0));
	}

	// Static unit X vector
	[[nodiscard]] static constexpr Vector3 unit_x() {
		return Vector3(T(1), T(0), T(0));
	}

	// Static unit Y vector
	[[nodiscard]] static constexpr Vector3 unit_y() {
		return Vector3(T(0), T(1), T(0));
	}

	// Static unit Z vector
	[[nodiscard]] static constexpr Vector3 unit_z() {
		return Vector3(T(0), T(0), T(1));
	}

	// Static up vector
	[[nodiscard]] static constexpr Vector3 up() {
		return Vector3(T(0), T(1), T(0));
	}

	// Static down vector
	[[nodiscard]] static constexpr Vector3 down() {
		return Vector3(T(0), T(-1), T(0));
	}

	// Static forward vector
	[[nodiscard]] static constexpr Vector3 forward() {
		return Vector3(T(0), T(0), T(1));
	}

	// Static back vector
	[[nodiscard]] static constexpr Vector3 back() {
		return Vector3(T(0), T(0), T(-1));
	}

	// Static right vector
	[[nodiscard]] static constexpr Vector3 right() {
		return Vector3(T(1), T(0), T(0));
	}

	// Static left vector
	[[nodiscard]] static constexpr Vector3 left() {
		return Vector3(T(-1), T(0), T(0));
	}

	// Static one vector
	[[nodiscard]] static constexpr Vector3 one() {
		return Vector3(T(1), T(1), T(1));
	}

	// Distance between two vectors
	[[nodiscard]] static MML_FORCE_INLINE T distance(const Vector3 &a, const Vector3 &b) {
		return a.distance(b);
	}

	// Cross product of two vectors
	[[nodiscard]] static constexpr MML_FORCE_INLINE Vector3 cross(const Vector3 &a, const Vector3 &b) {
		return a.cross(b);
	}

	// Dot product of two vectors
	[[nodiscard]] static constexpr MML_FORCE_INLINE T dot(const Vector3 &a, const Vector3 &b) {
		return a.dot(b);
	}

	// Angle between two vectors (in radians)
	[[nodiscard]] static MML_FORCE_INLINE T angle_between(const Vector3 &a, const Vector3 &b) {
		return a.angle(b);
	}

	// Linear interpolation between two vectors
	[[nodiscard]] static constexpr MML_FORCE_INLINE Vector3 lerp(const Vector3 &a, const Vector3 &b, T t) {
		return a.lerp(b, t);
	}

	// Minimum of two vectors
	[[nodiscard]] static constexpr MML_FORCE_INLINE Vector3 min(const Vector3 &a, const Vector3 &b) {
		return a.min(b);
	}

	// Maximum of two vectors
	[[nodiscard]] static constexpr MML_FORCE_INLINE Vector3 max(const Vector3 &a, const Vector3 &b) {
		return a.max(b);
	}
};

// Non-member scalar multiplication (scalar * vector)
template <typename T>
[[nodiscard]] MML_FORCE_INLINE Vector3<T> operator*(T scalar, const Vector3<T> &vec) {
	return vec * scalar;
}

// Type aliases
using Vector3f = Vector3<float>;
using Vector3d = Vector3<double>;

} // namespace mml