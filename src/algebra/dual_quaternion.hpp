/**************************************************************************/
/*  dual_quaternion.hpp                                                   */
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

#include "../algebra/quaternion.hpp"
#include "../algebra/vector3.hpp"
#include "../algebra/vector4.hpp"
#include "../core/config.hpp"
#include "../core/epsilon.hpp"
#include "../utils/math.hpp"
#include <algorithm>
#include <cmath>
#include <type_traits>
#include <vector>

namespace mml {

template <typename T>
struct DualQuaternion {
	static_assert(std::is_floating_point_v<T>, "DualQuaternion requires floating-point type");

	Quaternion<T> real;
	Quaternion<T> dual;

	DualQuaternion() = default;

	DualQuaternion(const Quaternion<T> &r, const Quaternion<T> &d) :
			real(r), dual(d) {}

	static DualQuaternion from_rotation_translation(const Quaternion<T> &rotation, const Vector3<T> &translation) {
		DualQuaternion dq;
		dq.real = rotation.normalized();
		dq.dual = Quaternion<T>(
						  translation.x * T(0.5),
						  translation.y * T(0.5),
						  translation.z * T(0.5),
						  T(0)) *
				dq.real;
		return dq;
	}

	static DualQuaternion from_transform(const Matrix4<T> &transform) {
		Vector3<T> translation = transform.translation();
		Matrix3<T> rotation_matrix = transform.to_matrix3();
		Quaternion<T> rotation = Quaternion<T>::from_matrix(rotation_matrix);
		return from_rotation_translation(rotation, translation);
	}

	[[nodiscard]] Matrix4<T> to_matrix4() const {
		DualQuaternion n = normalized();
		Quaternion<T> r = n.real;
		Quaternion<T> d = n.dual;

		T xx = r.x * r.x, xy = r.x * r.y, xz = r.x * r.z;
		T yy = r.y * r.y, yz = r.y * r.z, zz = r.z * r.z;
		T wx = r.w * r.x, wy = r.w * r.y, wz = r.w * r.z;

		Quaternion<T> t_quat = d * r.conjugate();
		T tx = T(2) * t_quat.x;
		T ty = T(2) * t_quat.y;
		T tz = T(2) * t_quat.z;

		Matrix4<T> result;
		result[0][0] = T(1) - T(2) * (yy + zz);
		result[0][1] = T(2) * (xy + wz);
		result[0][2] = T(2) * (xz - wy);
		result[0][3] = tx;

		result[1][0] = T(2) * (xy - wz);
		result[1][1] = T(1) - T(2) * (xx + zz);
		result[1][2] = T(2) * (yz + wx);
		result[1][3] = ty;

		result[2][0] = T(2) * (xz + wy);
		result[2][1] = T(2) * (yz - wx);
		result[2][2] = T(1) - T(2) * (xx + yy);
		result[2][3] = tz;

		result[3][0] = T(0);
		result[3][1] = T(0);
		result[3][2] = T(0);
		result[3][3] = T(1);

		return result;
	}

	[[nodiscard]] DualQuaternion normalized() const {
		T len = std::sqrt(real.x * real.x + real.y * real.y + real.z * real.z + real.w * real.w);
		if (len < Epsilon<T>::value) {
			return DualQuaternion();
		}
		return DualQuaternion(
				Quaternion<T>(real.x / len, real.y / len, real.z / len, real.w / len),
				Quaternion<T>(dual.x / len, dual.y / len, dual.z / len, dual.w / len));
	}

	DualQuaternion &normalize() {
		T len = std::sqrt(real.x * real.x + real.y * real.y + real.z * real.z + real.w * real.w);
		if (len >= Epsilon<T>::value) {
			real.x /= len;
			real.y /= len;
			real.z /= len;
			real.w /= len;
			dual.x /= len;
			dual.y /= len;
			dual.z /= len;
			dual.w /= len;
		}
		return *this;
	}

	[[nodiscard]] Vector3<T> translation() const {
		Quaternion<T> t_quat = dual * real.conjugate();
		return Vector3<T>(T(2) * t_quat.x, T(2) * t_quat.y, T(2) * t_quat.z);
	}

	[[nodiscard]] DualQuaternion operator+(const DualQuaternion &rhs) const {
		return DualQuaternion(real + rhs.real, dual + rhs.dual);
	}

	DualQuaternion &operator+=(const DualQuaternion &rhs) {
		real += rhs.real;
		dual += rhs.dual;
		return *this;
	}

	[[nodiscard]] DualQuaternion operator-(const DualQuaternion &rhs) const {
		return DualQuaternion(real - rhs.real, dual - rhs.dual);
	}

	DualQuaternion &operator-=(const DualQuaternion &rhs) {
		real -= rhs.real;
		dual -= rhs.dual;
		return *this;
	}

	[[nodiscard]] DualQuaternion operator*(const DualQuaternion &rhs) const {
		return DualQuaternion(
				real * rhs.real,
				real * rhs.dual + dual * rhs.real);
	}

	DualQuaternion &operator*=(const DualQuaternion &rhs) {
		Quaternion<T> new_real = real * rhs.real;
		dual = real * rhs.dual + dual * rhs.real;
		real = new_real;
		return *this;
	}

	[[nodiscard]] DualQuaternion operator*(T scalar) const {
		return DualQuaternion(real * scalar, dual * scalar);
	}

	DualQuaternion &operator*=(T scalar) {
		real *= scalar;
		dual *= scalar;
		return *this;
	}

	[[nodiscard]] DualQuaternion conjugate() const {
		return DualQuaternion(real.conjugate(), dual.conjugate());
	}

	[[nodiscard]] DualQuaternion inverse() const {
		DualQuaternion conj = conjugate();
		T len_sq = real.x * real.x + real.y * real.y + real.z * real.z + real.w * real.w;
		if (len_sq < Epsilon<T>::value) {
			return DualQuaternion();
		}
		T inv_len_sq = T(1) / len_sq;
		return DualQuaternion(
				Quaternion<T>(conj.real.x * inv_len_sq, conj.real.y * inv_len_sq, conj.real.z * inv_len_sq, conj.real.w * inv_len_sq),
				Quaternion<T>(conj.dual.x * inv_len_sq, conj.dual.y * inv_len_sq, conj.dual.z * inv_len_sq, conj.dual.w * inv_len_sq));
	}

	[[nodiscard]] bool operator==(const DualQuaternion &rhs) const {
		return real == rhs.real && dual == rhs.dual;
	}

	[[nodiscard]] bool operator!=(const DualQuaternion &rhs) const {
		return !(*this == rhs);
	}

	[[nodiscard]] T dot(const DualQuaternion &rhs) const {
		return real.dot(rhs.real);
	}

	[[nodiscard]] T length_squared() const {
		return dot(*this);
	}

	[[nodiscard]] T length() const {
		return std::sqrt(length_squared());
	}

	[[nodiscard]] static DualQuaternion slerp(const DualQuaternion &a, const DualQuaternion &b, T t) {
		T cos_theta = a.real.dot(b.real);
		cos_theta = std::max(T(-1), std::min(T(1), cos_theta));
		T theta = std::acos(cos_theta);

		if (theta < Epsilon<T>::value) {
			return a * (T(1) - t) + b * t;
		}

		T sin_theta = std::sin(theta);
		T wa = std::sin((T(1) - t) * theta) / sin_theta;
		T wb = std::sin(t * theta) / sin_theta;

		return DualQuaternion(
				Quaternion<T>(
						wa * a.real.x + wb * b.real.x,
						wa * a.real.y + wb * b.real.y,
						wa * a.real.z + wb * b.real.z,
						wa * a.real.w + wb * b.real.w),
				Quaternion<T>(
						wa * a.dual.x + wb * b.dual.x,
						wa * a.dual.y + wb * b.dual.y,
						wa * a.dual.z + wb * b.dual.z,
						wa * a.dual.w + wb * b.dual.w));
	}

	[[nodiscard]] static DualQuaternion identity() {
		return DualQuaternion(Quaternion<T>::identity(), Quaternion<T>(T(0), T(0), T(0), T(0)));
	}
};

using DualQuaternionf = DualQuaternion<float>;
using DualQuaterniond = DualQuaternion<double>;

} //namespace mml