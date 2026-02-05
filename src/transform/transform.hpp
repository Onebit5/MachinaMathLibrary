/**************************************************************************/
/*  transform.hpp                                                         */
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

#include "../algebra/matrix4.hpp"
#include "../algebra/quaternion.hpp"
#include "../algebra/vector3.hpp"
#include "../core/config.hpp"
#include "../core/epsilon.hpp"
#include <type_traits>

namespace mml {

// 3D transformation (position, rotation, scale)
template <typename T>
struct Transform {
	static_assert(std::is_floating_point_v<T>, "Transform requires floating-point type");

	Vector3<T> position;
	Quaternion<T> rotation;
	Vector3<T> scale;

	// Default constructor
	Transform() :
			position(Vector3<T>(T(0), T(0), T(0))), rotation(Quaternion<T>::identity()), scale(Vector3<T>(T(1), T(1), T(1))) {}

	// Explicit constructor
	explicit Transform(const Vector3<T> &pos, const Quaternion<T> &rot, const Vector3<T> &scl) :
			position(pos), rotation(rot), scale(scl) {}

	// Explicit copy constructor
	Transform(const Transform &other) = default;
	Transform &operator=(const Transform &other) = default;

	// Create identity transform
	static Transform identity() {
		return Transform();
	}

	// Convert to 4x4 transformation matrix (TRS order)
	Matrix4<T> to_matrix() const {
		// Scale matrix
		Matrix4<T> s = Matrix4<T>::scale(scale);

		// Rotation matrix
		Matrix4<T> r = rotation.to_matrix4();

		// Translation matrix
		Matrix4<T> t = Matrix4<T>::translation(position);

		// Combine: T * R * S (right-to-left multiplication)
		return t * r * s;
	}

	// Create transform from matrix (decomposition)
	static Transform from_matrix(const Matrix4<T> &m) {
		Transform result;

		// Extract translation
		result.position = Vector3<T>(m[3][0], m[3][1], m[3][2]);

		// Extract scale
		Vector3<T> scale_x = Vector3<T>(m[0][0], m[0][1], m[0][2]);
		Vector3<T> scale_y = Vector3<T>(m[1][0], m[1][1], m[1][2]);
		Vector3<T> scale_z = Vector3<T>(m[2][0], m[2][1], m[2][2]);

		result.scale = Vector3<T>(
				scale_x.length(),
				scale_y.length(),
				scale_z.length());

		// Extract rotation (remove scale from rotation matrix)
		Matrix3<T> rotate_matrix(
				scale_x / result.scale.x,
				scale_y / result.scale.y,
				scale_z / result.scale.z);

		result.rotation = Quaternion<T>::from_matrix(rotate_matrix);

		return result;
	}

	// Transform a point (apply scale, rotation, translation)
	MML_FORCE_INLINE Vector3<T> transform_point(const Vector3<T> &point) const {
		Vector3<T> scaled = Vector3<T>(point.x * scale.x, point.y * scale.y, point.z * scale.z);
		Vector3<T> rotated = rotation.rotate(scaled);
		return rotated + position;
	}

	// Transform a direction (apply rotation only)
	MML_FORCE_INLINE Vector3<T> transform_direction(const Vector3<T> &direction) const {
		return rotation.rotate(direction);
	}

	// Combine two transforms (this * other)
	MML_FORCE_INLINE Transform operator*(const Transform &other) const {
		Transform result;
		result.position = position + rotation.rotate(Vector3<T>(other.position.x * scale.x, other.position.y * scale.y, other.position.z * scale.z));
		result.rotation = rotation * other.rotation;
		result.scale = Vector3<T>(
				scale.x * other.scale.x,
				scale.y * other.scale.y,
				scale.z * other.scale.z);
		return result;
	}

	// Inverse transform with zero-scale protection
	Transform inverse() const {
		Transform result;

		// Inverse rotation
		result.rotation = rotation.conjugate();

		// Inverse scale with zero-check protection
		result.scale = Vector3<T>(
				Epsilon<T>::is_zero(scale.x) ? T(0) : T(1) / scale.x,
				Epsilon<T>::is_zero(scale.y) ? T(0) : T(1) / scale.y,
				Epsilon<T>::is_zero(scale.z) ? T(0) : T(1) / scale.z);

		// Inverse translation
		Vector3<T> inv_pos = -position;
		Vector3<T> rotated = result.rotation.rotate(inv_pos);
		result.position = Vector3<T>(
				rotated.x * result.scale.x,
				rotated.y * result.scale.y,
				rotated.z * result.scale.z);

		return result;
	}

	// Interpolate between two transforms
	static Transform lerp(const Transform &a, const Transform &b, T t) {
		Transform result;
		result.position = a.position + (b.position - a.position) * t;
		result.rotation = Quaternion<T>::slerp(a.rotation, b.rotation, t);
		result.scale = a.scale + (b.scale - a.scale) * t;
		return result;
	}

	// Look at target (useful for cameras)
	static Transform look_at(const Vector3<T> &position, const Vector3<T> &target, const Vector3<T> &up) {
		Vector3<T> forward = (target - position).normalized();
		Vector3<T> right = forward.cross(up).normalized();
		Vector3<T> new_up = right.cross(forward);

		// Create rotation matrix from basis vectors
		Matrix3<T> rotate_matrix(right, new_up, -forward); // negative forward for right-handed system

		Transform result;
		result.position = position;
		result.rotation = Quaternion<T>::from_matrix(rotate_matrix);
		result.scale = Vector3<T>(T(1), T(1), T(1));

		return result;
	}
};

// Type aliases
using Transformf = Transform<float>;
using Transformd = Transform<double>;

} // namespace mml