/**************************************************************************/
/*  aabb.hpp                                                              */
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
#include "../algebra/vector3.hpp"
#include "../core/config.hpp"
#include "../core/epsilon.hpp"
#include "ray.hpp"
#include <algorithm>
#include <cmath>
#include <limits>
#include <type_traits>

namespace mml {

template <typename T>
struct AABB {
	static_assert(std::is_floating_point_v<T>, "AABB requires floating-point type");

	Vector3<T> min;
	Vector3<T> max;

	AABB() = default;

	constexpr AABB(const Vector3<T> &min_, const Vector3<T> &max_) :
			min(min_), max(max_) {}

	static constexpr AABB from_center_extents(const Vector3<T> &center, const Vector3<T> &half_extents) {
		return AABB(center - half_extents, center + half_extents);
	}

	static constexpr AABB empty() {
		return AABB(
				Vector3<T>(std::numeric_limits<T>::max(), std::numeric_limits<T>::max(), std::numeric_limits<T>::max()),
				Vector3<T>(std::numeric_limits<T>::lowest(), std::numeric_limits<T>::lowest(), std::numeric_limits<T>::lowest()));
	}

	constexpr bool is_empty() const {
		return min.x > max.x || min.y > max.y || min.z > max.z;
	}

	AABB(const AABB &other) = default;
	AABB &operator=(const AABB &other) = default;

	constexpr MML_FORCE_INLINE Vector3<T> center() const {
		return (min + max) * T(0.5);
	}

	constexpr MML_FORCE_INLINE Vector3<T> half_extents() const {
		return (max - min) * T(0.5);
	}

	constexpr MML_FORCE_INLINE Vector3<T> extents() const {
		return max - min;
	}

	constexpr MML_FORCE_INLINE T volume() const {
		Vector3<T> size = extents();
		return size.x * size.y * size.z;
	}

	constexpr MML_FORCE_INLINE T surface_area() const {
		Vector3<T> size = extents();
		return T(2) * (size.x * size.y + size.y * size.z + size.z * size.x);
	}

	constexpr MML_FORCE_INLINE bool contains(const Vector3<T> &point) const {
		return point.x >= min.x && point.x <= max.x &&
				point.y >= min.y && point.y <= max.y &&
				point.z >= min.z && point.z <= max.z;
	}

	constexpr MML_FORCE_INLINE bool contains(const AABB &other) const {
		return contains(other.min) && contains(other.max);
	}

	constexpr MML_FORCE_INLINE bool intersects(const AABB &other) const {
		return min.x <= other.max.x && max.x >= other.min.x &&
				min.y <= other.max.y && max.y >= other.min.y &&
				min.z <= other.max.z && max.z >= other.min.z;
	}

	MML_FORCE_INLINE bool intersect_ray(const Ray<T> &ray, T &out_t_near, T &out_t_far) const {
		T t_near = -std::numeric_limits<T>::infinity();
		T t_far = std::numeric_limits<T>::infinity();

		for (size_t i = 0; i < 3; ++i) {
			if (Epsilon<T>::approx_zero(ray.direction[i])) {
				if (ray.origin[i] < min[i] || ray.origin[i] > max[i]) {
					return false;
				}
			} else {
				T inv_d = T(1) / ray.direction[i];
				T t1 = (min[i] - ray.origin[i]) * inv_d;
				T t2 = (max[i] - ray.origin[i]) * inv_d;

				if (t1 > t2) {
					std::swap(t1, t2);
				}

				t_near = std::max(t_near, t1);
				t_far = std::min(t_far, t2);

				if (t_near > t_far) {
					return false;
				}
			}
		}

		out_t_near = t_near;
		out_t_far = t_far;
		return true;
	}

	MML_FORCE_INLINE bool intersect_ray(const Ray<T> &ray, Vector3<T> &out_point) const {
		T t_near, t_far;
		if (intersect_ray(ray, t_near, t_far)) {
			T t = (t_near >= T(0)) ? t_near : t_far;
			if (t >= T(0)) {
				out_point = ray.at(t);
				return true;
			}
		}
		return false;
	}

	MML_FORCE_INLINE void expand(const Vector3<T> &point) {
		min.x = std::min(min.x, point.x);
		min.y = std::min(min.y, point.y);
		min.z = std::min(min.z, point.z);
		max.x = std::max(max.x, point.x);
		max.y = std::max(max.y, point.y);
		max.z = std::max(max.z, point.z);
	}

	MML_FORCE_INLINE void expand(const AABB &other) {
		expand(other.min);
		expand(other.max);
	}

	static constexpr AABB merge(const AABB &a, const AABB &b) {
		return AABB(
				Vector3<T>(
						std::min(a.min.x, b.min.x),
						std::min(a.min.y, b.min.y),
						std::min(a.min.z, b.min.z)),
				Vector3<T>(
						std::max(a.max.x, b.max.x),
						std::max(a.max.y, b.max.y),
						std::max(a.max.z, b.max.z)));
	}

	constexpr MML_FORCE_INLINE Vector3<T> closest_point(const Vector3<T> &point) const {
		return Vector3<T>(
				(point.x < min.x) ? min.x : (point.x > max.x) ? max.x
															  : point.x,
				(point.y < min.y) ? min.y : (point.y > max.y) ? max.y
															  : point.y,
				(point.z < min.z) ? min.z : (point.z > max.z) ? max.z
															  : point.z);
	}

	[[nodiscard]] MML_FORCE_INLINE AABB transformed(const Matrix4<T> &matrix) const {
		Vector3<T> center = (min + max) * T(0.5);
		Vector3<T> extents = (max - min) * T(0.5);

		Vector3<T> new_center = matrix.transform_point(center);

		Vector3<T> new_extents;
		new_extents.x = std::abs(matrix[0][0]) * extents.x + std::abs(matrix[1][0]) * extents.y + std::abs(matrix[2][0]) * extents.z;
		new_extents.y = std::abs(matrix[0][1]) * extents.x + std::abs(matrix[1][1]) * extents.y + std::abs(matrix[2][1]) * extents.z;
		new_extents.z = std::abs(matrix[0][2]) * extents.x + std::abs(matrix[1][2]) * extents.y + std::abs(matrix[2][2]) * extents.z;

		return AABB(new_center - new_extents, new_center + new_extents);
	}

	MML_FORCE_INLINE bool contains_sphere(const Vector3<T> &sphere_center, T sphere_radius) const {
		Vector3<T> r(sphere_radius, sphere_radius, sphere_radius);
		AABB sphere_aabb(sphere_center - r, sphere_center + r);
		return contains(sphere_aabb);
	}

	MML_FORCE_INLINE bool intersects_sphere(const Vector3<T> &sphere_center, T sphere_radius) const {
		Vector3<T> closest = closest_point(sphere_center);
		return sphere_center.distance_squared(closest) <= sphere_radius * sphere_radius;
	}

	friend constexpr MML_FORCE_INLINE AABB operator|(const AABB &a, const AABB &b) {
		return merge(a, b);
	}
};

using AABBf = AABB<float>;
using AABBd = AABB<double>;

} //namespace mml