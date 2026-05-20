/**************************************************************************/
/*  cylinder.hpp                                                          */
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

#include "../algebra/vector3.hpp"
#include "../core/config.hpp"
#include "../core/constants.hpp"
#include "../core/epsilon.hpp"
#include "../utils/math.hpp"
#include "aabb.hpp"
#include <cmath>
#include <type_traits>

namespace mml {

template <typename T>
struct Cylinder {
	static_assert(std::is_floating_point_v<T>, "Cylinder requires floating-point type");

	Vector3<T> center;
	T radius;
	T height;
	Vector3<T> axis;

	Cylinder() = default;

	explicit Cylinder(const Vector3<T> &center_, T radius_, T height_, const Vector3<T> &axis_ = Vector3<T>::up()) :
			center(center_), radius(radius_), height(height_), axis(axis_.normalized()) {}

	constexpr Cylinder(const Cylinder &other) = default;
	Cylinder &operator=(const Cylinder &other) = default;

	MML_FORCE_INLINE T volume() const {
		return Constants<T>::pi * radius * radius * height;
	}

	MML_FORCE_INLINE T surface_area() const {
		T side_area = T(2) * Constants<T>::pi * radius * height;
		T cap_area = T(2) * Constants<T>::pi * radius * radius;
		return side_area + cap_area;
	}

	MML_FORCE_INLINE Vector3<T> top_center() const {
		return center + axis * (height * T(0.5));
	}

	MML_FORCE_INLINE Vector3<T> bottom_center() const {
		return center - axis * (height * T(0.5));
	}

	MML_FORCE_INLINE bool contains(const Vector3<T> &point) const {
		Vector3<T> to_point = point - center;
		T proj = to_point.dot(axis);

		if (std::abs(proj) > height * T(0.5)) {
			return false;
		}

		Vector3<T> projected = center + axis * proj;
		return projected.distance_squared(point) <= radius * radius;
	}

	MML_FORCE_INLINE Vector3<T> closest_point(const Vector3<T> &point) const {
		Vector3<T> to_point = point - center;
		T proj = to_point.dot(axis);

		T half_height = height * T(0.5);
		T clamped_proj = proj < -half_height ? -half_height : (proj > half_height ? half_height : proj);

		Vector3<T> projected_center = center + axis * clamped_proj;
		Vector3<T> to_surface = point - projected_center;
		T dist = to_surface.length();

		if (Epsilon<T>::approx_zero(dist)) {
			return projected_center + axis * half_height;
		}

		return projected_center + to_surface.normalized() * radius;
	}

	MML_FORCE_INLINE T distance_squared(const Vector3<T> &point) const {
		Vector3<T> closest = closest_point(point);
		return (point - closest).length_squared();
	}

	MML_FORCE_INLINE T distance(const Vector3<T> &point) const {
		return Math<T>::safe_sqrt(distance_squared(point));
	}

	MML_FORCE_INLINE AABB<T> bounding_box() const {
		Vector3<T> half_height = axis * (height * T(0.5));
		Vector3<T> top = center + half_height;
		Vector3<T> bottom = center - half_height;

		Vector3<T> perp1, perp2;
		if (std::abs(axis.x) > T(0.9)) {
			perp1 = Vector3<T>(T(0), T(1), T(0));
		} else {
			perp1 = Vector3<T>(T(1), T(0), T(0));
		}
		perp1 = perp1 - axis * perp1.dot(axis);
		perp1 = perp1.normalized();
		perp2 = axis.cross(perp1).normalized();

		Vector3<T> extent = perp1 * radius + perp2 * radius;

		Vector3<T> min_v = (top - extent).min(bottom - extent);
		Vector3<T> max_v = (top + extent).max(bottom + extent);

		return AABB<T>(min_v, max_v);
	}

	MML_FORCE_INLINE static Cylinder from_top_bottom(const Vector3<T> &top, const Vector3<T> &bottom, T radius_) {
		Vector3<T> center = (top + bottom) * T(0.5);
		Vector3<T> axis_ = (top - bottom).normalized();
		T height_ = top.distance(bottom);
		return Cylinder(center, radius_, height_, axis_);
	}
};

using Cylinderf = Cylinder<float>;
using Cylinderd = Cylinder<double>;

} //namespace mml