/**************************************************************************/
/*  quaternion.hpp                                                        */
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
#include "../core/epsilon.hpp"
#include "../utils/math.hpp"
#include "matrix3.hpp"
#include "matrix4.hpp"
#include "vector3.hpp"
#include <algorithm>
#include <cmath>
#include <type_traits>

namespace mml {

template <typename T>
struct Quaternion {
	static_assert(std::is_floating_point_v<T>, "Quaternion requires floating-point type");

	T x, y, z, w;

	Quaternion() = default;

	[[nodiscard]] static constexpr Quaternion identity() {
		return Quaternion(T(0), T(0), T(0), T(1));
	}

	explicit constexpr Quaternion(T x_, T y_, T z_, T w_) :
			x(x_), y(y_), z(z_), w(w_) {}

	[[nodiscard]] static Quaternion from_axis_angle(const Vector3<T> &axis, T angle) {
		T half_angle = angle * T(0.5);
		T s = std::sin(half_angle);
		Vector3<T> normalized_axis = axis.normalized();
		return Quaternion(
				normalized_axis.x * s,
				normalized_axis.y * s,
				normalized_axis.z * s,
				std::cos(half_angle));
	}

	[[nodiscard]] static Quaternion from_euler(T pitch, T yaw, T roll) {
		T cy = std::cos(yaw * T(0.5));
		T sy = std::sin(yaw * T(0.5));
		T cp = std::cos(pitch * T(0.5));
		T sp = std::sin(pitch * T(0.5));
		T cr = std::cos(roll * T(0.5));
		T sr = std::sin(roll * T(0.5));

		return Quaternion(
				sr * cp * cy - cr * sp * sy,
				cr * sp * cy + sr * cp * sy,
				cr * cp * sy - sr * sp * cy,
				cr * cp * cy + sr * sp * sy);
	}

	[[nodiscard]] static Quaternion from_euler(const Vector3<T> &angles) {
		return from_euler(angles.x, angles.y, angles.z);
	}

	[[nodiscard]] static Quaternion from_to_rotation(const Vector3<T> &from, const Vector3<T> &to) {
		Vector3<T> from_norm = from.normalized();
		Vector3<T> to_norm = to.normalized();

		T dot_val = from_norm.dot(to_norm);

		if (dot_val >= T(1) - Epsilon<T>::value) {
			return identity();
		}

		if (dot_val <= T(-1) + Epsilon<T>::value) {
			Vector3<T> orth = std::abs(from_norm.x) < std::abs(from_norm.y) ? Vector3<T>::unit_x() : Vector3<T>::unit_y();
			Vector3<T> axis = from_norm.cross(orth).normalized();
			return from_axis_angle(axis, Constants<T>::pi);
		}

		Vector3<T> axis = from_norm.cross(to_norm);
		T w = std::sqrt((T(1) + dot_val) * T(2));
		T inv_w = T(1) / w;

		return Quaternion(axis.x * inv_w, axis.y * inv_w, axis.z * inv_w, w * T(0.5));
	}

	[[nodiscard]] static Quaternion rotation_between(const Vector3<T> &from, const Vector3<T> &to) {
		return from_to_rotation(from, to);
	}

	[[nodiscard]] void to_axis_angle(Vector3<T> &out_axis, T &out_angle) const {
		out_angle = angle();
		out_axis = axis();
	}

	[[nodiscard]] Quaternion rotation_to(const Quaternion &to) const {
		return conjugate() * to;
	}

	[[nodiscard]] T angle_to(const Quaternion &to) const {
		return rotation_to(to).angle();
	}

	[[nodiscard]] static Quaternion look_rotation(const Vector3<T> &forward, const Vector3<T> &up = Vector3<T>::up()) {
		Vector3<T> f = forward.normalized();
		Vector3<T> r = up.cross(f).normalized();
		Vector3<T> u = f.cross(r);

		Matrix3<T> m(r, u, f);
		return from_matrix(m);
	}

	[[nodiscard]] static Quaternion from_matrix(const Matrix3<T> &m) {
		T trace = m[0][0] + m[1][1] + m[2][2];

		if (trace > T(0)) {
			T s = Math<T>::safe_sqrt(trace + T(1)) * T(2);
			return Quaternion(
					(m[1][2] - m[2][1]) / s,
					(m[2][0] - m[0][2]) / s,
					(m[0][1] - m[1][0]) / s,
					s * T(0.25));
		} else if (m[0][0] > m[1][1] && m[0][0] > m[2][2]) {
			T s = Math<T>::safe_sqrt(T(1) + m[0][0] - m[1][1] - m[2][2]) * T(2);
			return Quaternion(
					s * T(0.25),
					(m[1][0] + m[0][1]) / s,
					(m[2][0] + m[0][2]) / s,
					(m[1][2] - m[2][1]) / s);
		} else if (m[1][1] > m[2][2]) {
			T s = Math<T>::safe_sqrt(T(1) + m[1][1] - m[0][0] - m[2][2]) * T(2);
			return Quaternion(
					(m[1][0] + m[0][1]) / s,
					s * T(0.25),
					(m[2][1] + m[1][2]) / s,
					(m[2][0] - m[0][2]) / s);
		} else {
			T s = Math<T>::safe_sqrt(T(1) + m[2][2] - m[0][0] - m[1][1]) * T(2);
			return Quaternion(
					(m[2][0] + m[0][2]) / s,
					(m[2][1] + m[1][2]) / s,
					s * T(0.25),
					(m[0][1] - m[1][0]) / s);
		}
	}

	constexpr Quaternion(const Quaternion &other) = default;
	Quaternion &operator=(const Quaternion &other) = default;

	[[nodiscard]] MML_FORCE_INLINE Quaternion operator+(const Quaternion &rhs) const {
		return Quaternion(x + rhs.x, y + rhs.y, z + rhs.z, w + rhs.w);
	}

	[[nodiscard]] MML_FORCE_INLINE Quaternion operator-(const Quaternion &rhs) const {
		return Quaternion(x - rhs.x, y - rhs.y, z - rhs.z, w - rhs.w);
	}

	[[nodiscard]] MML_FORCE_INLINE Quaternion operator*(const Quaternion &rhs) const {
		return Quaternion(
				w * rhs.x + x * rhs.w + y * rhs.z - z * rhs.y,
				w * rhs.y - x * rhs.z + y * rhs.w + z * rhs.x,
				w * rhs.z + x * rhs.y - y * rhs.x + z * rhs.w,
				w * rhs.w - x * rhs.x - y * rhs.y - z * rhs.z);
	}

	[[nodiscard]] MML_FORCE_INLINE Quaternion operator*(T scalar) const {
		return Quaternion(x * scalar, y * scalar, z * scalar, w * scalar);
	}

	[[nodiscard]] MML_FORCE_INLINE Vector3<T> rotate(const Vector3<T> &v) const {
		Vector3<T> qv(x, y, z);
		Vector3<T> t = qv.cross(v) * T(2);
		return v + t * w + qv.cross(t);
	}

	[[nodiscard]] MML_FORCE_INLINE T dot(const Quaternion &rhs) const {
		return x * rhs.x + y * rhs.y + z * rhs.z + w * rhs.w;
	}

	[[nodiscard]] MML_FORCE_INLINE T length_squared() const {
		return x * x + y * y + z * z + w * w;
	}

	[[nodiscard]] MML_FORCE_INLINE T length() const {
		return Math<T>::safe_sqrt(length_squared());
	}

	[[nodiscard]] MML_FORCE_INLINE bool is_zero() const {
		return Epsilon<T>::approx_zero(length_squared());
	}

	[[nodiscard]] MML_FORCE_INLINE bool is_identity() const {
		return Epsilon<T>::equal(x, T(0)) && Epsilon<T>::equal(y, T(0)) &&
				Epsilon<T>::equal(z, T(0)) && Epsilon<T>::equal(w, T(1));
	}

	[[nodiscard]] MML_FORCE_INLINE bool is_finite() const {
		return std::isfinite(x) && std::isfinite(y) && std::isfinite(z) && std::isfinite(w);
	}

	[[nodiscard]] MML_FORCE_INLINE bool is_normalized() const {
		return Epsilon<T>::equal(length_squared(), T(1));
	}

	[[nodiscard]] MML_FORCE_INLINE Quaternion normalized() const {
		T len = length();
		if (Epsilon<T>::approx_zero(len)) {
			return identity();
		}
		T inv_len = T(1) / len;
		return Quaternion(x * inv_len, y * inv_len, z * inv_len, w * inv_len);
	}

	MML_FORCE_INLINE Quaternion &normalize() {
		T len = length();
		if (!Epsilon<T>::approx_zero(len)) {
			T inv_len = T(1) / len;
			x *= inv_len;
			y *= inv_len;
			z *= inv_len;
			w *= inv_len;
		}
		return *this;
	}

	[[nodiscard]] MML_FORCE_INLINE Quaternion conjugate() const {
		return Quaternion(-x, -y, -z, w);
	}

	[[nodiscard]] MML_FORCE_INLINE Quaternion inverse() const {
		T len_sq = length_squared();
		if (Epsilon<T>::approx_zero(len_sq)) {
			return identity();
		}
		T inv_len_sq = T(1) / len_sq;
		return Quaternion(-x * inv_len_sq, -y * inv_len_sq, -z * inv_len_sq, w * inv_len_sq);
	}

	[[nodiscard]] Matrix3<T> to_matrix3() const {
		T xx = x * x, yy = y * y, zz = z * z;
		T xy = x * y, xz = x * z, yz = y * z;
		T wx = w * x, wy = w * y, wz = w * z;

		return Matrix3<T>(
				T(1) - T(2) * (yy + zz), T(2) * (xy + wz), T(2) * (xz - wy),
				T(2) * (xy - wz), T(1) - T(2) * (xx + zz), T(2) * (yz + wx),
				T(2) * (xz + wy), T(2) * (yz - wx), T(1) - T(2) * (xx + yy));
	}

	[[nodiscard]] Matrix4<T> to_matrix4() const {
		Matrix3<T> m3 = to_matrix3();
		return Matrix4<T>(
				Vector4<T>(m3[0], T(0)),
				Vector4<T>(m3[1], T(0)),
				Vector4<T>(m3[2], T(0)),
				Vector4<T>(T(0), T(0), T(0), T(1)));
	}

	[[nodiscard]] static Quaternion slerp(const Quaternion &a, const Quaternion &b, T t) {
		T cos_theta = a.dot(b);

		Quaternion b_adjusted = b;
		if (cos_theta < T(0)) {
			b_adjusted = Quaternion(-b.x, -b.y, -b.z, -b.w);
			cos_theta = -cos_theta;
		}

		if (cos_theta > T(1) - Epsilon<T>::value) {
			return Quaternion(
					a.x + t * (b_adjusted.x - a.x),
					a.y + t * (b_adjusted.y - a.y),
					a.z + t * (b_adjusted.z - a.z),
					a.w + t * (b_adjusted.w - a.w))
					.normalized();
		}

		T theta = Math<T>::safe_acos(cos_theta);
		T sin_theta = std::sin(theta);
		T wa = std::sin((T(1) - t) * theta) / sin_theta;
		T wb = std::sin(t * theta) / sin_theta;

		return Quaternion(
				a.x * wa + b_adjusted.x * wb,
				a.y * wa + b_adjusted.y * wb,
				a.z * wa + b_adjusted.z * wb,
				a.w * wa + b_adjusted.w * wb);
	}

	[[nodiscard]] static Quaternion squad(const Quaternion &q0, const Quaternion &q1, const Quaternion &q2, const Quaternion &q3, T t) {
		Quaternion s0 = slerp(q0, q3, t);
		Quaternion s1 = slerp(q1, q2, t);
		return slerp(s0, s1, 2.0 * t * (1.0 - t));
	}

	[[nodiscard]] static Quaternion squad_intermediate(const Quaternion &q_prev, const Quaternion &q_curr, const Quaternion &q_next) {
		Quaternion q_prev_inv = q_prev.conjugate();
		Quaternion q_next_inv = q_next.conjugate();
		Quaternion term1 = q_prev_inv * q_curr;
		Quaternion term2 = q_next_inv * q_curr;
		return q_curr * (term1 + term2).normalized() * T(-0.25);
	}

	[[nodiscard]] static Quaternion lerp_unnormalized(const Quaternion &a, const Quaternion &b, T t) {
		return Quaternion(
				a.x + t * (b.x - a.x),
				a.y + t * (b.y - a.y),
				a.z + t * (b.z - a.z),
				a.w + t * (b.w - a.w));
	}

	[[nodiscard]] static Quaternion nlerp(const Quaternion &a, const Quaternion &b, T t) {
		return lerp_unnormalized(a, b, t).normalized();
	}

	[[nodiscard]] Quaternion exp() const {
		Vector3<T> v(x, y, z);
		T v_len = v.length();

		if (Epsilon<T>::approx_zero(v_len)) {
			return identity();
		}

		T s = std::sin(v_len) / v_len;
		T e = std::exp(w);

		return Quaternion(
				e * s * v.x,
				e * s * v.y,
				e * s * v.z,
				e * std::cos(v_len));
	}

	[[nodiscard]] Quaternion log() const {
		T len = length();

		if (Epsilon<T>::approx_zero(len)) {
			return Quaternion(T(0), T(0), T(0), std::numeric_limits<T>::lowest());
		}

		T v_len = Math<T>::safe_sqrt(x * x + y * y + z * z);

		if (Epsilon<T>::approx_zero(v_len)) {
			return Quaternion(T(0), T(0), T(0), std::log(len));
		}

		T coeff = Math<T>::safe_acos(w / len) / v_len;
		return Quaternion(
				coeff * x,
				coeff * y,
				coeff * z,
				std::log(len));
	}

	[[nodiscard]] Quaternion pow(T exponent) const {
		T len = length();
		if (Epsilon<T>::approx_zero(len)) {
			return identity();
		}

		T alpha = Math<T>::safe_acos(w / len);
		T new_alpha = alpha * exponent;
		T s = std::sin(new_alpha) / std::sin(alpha);

		return Quaternion(
				s * x,
				s * y,
				s * z,
				std::cos(new_alpha));
	}

	[[nodiscard]] Quaternion negate() const {
		return Quaternion(-x, -y, -z, -w);
	}

	[[nodiscard]] T angle() const {
		return T(2) * Math<T>::safe_acos(std::abs(w) / length());
	}

	[[nodiscard]] Vector3<T> axis() const {
		T s = Math<T>::safe_sqrt(T(1) - w * w);
		if (Epsilon<T>::approx_zero(s)) {
			return Vector3<T>::unit_x();
		}
		return Vector3<T>(x, y, z) / s;
	}

	[[nodiscard]] Vector3<T> to_euler() const {
		T sinr_cosp = T(2) * (w * x + y * z);
		T cosr_cosp = T(1) - T(2) * (x * x + y * y);
		T roll = std::atan2(sinr_cosp, cosr_cosp);

		T sinp = T(2) * (w * y - z * x);
		T pitch;
		if (std::abs(sinp) >= T(1)) {
			pitch = std::copysign(Constants<T>::half_pi, sinp);
		} else {
			pitch = Math<T>::safe_asin(sinp);
		}

		T siny_cosp = T(2) * (w * z + x * y);
		T cosy_cosp = T(1) - T(2) * (y * y + z * z);
		T yaw = std::atan2(siny_cosp, cosy_cosp);

		return Vector3<T>(pitch, yaw, roll);
	}

	[[nodiscard]] Vector3<T> forward() const {
		return rotate(Vector3<T>::forward());
	}

	[[nodiscard]] Vector3<T> up() const {
		return rotate(Vector3<T>::up());
	}

	[[nodiscard]] Vector3<T> right() const {
		return rotate(Vector3<T>::right());
	}

	[[nodiscard]] static T angle_between(const Quaternion &a, const Quaternion &b) {
		T dot_val = a.dot(b);
		return T(2) * Math<T>::safe_acos(std::abs(dot_val));
	}

	[[nodiscard]] static T angular_distance(const Quaternion &a, const Quaternion &b) {
		Quaternion diff = b.inverse() * a;
		return diff.angle();
	}

	[[nodiscard]] static Quaternion shortest_path(const Quaternion &a, const Quaternion &b) {
		if (a.dot(b) < T(0)) {
			return Quaternion(-b.x, -b.y, -b.z, -b.w);
		}
		return b;
	}

	[[nodiscard]] bool is_valid() const {
		return Epsilon<T>::equal(length(), T(1));
	}

	void swing_twist(const Vector3<T> &twist_axis, Quaternion &out_swing, Quaternion &out_twist) const {
		Vector3<T> r(x, y, z);
		T p = r.dot(twist_axis);
		out_twist = Quaternion(twist_axis * p, w).normalized();
		out_swing = *this * out_twist.conjugate();
	}

	[[nodiscard]] Quaternion roll_quat() const {
		return Quaternion(x, T(0), T(0), w).normalized();
	}

	[[nodiscard]] Quaternion pitch_quat() const {
		return Quaternion(T(0), y, T(0), w).normalized();
	}

	[[nodiscard]] Quaternion yaw_quat() const {
		return Quaternion(T(0), T(0), z, w).normalized();
	}

	struct DualQuaternion {
		Quaternion real;
		Quaternion dual;

		static DualQuaternion from_rotation_translation(const Quaternion &rot, const Vector3<T> &trans) {
			DualQuaternion dq;
			dq.real = rot;
			dq.dual = Quaternion(T(0.5) * trans.x, T(0.5) * trans.y, T(0.5) * trans.z, T(0)) * rot;
			return dq;
		}

		Vector3<T> transform_point(const Vector3<T> &p) const {
			Quaternion point(T(0), p.x, p.y, p.z);
			Quaternion result = real * point * real.conjugate() + T(2) * (dual * real.conjugate());
			return Vector3<T>(result.x, result.y, result.z);
		}
	};

	[[nodiscard]] Quaternion scale(T scale_factor) const {
		T new_len = length() * scale_factor;
		T factor = new_len / length();
		return Quaternion(x * factor, y * factor, z * factor, w * factor);
	}

	[[nodiscard]] T distance_to(const Quaternion &rhs) const {
		return angle_between(*this, rhs);
	}

	[[nodiscard]] constexpr MML_FORCE_INLINE bool operator==(const Quaternion &rhs) const {
		return Epsilon<T>::equal(x, rhs.x) &&
				Epsilon<T>::equal(y, rhs.y) &&
				Epsilon<T>::equal(z, rhs.z) &&
				Epsilon<T>::equal(w, rhs.w);
	}

	[[nodiscard]] constexpr MML_FORCE_INLINE bool operator!=(const Quaternion &rhs) const {
		return !(*this == rhs);
	}
};

template <typename T>
[[nodiscard]] MML_FORCE_INLINE Quaternion<T> operator*(T scalar, const Quaternion<T> &q) {
	return q * scalar;
}

using Quaternionf = Quaternion<float>;
using Quaterniond = Quaternion<double>;

} //namespace mml