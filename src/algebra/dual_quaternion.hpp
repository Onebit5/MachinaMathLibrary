/**************************************************************************/
/*  dual_quaternion.hpp                                                   */
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
		Matrix4<T> result;

		Quaternion<T> r = real.normalized();
		Quaternion<T> d = dual.normalized();

		T wx = r.w * r.x, wy = r.w * r.y, wz = r.w * r.z;
		T xx = r.x * r.x, xy = r.x * r.y, xz = r.x * r.z;
		T yy = r.y * r.y, yz = r.y * r.z, zz = r.z * r.z;
		T tx = T(2) * (d.w * r.x - d.x * r.w + d.y * r.z - d.z * r.y);
		T ty = T(2) * (d.w * r.y - d.x * r.z - d.y * r.w + d.z * r.x);
		T tz = T(2) * (d.w * r.z + d.x * r.y - d.y * r.x - d.z * r.w);

		result[0][0] = T(1) - T(2) * (yy + zz);
		result[0][1] = T(2) * (xy + wz);
		result[0][2] = T(2) * (xz - wy);
		result[0][3] = T(0);

		result[1][0] = T(2) * (xy - wz);
		result[1][1] = T(1) - T(2) * (xx + zz);
		result[1][2] = T(2) * (yz + wx);
		result[1][3] = T(0);

		result[2][0] = T(2) * (xz + wy);
		result[2][1] = T(2) * (yz - wx);
		result[2][2] = T(1) - T(2) * (xx + yy);
		result[2][3] = T(0);

		result[3][0] = tx;
		result[3][1] = ty;
		result[3][2] = tz;
		result[3][3] = T(1);

		return result;
	}

	[[nodiscard]] Vector3<T> transform_point(const Vector3<T> &p) const {
		Quaternion<T> p_quat(T(0), p.x, p.y, p.z);
		Quaternion<T> transformed = real * p_quat * real.conjugate() +
				T(2) * (dual * real.conjugate() - real * dual.conjugate());
		return Vector3<T>(transformed.x, transformed.y, transformed.z);
	}

	[[nodiscard]] Vector3<T> transform_direction(const Vector3<T> &d) const {
		Quaternion<T> d_quat(T(0), d.x, d.y, d.z);
		Quaternion<T> transformed = real * d_quat * real.conjugate();
		return Vector3<T>(transformed.x, transformed.y, transformed.z);
	}

	[[nodiscard]] DualQuaternion operator*(const DualQuaternion &rhs) const {
		return DualQuaternion(real * rhs.real, real * rhs.dual + dual * rhs.real);
	}

	[[nodiscard]] DualQuaternion conjugate() const {
		return DualQuaternion(real.conjugate(), -dual.conjugate());
	}

	[[nodiscard]] DualQuaternion inverse() const {
		Quaternion<T> r_conj = real.conjugate();
		T r_norm_sq = real.length_squared();
		return DualQuaternion(r_conj, (r_conj * dual * T(-1)) / r_norm_sq);
	}

	[[nodiscard]] DualQuaternion normalized() const {
		T norm = real.length();
		return DualQuaternion(real / norm, dual / norm);
	}

	[[nodiscard]] T length() const {
		return real.length();
	}

	[[nodiscard]] bool is_normalized() const {
		return Epsilon<T>::equal(real.length_squared(), T(1));
	}

	[[nodiscard]] static DualQuaternion identity() {
		return DualQuaternion(Quaternion<T>::identity(), Quaternion<T>(T(0), T(0), T(0), T(0)));
	}

	[[nodiscard]] static DualQuaternion lerp(const DualQuaternion &a, const DualQuaternion &b, T t) {
		return DualQuaternion(
				Quaternion<T>::lerp_unnormalized(a.real, b.real, t),
				Quaternion<T>::lerp_unnormalized(a.dual, b.dual, t))
				.normalized();
	}

	[[nodiscard]] static DualQuaternion slerp(const DualQuaternion &a, DualQuaternion b, T t) {
		T cos_theta = a.real.dot(b.real);
		if (cos_theta < T(0)) {
			b.real = Quaternion<T>(-b.real.x, -b.real.y, -b.real.z, -b.real.w);
			b.dual = Quaternion<T>(-b.dual.x, -b.dual.y, -b.dual.z, -b.dual.w);
			cos_theta = -cos_theta;
		}

		T ratio_a, ratio_b;
		if (cos_theta > T(1) - Epsilon<T>::value) {
			ratio_a = T(1) - t;
			ratio_b = t;
		} else {
			T theta = Math<T>::safe_acos(cos_theta);
			T sin_theta = std::sin(theta);
			ratio_a = std::sin((T(1) - t) * theta) / sin_theta;
			ratio_b = std::sin(t * theta) / sin_theta;
		}

		DualQuaternion result;
		result.real = a.real * ratio_a + b.real * ratio_b;
		result.dual = a.dual * ratio_a + b.dual * ratio_b;
		return result.normalized();
	}

	[[nodiscard]] Vector3<T> translation() const {
		T wx = real.w * real.x, wy = real.w * real.y, wz = real.w * real.z;
		T xx = real.x * real.x, xy = real.x * real.y, xz = real.x * real.z;
		T yy = real.y * real.y, yz = real.y * real.z, zz = real.z * real.z;

		return Vector3<T>(
				T(2) * (dual.x * real.w - dual.w * real.x + dual.y * real.z - dual.z * real.y),
				T(2) * (dual.y * real.w - dual.w * real.y + dual.z * real.x - dual.x * real.z),
				T(2) * (dual.z * real.w - dual.w * real.z + dual.x * real.y - dual.y * real.x));
	}

	void set_translation(const Vector3<T> &t) {
		dual = Quaternion<T>(
					   t.x * T(0.5),
					   t.y * T(0.5),
					   t.z * T(0.5),
					   T(0)) *
				real;
	}
};

template <typename T>
struct Bone {
	Vector3<T> position;
	Quaternion<T> rotation;
	T scale;
	std::vector<size_t> children;
	size_t parent_index;

	Bone() :
			position(Vector3<T>::zero()), rotation(Quaternion<T>::identity()), scale(T(1)), parent_index(std::numeric_limits<size_t>::max()) {}
};

template <typename T>
struct Skeleton {
	std::vector<Bone<T>> bones;
	std::vector<DualQuaternion<T>> bind_pose_dqs;
	std::vector<DualQuaternion<T>> inverse_bind_pose_dqs;
	std::vector<Matrix4<T>> global_transforms;
	std::vector<Matrix4<T>> local_transforms;

	void initialize() {
		size_t num_bones = bones.size();
		bind_pose_dqs.resize(num_bones);
		inverse_bind_pose_dqs.resize(num_bones);
		global_transforms.resize(num_bones);
		local_transforms.resize(num_bones);

		for (size_t i = 0; i < num_bones; ++i) {
			const Bone<T> &bone = bones[i];
			DualQuaternion<T> dq = DualQuaternion<T>::from_rotation_translation(bone.rotation, bone.position);
			bind_pose_dqs[i] = dq;
			inverse_bind_pose_dqs[i] = dq.inverse();
		}

		update_global_transforms();
	}

	void update_global_transforms() {
		size_t num_bones = bones.size();
		for (size_t i = 0; i < num_bones; ++i) {
			const Bone<T> &bone = bones[i];
			local_transforms[i] = DualQuaternion<T>::from_rotation_translation(bone.rotation, bone.position).to_matrix4();
		}

		for (size_t i = 0; i < num_bones; ++i) {
			if (bones[i].parent_index == std::numeric_limits<size_t>::max()) {
				global_transforms[i] = local_transforms[i];
			} else {
				global_transforms[i] = global_transforms[bones[i].parent_index] * local_transforms[i];
			}
		}
	}

	[[nodiscard]] std::vector<Matrix4<T>> get_skinning_matrices() const {
		size_t num_bones = bones.size();
		std::vector<Matrix4<T>> skinning(num_bones);

		for (size_t i = 0; i < num_bones; ++i) {
			Matrix4<T> skinMatrix = global_transforms[i] * bind_pose_dqs[i].to_matrix4() * inverse_bind_pose_dqs[i].to_matrix4();
			skinning[i] = skinMatrix;
		}

		return skinning;
	}

	void set_pose(const std::vector<Quaternion<T>> &rotations, const std::vector<Vector3<T>> &positions, const std::vector<T> &scales) {
		size_t num_bones = bones.size();
		for (size_t i = 0; i < num_bones; ++i) {
			if (i < rotations.size()) {
				bones[i].rotation = rotations[i];
			}
			if (i < positions.size()) {
				bones[i].position = positions[i];
			}
			if (i < scales.size()) {
				bones[i].scale = scales[i];
			}
		}
		update_global_transforms();
	}
};

using DualQuaternionf = DualQuaternion<float>;
using DualQuaterniond = DualQuaternion<double>;
using Bonef = Bone<float>;
using Boned = Bone<double>;
using Skeletonf = Skeleton<float>;
using Skeletond = Skeleton<double>;

} // namespace mml