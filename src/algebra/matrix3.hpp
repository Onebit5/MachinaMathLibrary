/**************************************************************************/
/*  matrix3.hpp                                                           */
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
#include "vector3.hpp"
#include <cmath>
#include <type_traits>

namespace mml {

// 3x3 matrix (column-major storage)
template <typename T>
struct Matrix3 {
	static_assert(std::is_floating_point_v<T>, "Matrix3 requires floating-point type");

	// Column vectors
	Vector3<T> cols[3];

	// Default constructor
	constexpr Matrix3() = default;

	// Explicit identity constructor
	[[nodiscard]] static constexpr Matrix3 identity() {
		Matrix3 m;
		m.cols[0] = Vector3<T>(T(1), T(0), T(0));
		m.cols[1] = Vector3<T>(T(0), T(1), T(0));
		m.cols[2] = Vector3<T>(T(0), T(0), T(1));
		return m;
	}

	// Explicit zero matrix
	[[nodiscard]] static constexpr Matrix3 zero() {
		Matrix3 m;
		m.cols[0] = Vector3<T>(T(0), T(0), T(0));
		m.cols[1] = Vector3<T>(T(0), T(0), T(0));
		m.cols[2] = Vector3<T>(T(0), T(0), T(0));
		return m;
	}

	// Explicit constructor from three column vectors
	explicit constexpr Matrix3(const Vector3<T> &c0, const Vector3<T> &c1, const Vector3<T> &c2) {
		cols[0] = c0;
		cols[1] = c1;
		cols[2] = c2;
	}

	// Explicit constructor from nine components (column-major order)
	explicit constexpr Matrix3(
			T m00, T m10, T m20, // first column
			T m01, T m11, T m21, // second column
			T m02, T m12, T m22 // third column
	) {
		cols[0] = Vector3<T>(m00, m10, m20);
		cols[1] = Vector3<T>(m01, m11, m21);
		cols[2] = Vector3<T>(m02, m12, m22);
	}

	// Explicit copy constructor
	constexpr Matrix3(const Matrix3 &other) = default;
	Matrix3 &operator=(const Matrix3 &other) = default;

	// Column access
	constexpr MML_FORCE_INLINE Vector3<T> &operator[](size_t i) { return cols[i]; }
	constexpr MML_FORCE_INLINE const Vector3<T> &operator[](size_t i) const { return cols[i]; }

	// Matrix-matrix multiplication
	[[nodiscard]] constexpr MML_FORCE_INLINE Matrix3 operator*(const Matrix3 &rhs) const {
		Matrix3 result;
		for (size_t col = 0; col < 3; ++col) {
			for (size_t row = 0; row < 3; ++row) {
				result[col][row] =
						cols[0][row] * rhs[col][0] +
						cols[1][row] * rhs[col][1] +
						cols[2][row] * rhs[col][2];
			}
		}
		return result;
	}

	// Matrix-vector multiplication (column-major)
	[[nodiscard]] constexpr MML_FORCE_INLINE Vector3<T> operator*(const Vector3<T> &v) const {
		return Vector3<T>(
				cols[0].x * v.x + cols[1].x * v.y + cols[2].x * v.z,
				cols[0].y * v.x + cols[1].y * v.y + cols[2].y * v.z,
				cols[0].z * v.x + cols[1].z * v.y + cols[2].z * v.z);
	}

	// Matrix addition
	[[nodiscard]] constexpr MML_FORCE_INLINE Matrix3 operator+(const Matrix3 &rhs) const {
		return Matrix3(
				cols[0] + rhs.cols[0],
				cols[1] + rhs.cols[1],
				cols[2] + rhs.cols[2]);
	}

	// Matrix subtraction
	[[nodiscard]] constexpr MML_FORCE_INLINE Matrix3 operator-(const Matrix3 &rhs) const {
		return Matrix3(
				cols[0] - rhs.cols[0],
				cols[1] - rhs.cols[1],
				cols[2] - rhs.cols[2]);
	}

	// Scalar multiplication
	[[nodiscard]] constexpr MML_FORCE_INLINE Matrix3 operator*(T scalar) const {
		return Matrix3(
				cols[0] * scalar,
				cols[1] * scalar,
				cols[2] * scalar);
	}

	// Transpose
	[[nodiscard]] constexpr MML_FORCE_INLINE Matrix3 transposed() const {
		return Matrix3(
				cols[0].x, cols[1].x, cols[2].x,
				cols[0].y, cols[1].y, cols[2].y,
				cols[0].z, cols[1].z, cols[2].z);
	}

	// Determinant
	[[nodiscard]] constexpr MML_FORCE_INLINE T determinant() const {
		return cols[0].x * (cols[1].y * cols[2].z - cols[2].y * cols[1].z) -
				cols[1].x * (cols[0].y * cols[2].z - cols[2].y * cols[0].z) +
				cols[2].x * (cols[0].y * cols[1].z - cols[1].y * cols[0].z);
	}

	// Inverse
	[[nodiscard]] MML_FORCE_INLINE Matrix3 inverse() const {
		T det = determinant();
		if (Epsilon<T>::approx_zero(det)) {
			return identity(); // return identity for singular matrices
		}

		T inv_det = T(1) / det;

		Matrix3 result;
		result[0][0] = (cols[1].y * cols[2].z - cols[2].y * cols[1].z) * inv_det;
		result[0][1] = (cols[2].y * cols[0].z - cols[0].y * cols[2].z) * inv_det;
		result[0][2] = (cols[0].y * cols[1].z - cols[1].y * cols[0].z) * inv_det;
		result[1][0] = (cols[2].x * cols[1].z - cols[1].x * cols[2].z) * inv_det;
		result[1][1] = (cols[0].x * cols[2].z - cols[2].x * cols[0].z) * inv_det;
		result[1][2] = (cols[1].x * cols[0].z - cols[0].x * cols[1].z) * inv_det;
		result[2][0] = (cols[1].x * cols[2].y - cols[2].x * cols[1].y) * inv_det;
		result[2][1] = (cols[2].x * cols[0].y - cols[0].x * cols[2].y) * inv_det;
		result[2][2] = (cols[0].x * cols[1].y - cols[1].x * cols[0].y) * inv_det;

		return result;
	}

	// Create rotation matrix around X axis (radians)
	[[nodiscard]] static Matrix3 rotation_x(T angle) {
		T c = std::cos(angle); // cosine of angle
		T s = std::sin(angle); // sine of angle
		return Matrix3(
				T(1), T(0), T(0),
				T(0), c, s,
				T(0), -s, c);
	}

	// Create rotation matrix around Y axis (radians)
	[[nodiscard]] static Matrix3 rotation_y(T angle) {
		T c = std::cos(angle); // cosine of angle
		T s = std::sin(angle); // sine of angle
		return Matrix3(
				c, T(0), -s,
				T(0), T(1), T(0),
				s, T(0), c);
	}

	// Create rotation matrix around Z axis (radians)
	[[nodiscard]] static Matrix3 rotation_z(T angle) {
		T c = std::cos(angle); // cosine of angle
		T s = std::sin(angle); // sine of angle
		return Matrix3(
				c, s, T(0),
				-s, c, T(0),
				T(0), T(0), T(1));
	}

	// Create scale matrix
	[[nodiscard]] static constexpr Matrix3 scale(T x, T y, T z) {
		Matrix3 m = zero();
		m[0][0] = x;
		m[1][1] = y;
		m[2][2] = z;
		return m;
	}

	// Create uniform scale matrix
	[[nodiscard]] static constexpr Matrix3 scale(T scalar) {
		return scale(scalar, scalar, scalar);
	}

	// Create scale matrix from Vector3
	[[nodiscard]] static constexpr Matrix3 scale(const Vector3<T> &v) {
		return scale(v.x, v.y, v.z);
	}

	// Create rotation matrix from axis and angle (radians)
	[[nodiscard]] static Matrix3 rotation_axis(const Vector3<T> &axis, T angle) {
		T c = std::cos(angle); // cosine of angle
		T s = std::sin(angle); // sine of angle
		T t = T(1) - c; // 1 - cosine
		Vector3<T> a = axis.normalized(); // normalized axis
		T x = a.x, y = a.y, z = a.z;

		return Matrix3(
				t * x * x + c, t * x * y + s * z, t * x * z - s * y,
				t * x * y - s * z, t * y * y + c, t * y * z + s * x,
				t * x * z + s * y, t * y * z - s * x, t * z * z + c);
	}

	// Create rotation matrix from Euler angles (radians, ZYX order)
	[[nodiscard]] static Matrix3 rotation_euler(T pitch, T yaw, T roll) {
		T cy = std::cos(yaw); // cosine of yaw
		T sy = std::sin(yaw); // sine of yaw
		T cp = std::cos(pitch); // cosine of pitch
		T sp = std::sin(pitch); // sine of pitch
		T cr = std::cos(roll); // cosine of roll
		T sr = std::sin(roll); // sine of roll

		return Matrix3(
				cy * cr, sy * sp * cr - cp * sr, sy * cp * cr + sp * sr,
				cy * sr, sy * sp * sr + cp * cr, sy * cp * sr - sp * cr,
				-sy, cy * sp, cy * cp);
	}

	// Create shear matrix
	[[nodiscard]] static Matrix3 shear(T xy, T xz, T yx, T yz, T zx, T zy) {
		return Matrix3(
				T(1), yx, zx, // column 0
				xy, T(1), zy, // column 1
				xz, yz, T(1)); // column 2
	}

	// Multiply matrix in place
	MML_FORCE_INLINE Matrix3 &operator*=(const Matrix3 &rhs) {
		*this = *this * rhs;
		return *this;
	}

	// Scalar division
	[[nodiscard]] MML_FORCE_INLINE Matrix3 operator/(T scalar) const {
		T inv = T(1) / scalar;
		return *this * inv;
	}

	MML_FORCE_INLINE Matrix3 &operator/=(T scalar) {
		*this = *this / scalar;
		return *this;
	}

	// Negate matrix
	[[nodiscard]] MML_FORCE_INLINE Matrix3 operator-() const {
		return Matrix3(-cols[0], -cols[1], -cols[2]);
	}

	// Get diagonal as vector
	[[nodiscard]] MML_FORCE_INLINE Vector3<T> diagonal() const {
		return Vector3<T>(cols[0].x, cols[1].y, cols[2].z);
	}

	// Get trace (sum of diagonal elements)
	[[nodiscard]] MML_FORCE_INLINE T trace() const {
		return cols[0].x + cols[1].y + cols[2].z;
	}

	// Check if matrix is symmetric
	[[nodiscard]] MML_FORCE_INLINE bool is_symmetric() const {
		return Epsilon<T>::equal(cols[0].y, cols[1].x) &&
				Epsilon<T>::equal(cols[0].z, cols[2].x) &&
				Epsilon<T>::equal(cols[1].z, cols[2].y);
	}

	// Check if matrix is orthogonal (inverse equals transpose)
	[[nodiscard]] MML_FORCE_INLINE bool is_orthogonal() const {
		Matrix3 transposed = this->transposed();
		Matrix3 product = *this * transposed;
		Matrix3 identity = Matrix3::identity();
		return Epsilon<T>::equal(product[0][0], identity[0][0]) &&
				Epsilon<T>::equal(product[1][1], identity[1][1]) &&
				Epsilon<T>::equal(product[2][2], identity[2][2]);
	}

	// Transform a point (assumes matrix is affine)
	[[nodiscard]] MML_FORCE_INLINE Vector3<T> transform_point(const Vector3<T> &p) const {
		return *this * p;
	}

	// Transform a direction vector (ignores translation)
	[[nodiscard]] MML_FORCE_INLINE Vector3<T> transform_direction(const Vector3<T> &d) const {
		return *this * d;
	}

	// Get rotation part (assuming upper-left 3x3)
	[[nodiscard]] MML_FORCE_INLINE Matrix3 rotation_matrix() const {
		return *this;
	}

	// Get scale from matrix (assuming no rotation)
	[[nodiscard]] MML_FORCE_INLINE Vector3<T> scale_vector() const {
		return Vector3<T>(cols[0].length(), cols[1].length(), cols[2].length());
	}

	// Get orthonormal basis vectors
	[[nodiscard]] MML_FORCE_INLINE Vector3<T> right_vector() const {
		return Vector3<T>(cols[0].x, cols[0].y, cols[0].z).normalized();
	}

	[[nodiscard]] MML_FORCE_INLINE Vector3<T> up_vector() const {
		return Vector3<T>(cols[1].x, cols[1].y, cols[1].z).normalized();
	}

	[[nodiscard]] MML_FORCE_INLINE Vector3<T> forward_vector() const {
		return Vector3<T>(cols[2].x, cols[2].y, cols[2].z).normalized();
	}

	// Create matrix from forward and up vectors
	[[nodiscard]] static Matrix3 from_forward_up(const Vector3<T> &forward, const Vector3<T> &up) {
		Vector3<T> f = forward.normalized(); // forward direction
		Vector3<T> r = up.cross(f).normalized(); // right direction
		Vector3<T> u = f.cross(r); // up direction
		return Matrix3(r, u, f);
	}

	// Create matrix from target and up vectors
	[[nodiscard]] static Matrix3 look_at(const Vector3<T> &eye, const Vector3<T> &target, const Vector3<T> &up) {
		Vector3<T> f = (target - eye).normalized(); // forward: eye to target
		Vector3<T> r = up.cross(f).normalized(); // right direction
		Vector3<T> u = f.cross(r); // up direction
		return Matrix3(r, u, f);
	}

	// Frobenius norm (sqrt of sum of squared elements)
	[[nodiscard]] MML_FORCE_INLINE T frobenius_norm() const {
		return std::sqrt(
				cols[0].length_squared() +
				cols[1].length_squared() +
				cols[2].length_squared());
	}

	// Create rotation matrix from Euler angles (Vector3 version)
	[[nodiscard]] static Matrix3 from_euler(const Vector3<T> &angles) {
		return rotation_euler(angles.x, angles.y, angles.z);
	}

	// Create matrix from right, up, forward vectors
	[[nodiscard]] static Matrix3 from_basis(const Vector3<T> &right, const Vector3<T> &up, const Vector3<T> &forward) {
		return Matrix3(right, up, forward);
	}

	// Get rotation angles from matrix (returns Vector3 in radians)
	[[nodiscard]] Vector3<T> to_euler() const {
		T sy = cols[2].x;
		T cy = std::sqrt(T(1) - sy * sy);
		T cp, sp, cr, sr;

		if (cy > Epsilon<T>::value) {
			sp = cols[2].y;
			cp = cols[2].z;
			sr = cols[1].x;
			cr = cols[0].x;
		} else {
			sp = -cols[1].z;
			cp = cols[0].y;
			sr = cols[0].z;
			cr = cols[1].y;
		}

		T pitch = std::atan2(sp, cp);
		T yaw = std::atan2(-sy, cp);
		T roll = std::atan2(sr, cr);

		return Vector3<T>(pitch, yaw, roll);
	}
};

// Non-member scalar multiplication (scalar * matrix)
template <typename T>
[[nodiscard]] MML_FORCE_INLINE Matrix3<T> operator*(T scalar, const Matrix3<T> &mat) {
	return mat * scalar;
}

// Type aliases
using Matrix3f = Matrix3<float>;
using Matrix3d = Matrix3<double>;

} // namespace mml