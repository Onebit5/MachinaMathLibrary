/**************************************************************************/
/*  matrix4.hpp                                                           */
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
#include "../core/simd.hpp"
#include "matrix3.hpp"
#include "vector3.hpp"
#include "vector4.hpp"
#include <cmath>
#include <type_traits>

namespace mml {

template <typename T>
struct [[nodiscard]] Matrix4 {
	static_assert(std::is_floating_point_v<T>, "Matrix4 requires floating-point type");

	Vector4<T> cols[4];

	constexpr Matrix4() = default;

	[[nodiscard]] static constexpr Matrix4 identity() {
		Matrix4 m;
		m.cols[0] = Vector4<T>(T(1), T(0), T(0), T(0));
		m.cols[1] = Vector4<T>(T(0), T(1), T(0), T(0));
		m.cols[2] = Vector4<T>(T(0), T(0), T(1), T(0));
		m.cols[3] = Vector4<T>(T(0), T(0), T(0), T(1));
		return m;
	}

	[[nodiscard]] static constexpr Matrix4 zero() {
		Matrix4 m;
		m.cols[0] = Vector4<T>(T(0), T(0), T(0), T(0));
		m.cols[1] = Vector4<T>(T(0), T(0), T(0), T(0));
		m.cols[2] = Vector4<T>(T(0), T(0), T(0), T(0));
		m.cols[3] = Vector4<T>(T(0), T(0), T(0), T(0));
		return m;
	}

	explicit constexpr Matrix4(const Vector4<T> &c0, const Vector4<T> &c1, const Vector4<T> &c2, const Vector4<T> &c3) {
		cols[0] = c0;
		cols[1] = c1;
		cols[2] = c2;
		cols[3] = c3;
	}

	explicit constexpr Matrix4(
			T m00, T m10, T m20, T m30,
			T m01, T m11, T m21, T m31,
			T m02, T m12, T m22, T m32,
			T m03, T m13, T m23, T m33) {
		cols[0] = Vector4<T>(m00, m10, m20, m30);
		cols[1] = Vector4<T>(m01, m11, m21, m31);
		cols[2] = Vector4<T>(m02, m12, m22, m32);
		cols[3] = Vector4<T>(m03, m13, m23, m33);
	}

	constexpr Matrix4(const Matrix4 &other) = default;
	Matrix4 &operator=(const Matrix4 &other) = default;

	[[nodiscard]] constexpr MML_FORCE_INLINE Vector4<T> &operator[](size_t i) { return cols[i]; }
	[[nodiscard]] constexpr MML_FORCE_INLINE const Vector4<T> &operator[](size_t i) const { return cols[i]; }

	[[nodiscard]] constexpr MML_FORCE_INLINE Matrix4 operator*(const Matrix4 &rhs) const {
		Matrix4 result;
#if MML_SIMD_SSE2 && defined(__SSE2__)
		if constexpr (std::is_same_v<T, float>) {
			for (size_t col = 0; col < 4; ++col) {
				__m128 c0 = _mm_loadu_ps(&cols[0].x);
				__m128 c1 = _mm_loadu_ps(&cols[1].x);
				__m128 c2 = _mm_loadu_ps(&cols[2].x);
				__m128 c3 = _mm_loadu_ps(&cols[3].x);

				__m128 x = _mm_set1_ps(rhs[col][0]);
				__m128 y = _mm_set1_ps(rhs[col][1]);
				__m128 z = _mm_set1_ps(rhs[col][2]);
				__m128 w = _mm_set1_ps(rhs[col][3]);

				__m128 res = _mm_add_ps(
						_mm_add_ps(_mm_mul_ps(c0, x), _mm_mul_ps(c1, y)),
						_mm_add_ps(_mm_mul_ps(c2, z), _mm_mul_ps(c3, w)));
				_mm_storeu_ps(&result[col].x, res);
			}
			return result;
		}
#endif
		for (size_t col = 0; col < 4; ++col) {
			for (size_t row = 0; row < 4; ++row) {
				result[col][row] =
						cols[0][row] * rhs[col][0] +
						cols[1][row] * rhs[col][1] +
						cols[2][row] * rhs[col][2] +
						cols[3][row] * rhs[col][3];
			}
		}
		return result;
	}

	[[nodiscard]] constexpr MML_FORCE_INLINE Vector4<T> operator*(const Vector4<T> &v) const {
#if MML_SIMD_SSE2 && defined(__SSE2__)
		if constexpr (std::is_same_v<T, float>) {
			__m128 c0 = _mm_loadu_ps(&cols[0].x);
			__m128 c1 = _mm_loadu_ps(&cols[1].x);
			__m128 c2 = _mm_loadu_ps(&cols[2].x);
			__m128 c3 = _mm_loadu_ps(&cols[3].x);

			__m128 vx = _mm_set1_ps(v.x);
			__m128 vy = _mm_set1_ps(v.y);
			__m128 vz = _mm_set1_ps(v.z);
			__m128 vw = _mm_set1_ps(v.w);

			__m128 res = _mm_add_ps(
					_mm_add_ps(_mm_mul_ps(c0, vx), _mm_mul_ps(c1, vy)),
					_mm_add_ps(_mm_mul_ps(c2, vz), _mm_mul_ps(c3, vw)));

			Vector4<float> result;
			_mm_storeu_ps(&result.x, res);
			return result;
		}
#endif
		return Vector4<T>(
				cols[0].x * v.x + cols[1].x * v.y + cols[2].x * v.z + cols[3].x * v.w,
				cols[0].y * v.x + cols[1].y * v.y + cols[2].y * v.z + cols[3].y * v.w,
				cols[0].z * v.x + cols[1].z * v.y + cols[2].z * v.z + cols[3].z * v.w,
				cols[0].w * v.x + cols[1].w * v.y + cols[2].w * v.z + cols[3].w * v.w);
	}

	[[nodiscard]] constexpr MML_FORCE_INLINE Matrix4 operator+(const Matrix4 &rhs) const {
		return Matrix4(
				cols[0] + rhs.cols[0],
				cols[1] + rhs.cols[1],
				cols[2] + rhs.cols[2],
				cols[3] + rhs.cols[3]);
	}

	[[nodiscard]] constexpr MML_FORCE_INLINE Matrix4 operator-(const Matrix4 &rhs) const {
		return Matrix4(
				cols[0] - rhs.cols[0],
				cols[1] - rhs.cols[1],
				cols[2] - rhs.cols[2],
				cols[3] - rhs.cols[3]);
	}

	[[nodiscard]] constexpr MML_FORCE_INLINE Matrix4 operator*(T scalar) const {
		return Matrix4(
				cols[0] * scalar,
				cols[1] * scalar,
				cols[2] * scalar,
				cols[3] * scalar);
	}

	[[nodiscard]] constexpr MML_FORCE_INLINE Matrix4 transposed() const {
		return Matrix4(
				cols[0].x, cols[0].y, cols[0].z, cols[0].w,
				cols[1].x, cols[1].y, cols[1].z, cols[1].w,
				cols[2].x, cols[2].y, cols[2].z, cols[2].w,
				cols[3].x, cols[3].y, cols[3].z, cols[3].w);
	}

	[[nodiscard]] MML_FORCE_INLINE Vector4<T> diagonal() const {
		return Vector4<T>(cols[0].x, cols[1].y, cols[2].z, cols[3].w);
	}

	[[nodiscard]] MML_FORCE_INLINE T trace() const {
		return cols[0].x + cols[1].y + cols[2].z + cols[3].w;
	}

	[[nodiscard]] MML_FORCE_INLINE Vector3<T> translation() const {
		return Vector3<T>(cols[3].x, cols[3].y, cols[3].z);
	}

	[[nodiscard]] MML_FORCE_INLINE Matrix3<T> to_matrix3() const {
		return Matrix3<T>(
				Vector3<T>(cols[0].x, cols[0].y, cols[0].z),
				Vector3<T>(cols[1].x, cols[1].y, cols[1].z),
				Vector3<T>(cols[2].x, cols[2].y, cols[2].z));
	}

	[[nodiscard]] MML_FORCE_INLINE Vector3<T> transform_point(const Vector3<T> &p) const {
		Vector4<T> result = (*this) * Vector4<T>(p, T(1));
		return Vector3<T>(result.x, result.y, result.z);
	}

	[[nodiscard]] MML_FORCE_INLINE Vector3<T> transform_direction(const Vector3<T> &d) const {
		return Vector3<T>(
				cols[0].x * d.x + cols[1].x * d.y + cols[2].x * d.z,
				cols[0].y * d.x + cols[1].y * d.y + cols[2].y * d.z,
				cols[0].z * d.x + cols[1].z * d.y + cols[2].z * d.z);
	}

	[[nodiscard]] static Matrix4 translation(T x, T y, T z) {
		Matrix4 m = identity();
		m[3][0] = x;
		m[3][1] = y;
		m[3][2] = z;
		return m;
	}

	[[nodiscard]] static Matrix4 translation(const Vector3<T> &v) {
		return translation(v.x, v.y, v.z);
	}

	[[nodiscard]] static constexpr Matrix4 scale(T x, T y, T z) {
		Matrix4 m = zero();
		m[0][0] = x;
		m[1][1] = y;
		m[2][2] = z;
		m[3][3] = T(1);
		return m;
	}

	[[nodiscard]] static constexpr Matrix4 scale(const Vector3<T> &v) {
		return scale(v.x, v.y, v.z);
	}

	[[nodiscard]] static Matrix4 rotation_x(T angle) {
		T c = std::cos(angle);
		T s = std::sin(angle);
		Matrix4 m = identity();
		m[1][1] = c;
		m[1][2] = s;
		m[2][1] = -s;
		m[2][2] = c;
		return m;
	}

	[[nodiscard]] static Matrix4 rotation_y(T angle) {
		T c = std::cos(angle);
		T s = std::sin(angle);
		Matrix4 m = identity();
		m[0][0] = c;
		m[0][2] = -s;
		m[2][0] = s;
		m[2][2] = c;
		return m;
	}

	[[nodiscard]] static Matrix4 rotation_z(T angle) {
		T c = std::cos(angle);
		T s = std::sin(angle);
		Matrix4 m = identity();
		m[0][0] = c;
		m[0][1] = s;
		m[1][0] = -s;
		m[1][1] = c;
		return m;
	}

	[[nodiscard]] static Matrix4 rotation_axis(const Vector3<T> &axis, T angle) {
		T c = std::cos(angle);
		T s = std::sin(angle);
		T t = T(1) - c;
		Vector3<T> a = axis.normalized();
		T x = a.x, y = a.y, z = a.z;

		Matrix4 m = identity();
		m[0][0] = t * x * x + c;
		m[0][1] = t * x * y + s * z;
		m[0][2] = t * x * z - s * y;
		m[1][0] = t * x * y - s * z;
		m[1][1] = t * y * y + c;
		m[1][2] = t * y * z + s * x;
		m[2][0] = t * x * z + s * y;
		m[2][1] = t * y * z - s * x;
		m[2][2] = t * z * z + c;
		return m;
	}

	[[nodiscard]] static Matrix4 rotation_euler(T pitch, T yaw, T roll) {
		T cy = std::cos(yaw);
		T sy = std::sin(yaw);
		T cp = std::cos(pitch);
		T sp = std::sin(pitch);
		T cr = std::cos(roll);
		T sr = std::sin(roll);

		Matrix4 m = identity();
		m[0][0] = cy * cr;
		m[0][1] = sy * sp * cr - cp * sr;
		m[0][2] = sy * cp * cr + sp * sr;
		m[1][0] = cy * sr;
		m[1][1] = sy * sp * sr + cp * cr;
		m[1][2] = sy * cp * sr - sp * cr;
		m[2][0] = -sy;
		m[2][1] = cy * sp;
		m[2][2] = cy * cp;
		return m;
	}

	[[nodiscard]] static Matrix4 look_at(const Vector3<T> &eye, const Vector3<T> &target, const Vector3<T> &up) {
		Vector3<T> f = (target - eye).normalized();
		Vector3<T> s = f.cross(up).normalized();
		Vector3<T> u = s.cross(f);

		Matrix4 m = identity();
		m[0][0] = s.x;
		m[1][0] = s.y;
		m[2][0] = s.z;
		m[0][1] = u.x;
		m[1][1] = u.y;
		m[2][1] = u.z;
		m[0][2] = -f.x;
		m[1][2] = -f.y;
		m[2][2] = -f.z;
		m[3][0] = -s.dot(eye);
		m[3][1] = -u.dot(eye);
		m[3][2] = f.dot(eye);
		return m;
	}

	[[nodiscard]] static Matrix4 orthographic(T left, T right, T bottom, T top, T near_plane, T far_plane) {
		T tx = -(right + left) / (right - left);
		T ty = -(top + bottom) / (top - bottom);
		T tz = -(far_plane + near_plane) / (far_plane - near_plane);

		Matrix4 m = zero();
		m[0][0] = T(2) / (right - left);
		m[1][1] = T(2) / (top - bottom);
		m[2][2] = -T(2) / (far_plane - near_plane);
		m[3][0] = tx;
		m[3][1] = ty;
		m[3][2] = tz;
		m[3][3] = T(1);
		return m;
	}

	[[nodiscard]] static Matrix4 perspective(T fov_y, T aspect, T near_plane, T far_plane) {
		T tan_half_fov = std::tan(fov_y * T(0.5));

		Matrix4 m = zero();
		m[0][0] = T(1) / (aspect * tan_half_fov);
		m[1][1] = T(1) / tan_half_fov;
		m[2][2] = -(far_plane + near_plane) / (far_plane - near_plane);
		m[2][3] = -T(1);
		m[3][2] = -(T(2) * far_plane * near_plane) / (far_plane - near_plane);
		return m;
	}

	[[nodiscard]] MML_FORCE_INLINE Vector3<T> right_vector() const {
		return Vector3<T>(cols[0].x, cols[0].y, cols[0].z).normalized();
	}

	[[nodiscard]] MML_FORCE_INLINE Vector3<T> up_vector() const {
		return Vector3<T>(cols[1].x, cols[1].y, cols[1].z).normalized();
	}

	[[nodiscard]] MML_FORCE_INLINE Vector3<T> forward_vector() const {
		return Vector3<T>(cols[2].x, cols[2].y, cols[2].z).normalized();
	}

	void decompose(Vector3<T> &out_translation, Matrix3<T> &out_rotation, Vector3<T> &out_scale) const {
		out_translation = translation();
		out_scale = Vector3<T>(
				Vector3<T>(cols[0].x, cols[0].y, cols[0].z).length(),
				Vector3<T>(cols[1].x, cols[1].y, cols[1].z).length(),
				Vector3<T>(cols[2].x, cols[2].y, cols[2].z).length());

		Matrix3<T> rot(
				Vector3<T>(cols[0].x, cols[0].y, cols[0].z) / out_scale.x,
				Vector3<T>(cols[1].x, cols[1].y, cols[1].z) / out_scale.y,
				Vector3<T>(cols[2].x, cols[2].y, cols[2].z) / out_scale.z);
		out_rotation = rot;
	}

	[[nodiscard]] MML_FORCE_INLINE T frobenius_norm() const {
		T sum = T(0);
		for (size_t i = 0; i < 4; ++i) {
			sum += cols[i].length_squared();
		}
		return std::sqrt(sum);
	}

	[[nodiscard]] static Matrix4 from_euler(const Vector3<T> &angles) {
		return rotation_euler(angles.x, angles.y, angles.z);
	}

	[[nodiscard]] static Matrix4 from_basis(const Vector3<T> &right, const Vector3<T> &up, const Vector3<T> &forward) {
		Matrix4 m = identity();
		m[0][0] = right.x;
		m[1][0] = right.y;
		m[2][0] = right.z;
		m[0][1] = up.x;
		m[1][1] = up.y;
		m[2][1] = up.z;
		m[0][2] = forward.x;
		m[1][2] = forward.y;
		m[2][2] = forward.z;
		return m;
	}

	[[nodiscard]] MML_FORCE_INLINE Vector3<T> right_vector_raw() const {
		return Vector3<T>(cols[0].x, cols[0].y, cols[0].z);
	}

	[[nodiscard]] MML_FORCE_INLINE Vector3<T> up_vector_raw() const {
		return Vector3<T>(cols[1].x, cols[1].y, cols[1].z);
	}

	[[nodiscard]] MML_FORCE_INLINE Vector3<T> forward_vector_raw() const {
		return Vector3<T>(cols[2].x, cols[2].y, cols[2].z);
	}

	[[nodiscard]] MML_FORCE_INLINE Matrix4 inverse_affine() const {
		Matrix3<T> m3 = to_matrix3();
		Matrix3<T> m3_inv = m3.inverse();

		Matrix4 result = identity();
		result[0][0] = m3_inv[0][0];
		result[0][1] = m3_inv[0][1];
		result[0][2] = m3_inv[0][2];
		result[1][0] = m3_inv[1][0];
		result[1][1] = m3_inv[1][1];
		result[1][2] = m3_inv[1][2];
		result[2][0] = m3_inv[2][0];
		result[2][1] = m3_inv[2][1];
		result[2][2] = m3_inv[2][2];

		Vector3<T> t(cols[3].x, cols[3].y, cols[3].z);
		Vector3<T> neg_inv_t = -(m3_inv * t);
		result[3][0] = neg_inv_t.x;
		result[3][1] = neg_inv_t.y;
		result[3][2] = neg_inv_t.z;

		return result;
	}

	[[nodiscard]] MML_FORCE_INLINE Matrix4 inverse() const {
		T s0 = cols[0][0] * cols[1][1] - cols[0][1] * cols[1][0];
		T s1 = cols[0][0] * cols[2][1] - cols[0][1] * cols[2][0];
		T s2 = cols[0][0] * cols[3][1] - cols[0][1] * cols[3][0];
		T s3 = cols[1][0] * cols[2][1] - cols[1][1] * cols[2][0];
		T s4 = cols[1][0] * cols[3][1] - cols[1][1] * cols[3][0];
		T s5 = cols[2][0] * cols[3][1] - cols[2][1] * cols[3][0];

		T c0 = cols[0][2] * cols[1][3] - cols[0][3] * cols[1][2];
		T c1 = cols[0][2] * cols[2][3] - cols[0][3] * cols[2][2];
		T c2 = cols[0][2] * cols[3][3] - cols[0][3] * cols[3][2];
		T c3 = cols[1][2] * cols[2][3] - cols[1][3] * cols[2][2];
		T c4 = cols[1][2] * cols[3][3] - cols[1][3] * cols[3][2];
		T c5 = cols[2][2] * cols[3][3] - cols[2][3] * cols[3][2];

		T det = s0 * c5 - s1 * c4 + s2 * c3 + s3 * c2 - s4 * c1 + s5 * c0;

		if (Epsilon<T>::approx_zero(det)) {
			return identity();
		}

		T inv_det = T(1) / det;

		Matrix4 result;
		result[0][0] = (cols[1][1] * c5 - cols[2][1] * c4 + cols[3][1] * c3) * inv_det;
		result[0][1] = (-cols[0][1] * c5 + cols[2][1] * c2 - cols[3][1] * c1) * inv_det;
		result[0][2] = (cols[0][1] * c4 - cols[1][1] * c2 + cols[3][1] * c0) * inv_det;
		result[0][3] = (-cols[0][1] * c3 + cols[1][1] * c1 - cols[2][1] * c0) * inv_det;

		result[1][0] = (-cols[1][0] * c5 + cols[2][0] * c4 - cols[3][0] * c3) * inv_det;
		result[1][1] = (cols[0][0] * c5 - cols[2][0] * c2 + cols[3][0] * c1) * inv_det;
		result[1][2] = (-cols[0][0] * c4 + cols[1][0] * c2 - cols[3][0] * c0) * inv_det;
		result[1][3] = (cols[0][0] * c3 - cols[1][0] * c1 + cols[2][0] * c0) * inv_det;

		result[2][0] = (cols[1][3] * s5 - cols[2][3] * s4 + cols[3][3] * s3) * inv_det;
		result[2][1] = (-cols[0][3] * s5 + cols[2][3] * s2 - cols[3][3] * s1) * inv_det;
		result[2][2] = (cols[0][3] * s4 - cols[1][3] * s2 + cols[3][3] * s0) * inv_det;
		result[2][3] = (-cols[0][3] * s3 + cols[1][3] * s1 - cols[2][3] * s0) * inv_det;

		result[3][0] = (-cols[1][2] * s5 + cols[2][2] * s4 - cols[3][2] * s3) * inv_det;
		result[3][1] = (cols[0][2] * s5 - cols[2][2] * s2 + cols[3][2] * s1) * inv_det;
		result[3][2] = (-cols[0][2] * s4 + cols[1][2] * s2 - cols[3][2] * s0) * inv_det;
		result[3][3] = (cols[0][2] * s3 - cols[1][2] * s1 + cols[2][2] * s0) * inv_det;

		return result;
	}
};

using Matrix4f = Matrix4<float>;
using Matrix4d = Matrix4<double>;

} //namespace mml