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

// Quaternion for representing rotations
template <typename T>
struct Quaternion {
	static_assert(std::is_floating_point_v<T>, "Quaternion requires floating-point type");

	// Components: w + xi + yj + zk
	T x, y, z, w;

	// Default constructor
	Quaternion() = default;

	// Explicit identity constructor
	[[nodiscard]] static constexpr Quaternion identity() {
		return Quaternion(T(0), T(0), T(0), T(1));
	}

	// Explicit component constructor
	explicit constexpr Quaternion(T x_, T y_, T z_, T w_) :
			x(x_), y(y_), z(z_), w(w_) {}

	// Explicit constructor from axis and angle (radians, right-handed)
	[[nodiscard]] static Quaternion from_axis_angle(const Vector3<T> &axis, T angle) {
		T half_angle = angle * T(0.5); // half angle for quaternion
		T s = std::sin(half_angle); // sine of half angle
		Vector3<T> normalized_axis = axis.normalized(); // normalized axis
		return Quaternion(
				normalized_axis.x * s,
				normalized_axis.y * s,
				normalized_axis.z * s,
				std::cos(half_angle)); // cosine of half angle
	}

	// Create quaternion from Euler angles (radians, ZYX order)
	[[nodiscard]] static Quaternion from_euler(T pitch, T yaw, T roll) {
		T cy = std::cos(yaw * T(0.5)); // cosine of half yaw
		T sy = std::sin(yaw * T(0.5)); // sine of half yaw
		T cp = std::cos(pitch * T(0.5)); // cosine of half pitch
		T sp = std::sin(pitch * T(0.5)); // sine of half pitch
		T cr = std::cos(roll * T(0.5)); // cosine of half roll
		T sr = std::sin(roll * T(0.5)); // sine of half roll

		return Quaternion(
				sr * cp * cy - cr * sp * sy, // x component
				cr * sp * cy + sr * cp * sy, // y component
				cr * cp * sy - sr * sp * cy, // z component
				cr * cp * cy + sr * sp * sy // w component
		);
	}

	// Create quaternion from Euler angles (Vector3 version)
	[[nodiscard]] static Quaternion from_euler(const Vector3<T> &angles) {
		return from_euler(angles.x, angles.y, angles.z);
	}

	// Create quaternion that rotates from one vector to another
	[[nodiscard]] static Quaternion from_to_rotation(const Vector3<T> &from, const Vector3<T> &to) {
		Vector3<T> from_norm = from.normalized();
		Vector3<T> to_norm = to.normalized();

		T dot_val = from_norm.dot(to_norm);

		// Vectors are parallel
		if (dot_val >= T(1) - Epsilon<T>::value) {
			return identity(); // No rotation needed
		}

		// Vectors are opposite
		if (dot_val <= T(-1) + Epsilon<T>::value) {
			// Find orthogonal vector
			Vector3<T> orth = std::abs(from_norm.x) < std::abs(from_norm.y) ? Vector3<T>::unit_x() : Vector3<T>::unit_y();
			Vector3<T> axis = from_norm.cross(orth).normalized();
			return from_axis_angle(axis, Constants<T>::pi);
		}

		Vector3<T> axis = from_norm.cross(to_norm);
		T w = std::sqrt((T(1) + dot_val) * T(2));
		T inv_w = T(1) / w;

		return Quaternion(axis.x * inv_w, axis.y * inv_w, axis.z * inv_w, w * T(0.5));
	}

	// Create quaternion that rotates from one vector to another (alias for from_to_rotation)
	[[nodiscard]] static Quaternion rotation_between(const Vector3<T> &from, const Vector3<T> &to) {
		return from_to_rotation(from, to);
	}

	// Get angle and axis from quaternion
	[[nodiscard]] void to_axis_angle(Vector3<T> &out_axis, T &out_angle) const {
		out_angle = angle();
		out_axis = axis();
	}

	// Get normalized rotation difference (rotation needed to go from this to other)
	[[nodiscard]] Quaternion rotation_to(const Quaternion &to) const {
		return conjugate() * to;
	}

	// Get normalized rotation difference as angle
	[[nodiscard]] T angle_to(const Quaternion &to) const {
		return rotation_to(to).angle();
	}

	// Create look rotation quaternion (Unity-style)
	[[nodiscard]] static Quaternion look_rotation(const Vector3<T> &forward, const Vector3<T> &up = Vector3<T>::up()) {
		Vector3<T> f = forward.normalized();
		Vector3<T> r = up.cross(f).normalized();
		Vector3<T> u = f.cross(r);

		Matrix3<T> m(r, u, f);
		return from_matrix(m);
	}

	// Create quaternion from rotation matrix
	[[nodiscard]] static Quaternion from_matrix(const Matrix3<T> &m) {
		T trace = m[0][0] + m[1][1] + m[2][2]; // trace of matrix

		if (trace > T(0)) {
			T s = Math<T>::safe_sqrt(trace + T(1)) * T(2); // scale factor
			return Quaternion(
					(m[1][2] - m[2][1]) / s,
					(m[2][0] - m[0][2]) / s,
					(m[0][1] - m[1][0]) / s,
					s * T(0.25));
		} else if (m[0][0] > m[1][1] && m[0][0] > m[2][2]) {
			T s = Math<T>::safe_sqrt(T(1) + m[0][0] - m[1][1] - m[2][2]) * T(2); // scale factor
			return Quaternion(
					s * T(0.25),
					(m[1][0] + m[0][1]) / s,
					(m[2][0] + m[0][2]) / s,
					(m[1][2] - m[2][1]) / s);
		} else if (m[1][1] > m[2][2]) {
			T s = Math<T>::safe_sqrt(T(1) + m[1][1] - m[0][0] - m[2][2]) * T(2); // scale factor
			return Quaternion(
					(m[1][0] + m[0][1]) / s,
					s * T(0.25),
					(m[2][1] + m[1][2]) / s,
					(m[2][0] - m[0][2]) / s);
		} else {
			T s = Math<T>::safe_sqrt(T(1) + m[2][2] - m[0][0] - m[1][1]) * T(2); // scale factor
			return Quaternion(
					(m[2][0] + m[0][2]) / s,
					(m[2][1] + m[1][2]) / s,
					s * T(0.25),
					(m[0][1] - m[1][0]) / s);
		}
	}

	// Explicit copy constructor
	constexpr Quaternion(const Quaternion &other) = default;
	Quaternion &operator=(const Quaternion &other) = default;

	// Addition
	[[nodiscard]] MML_FORCE_INLINE Quaternion operator+(const Quaternion &rhs) const {
		return Quaternion(x + rhs.x, y + rhs.y, z + rhs.z, w + rhs.w);
	}

	// Subtraction
	[[nodiscard]] MML_FORCE_INLINE Quaternion operator-(const Quaternion &rhs) const {
		return Quaternion(x - rhs.x, y - rhs.y, z - rhs.z, w - rhs.w);
	}

	// Quaternion multiplication (Hamilton product)
	[[nodiscard]] MML_FORCE_INLINE Quaternion operator*(const Quaternion &rhs) const {
		return Quaternion(
				w * rhs.x + x * rhs.w + y * rhs.z - z * rhs.y,
				w * rhs.y - x * rhs.z + y * rhs.w + z * rhs.x,
				w * rhs.z + x * rhs.y - y * rhs.x + z * rhs.w,
				w * rhs.w - x * rhs.x - y * rhs.y - z * rhs.z);
	}

	// Scalar multiplication
	[[nodiscard]] MML_FORCE_INLINE Quaternion operator*(T scalar) const {
		return Quaternion(x * scalar, y * scalar, z * scalar, w * scalar);
	}

	// Rotate a vector by this quaternion
	[[nodiscard]] MML_FORCE_INLINE Vector3<T> rotate(const Vector3<T> &v) const {
		Vector3<T> qv(x, y, z); // quaternion vector part
		Vector3<T> t = qv.cross(v) * T(2); // 2 * cross(q, v)
		return v + t * w + qv.cross(t); // v' = v + 2 * cross(q, cross(q, v) + w * v)
	}

	// Dot product
	[[nodiscard]] MML_FORCE_INLINE T dot(const Quaternion &rhs) const {
		return x * rhs.x + y * rhs.y + z * rhs.z + w * rhs.w;
	}

	// Length squared
	[[nodiscard]] MML_FORCE_INLINE T length_squared() const {
		return x * x + y * y + z * z + w * w;
	}

	// Length
	[[nodiscard]] MML_FORCE_INLINE T length() const {
		return Math<T>::safe_sqrt(length_squared());
	}

	// Check if quaternion is zero (near zero length)
	[[nodiscard]] MML_FORCE_INLINE bool is_zero() const {
		return Epsilon<T>::approx_zero(length_squared());
	}

	// Check if quaternion is identity (near identity rotation)
	[[nodiscard]] MML_FORCE_INLINE bool is_identity() const {
		return Epsilon<T>::equal(x, T(0)) && Epsilon<T>::equal(y, T(0)) &&
				Epsilon<T>::equal(z, T(0)) && Epsilon<T>::equal(w, T(1));
	}

	// Check if all components are finite
	[[nodiscard]] MML_FORCE_INLINE bool is_finite() const {
		return std::isfinite(x) && std::isfinite(y) && std::isfinite(z) && std::isfinite(w);
	}

	// Check if quaternion is normalized (unit length)
	[[nodiscard]] MML_FORCE_INLINE bool is_normalized() const {
		return Epsilon<T>::equal(length_squared(), T(1));
	}

	// Normalize (returns normalized copy)
	[[nodiscard]] MML_FORCE_INLINE Quaternion normalized() const {
		T len = length();
		if (Epsilon<T>::approx_zero(len)) {
			return identity(); // return identity for zero-length quaternions
		}
		T inv_len = T(1) / len; // reciprocal of length
		return Quaternion(x * inv_len, y * inv_len, z * inv_len, w * inv_len);
	}

	// Normalize in place
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

	// Conjugate (inverse for unit quaternions)
	[[nodiscard]] MML_FORCE_INLINE Quaternion conjugate() const {
		return Quaternion(-x, -y, -z, w);
	}

	// Inverse
	[[nodiscard]] MML_FORCE_INLINE Quaternion inverse() const {
		T len_sq = length_squared();
		if (Epsilon<T>::approx_zero(len_sq)) {
			return identity(); // return identity for zero-length quaternions
		}
		T inv_len_sq = T(1) / len_sq; // reciprocal of squared length
		return Quaternion(-x * inv_len_sq, -y * inv_len_sq, -z * inv_len_sq, w * inv_len_sq);
	}

	// Convert to 3x3 rotation matrix
	[[nodiscard]] Matrix3<T> to_matrix3() const {
		T xx = x * x, yy = y * y, zz = z * z; // squared components
		T xy = x * y, xz = x * z, yz = y * z; // cross components
		T wx = w * x, wy = w * y, wz = w * z; // w-x components

		return Matrix3<T>(
				T(1) - T(2) * (yy + zz), T(2) * (xy + wz), T(2) * (xz - wy),
				T(2) * (xy - wz), T(1) - T(2) * (xx + zz), T(2) * (yz + wx),
				T(2) * (xz + wy), T(2) * (yz - wx), T(1) - T(2) * (xx + yy));
	}

	// Convert to 4x4 rotation matrix
	[[nodiscard]] Matrix4<T> to_matrix4() const {
		Matrix3<T> m3 = to_matrix3();
		return Matrix4<T>(
				Vector4<T>(m3[0], T(0)),
				Vector4<T>(m3[1], T(0)),
				Vector4<T>(m3[2], T(0)),
				Vector4<T>(T(0), T(0), T(0), T(1)));
	}

	// Spherical linear interpolation
	[[nodiscard]] static Quaternion slerp(const Quaternion &a, const Quaternion &b, T t) {
		T cos_theta = a.dot(b); // cosine of angle between quaternions

		Quaternion b_adjusted = b;
		if (cos_theta < T(0)) {
			b_adjusted = Quaternion(-b.x, -b.y, -b.z, -b.w); // take shortest path
			cos_theta = -cos_theta;
		}

		// Use linear interpolation for very small angles
		if (cos_theta > T(1) - Epsilon<T>::value) {
			return Quaternion(
					a.x + t * (b_adjusted.x - a.x),
					a.y + t * (b_adjusted.y - a.y),
					a.z + t * (b_adjusted.z - a.z),
					a.w + t * (b_adjusted.w - a.w))
					.normalized(); // use lerp for very small angles
		}

		// Compute slerp
		T theta = Math<T>::safe_acos(cos_theta); // angle between quaternions
		T sin_theta = std::sin(theta); // sine of angle
		T wa = std::sin((T(1) - t) * theta) / sin_theta; // weight for a
		T wb = std::sin(t * theta) / sin_theta; // weight for b

		return Quaternion(
				a.x * wa + b_adjusted.x * wb,
				a.y * wa + b_adjusted.y * wb,
				a.z * wa + b_adjusted.z * wb,
				a.w * wa + b_adjusted.w * wb);
	}

	// Spherical cubic interpolation (squad)
	[[nodiscard]] static Quaternion squad(const Quaternion &q0, const Quaternion &q1, const Quaternion &q2, const Quaternion &q3, T t) {
		Quaternion s0 = slerp(q0, q3, t);
		Quaternion s1 = slerp(q1, q2, t);
		return slerp(s0, s1, 2.0 * t * (1.0 - t));
	}

	// Compute intermediate quaternion for squad interpolation
	[[nodiscard]] static Quaternion squad_intermediate(const Quaternion &q_prev, const Quaternion &q_curr, const Quaternion &q_next) {
		Quaternion q_prev_inv = q_prev.conjugate();
		Quaternion q_next_inv = q_next.conjugate();
		Quaternion term1 = q_prev_inv * q_curr;
		Quaternion term2 = q_next_inv * q_curr;
		return q_curr * (term1 + term2).normalized() * T(-0.25);
	}

	// Linear interpolation (non-normalized)
	[[nodiscard]] static Quaternion lerp_unnormalized(const Quaternion &a, const Quaternion &b, T t) {
		return Quaternion(
				a.x + t * (b.x - a.x),
				a.y + t * (b.y - a.y),
				a.z + t * (b.z - a.z),
				a.w + t * (b.w - a.w));
	}

	// Normalized linear interpolation
	[[nodiscard]] static Quaternion nlerp(const Quaternion &a, const Quaternion &b, T t) {
		return lerp_unnormalized(a, b, t).normalized();
	}

	// Exponential of quaternion
	[[nodiscard]] Quaternion exp() const {
		Vector3<T> v(x, y, z); // vector part
		T v_len = v.length(); // length of vector part

		if (Epsilon<T>::approx_zero(v_len)) {
			return identity(); // return identity for zero vector part
		}

		T s = std::sin(v_len) / v_len; // sinc function
		T e = std::exp(w); // exponential of scalar part

		return Quaternion(
				e * s * v.x,
				e * s * v.y,
				e * s * v.z,
				e * std::cos(v_len)); // exponential result
	}

	// Natural logarithm of quaternion
	[[nodiscard]] Quaternion log() const {
		T len = length(); // length of quaternion

		if (Epsilon<T>::approx_zero(len)) {
			return Quaternion(T(0), T(0), T(0), std::numeric_limits<T>::lowest()); // handle zero quaternion
		}

		T v_len = Math<T>::safe_sqrt(x * x + y * y + z * z); // length of vector part

		if (Epsilon<T>::approx_zero(v_len)) {
			return Quaternion(T(0), T(0), T(0), std::log(len)); // real quaternion case
		}

		T coeff = Math<T>::safe_acos(w / len) / v_len; // coefficient for vector part
		return Quaternion(
				coeff * x,
				coeff * y,
				coeff * z,
				std::log(len)); // logarithm result
	}

	// Power of quaternion
	[[nodiscard]] Quaternion pow(T exponent) const {
		T len = length(); // length of quaternion
		if (Epsilon<T>::approx_zero(len)) {
			return identity(); // return identity for zero-length quaternions
		}

		T alpha = Math<T>::safe_acos(w / len); // angle of rotation
		T new_alpha = alpha * exponent; // scaled angle
		T s = std::sin(new_alpha) / std::sin(alpha); // scale factor

		return Quaternion(
				s * x,
				s * y,
				s * z,
				std::cos(new_alpha)); // power result
	}

	// Negate quaternion (represents same rotation)
	[[nodiscard]] Quaternion negate() const {
		return Quaternion(-x, -y, -z, -w);
	}

	// Get rotation angle (in radians)
	[[nodiscard]] T angle() const {
		return T(2) * Math<T>::safe_acos(std::abs(w) / length());
	}

	// Get rotation axis
	[[nodiscard]] Vector3<T> axis() const {
		T s = Math<T>::safe_sqrt(T(1) - w * w);
		if (Epsilon<T>::approx_zero(s)) {
			return Vector3<T>::unit_x();
		}
		return Vector3<T>(x, y, z) / s;
	}

	// Get Euler angles (ZYX order, in radians)
	[[nodiscard]] Vector3<T> to_euler() const {
		T sinr_cosp = T(2) * (w * x + y * z); // sine of roll times cosine of pitch
		T cosr_cosp = T(1) - T(2) * (x * x + y * y); // cosine of roll times cosine of pitch
		T roll = std::atan2(sinr_cosp, cosr_cosp); // roll angle

		T sinp = T(2) * (w * y - z * x); // sine of pitch
		T pitch;
		if (std::abs(sinp) >= T(1)) {
			pitch = std::copysign(Constants<T>::half_pi, sinp); // handle gimbal lock
		} else {
			pitch = Math<T>::safe_asin(sinp); // normal case
		}

		T siny_cosp = T(2) * (w * z + x * y); // sine of yaw times cosine of pitch
		T cosy_cosp = T(1) - T(2) * (y * y + z * z); // cosine of yaw times cosine of pitch
		T yaw = std::atan2(siny_cosp, cosy_cosp); // yaw angle

		return Vector3<T>(pitch, yaw, roll);
	}

	// Get forward vector (assuming quaternion represents rotation)
	[[nodiscard]] Vector3<T> forward() const {
		return rotate(Vector3<T>::forward());
	}

	// Get up vector (assuming quaternion represents rotation)
	[[nodiscard]] Vector3<T> up() const {
		return rotate(Vector3<T>::up());
	}

	// Get right vector (assuming quaternion represents rotation)
	[[nodiscard]] Vector3<T> right() const {
		return rotate(Vector3<T>::right());
	}

	// Angle between two quaternions (in radians)
	[[nodiscard]] static T angle_between(const Quaternion &a, const Quaternion &b) {
		T dot_val = a.dot(b);
		return T(2) * Math<T>::safe_acos(std::abs(dot_val));
	}

	// Angular distance between two quaternions
	[[nodiscard]] static T angular_distance(const Quaternion &a, const Quaternion &b) {
		Quaternion diff = b.inverse() * a; // relative rotation
		return diff.angle(); // angle of relative rotation
	}

	// Minimum rotation between two quaternions (chooses shortest path)
	[[nodiscard]] static Quaternion shortest_path(const Quaternion &a, const Quaternion &b) {
		if (a.dot(b) < T(0)) {
			return Quaternion(-b.x, -b.y, -b.z, -b.w);
		}
		return b;
	}

	// Check if quaternion is valid (length is approximately 1)
	[[nodiscard]] bool is_valid() const {
		return Epsilon<T>::equal(length(), T(1));
	}

	// Get swing-twist decomposition
	void swing_twist(const Vector3<T> &twist_axis, Quaternion &out_swing, Quaternion &out_twist) const {
		Vector3<T> r(x, y, z); // vector part
		T p = r.dot(twist_axis); // projection onto twist axis
		out_twist = Quaternion(twist_axis * p, w).normalized(); // twist component
		out_swing = *this * out_twist.conjugate(); // swing component
	}

	// Get roll, pitch, yaw as separate quaternions
	[[nodiscard]] Quaternion roll_quat() const {
		return Quaternion(x, T(0), T(0), w).normalized();
	}

	[[nodiscard]] Quaternion pitch_quat() const {
		return Quaternion(T(0), y, T(0), w).normalized();
	}

	[[nodiscard]] Quaternion yaw_quat() const {
		return Quaternion(T(0), T(0), z, w).normalized();
	}

	// Dual quaternion support (for rigid transformations)
	struct DualQuaternion {
		Quaternion real; // real part (rotation)
		Quaternion dual; // dual part (translation)

		static DualQuaternion from_rotation_translation(const Quaternion &rot, const Vector3<T> &trans) {
			DualQuaternion dq;
			dq.real = rot; // rotation
			dq.dual = Quaternion(T(0.5) * trans.x, T(0.5) * trans.y, T(0.5) * trans.z, T(0)) * rot; // translation
			return dq;
		}

		Vector3<T> transform_point(const Vector3<T> &p) const {
			Quaternion point(T(0), p.x, p.y, p.z); // point as pure quaternion
			Quaternion result = real * point * real.conjugate() + T(2) * (dual * real.conjugate()); // transform
			return Vector3<T>(result.x, result.y, result.z); // transformed point
		}
	};

	// Scale quaternion
	[[nodiscard]] Quaternion scale(T scale_factor) const {
		T new_len = length() * scale_factor; // new length
		T factor = new_len / length(); // scale factor
		return Quaternion(x * factor, y * factor, z * factor, w * factor); // scaled quaternion
	}

	// Distance between two quaternions
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

// Non-member scalar multiplication (scalar * quaternion)
template <typename T>
[[nodiscard]] MML_FORCE_INLINE Quaternion<T> operator*(T scalar, const Quaternion<T> &q) {
	return q * scalar;
}

// Type aliases
using Quaternionf = Quaternion<float>;
using Quaterniond = Quaternion<double>;

} // namespace mml