/**************************************************************************/
/*  cone.hpp                                                              */
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
struct Cone {
	static_assert(std::is_floating_point_v<T>, "Cone requires floating-point type");

	Vector3<T> apex;
	Vector3<T> base_center;
	T base_radius;

	Cone() = default;

	explicit Cone(const Vector3<T> &apex_, const Vector3<T> &base_center_, T radius_) :
			apex(apex_), base_center(base_center_), base_radius(radius_) {}

	constexpr Cone(const Cone &other) = default;
	Cone &operator=(const Cone &other) = default;

	MML_FORCE_INLINE Vector3<T> axis() const {
		return (base_center - apex).normalized();
	}

	MML_FORCE_INLINE T height() const {
		return apex.distance(base_center);
	}

	MML_FORCE_INLINE T volume() const {
		return (T(1) / T(3)) * Constants<T>::pi * base_radius * base_radius * height(); // V = (1/3)πr²h
	}

	MML_FORCE_INLINE T surface_area() const {
		T h = height(); // cone height
		T slant = Math<T>::safe_sqrt(h * h + base_radius * base_radius); // slant height
		T base_area = Constants<T>::pi * base_radius * base_radius; // πr²
		T side_area = Constants<T>::pi * base_radius * slant; // πrl (l = slant)
		return base_area + side_area; // total surface area
	}

	MML_FORCE_INLINE T aperture_angle() const {
		return T(2) * std::atan(base_radius / height()); // full cone angle
	}

	MML_FORCE_INLINE bool contains(const Vector3<T> &point) const {
		Vector3<T> to_point = point - apex; // vector from apex to point
		Vector3<T> axis_ = axis(); // cone axis
		T proj = to_point.dot(axis_); // projection onto axis

		if (proj < T(0) || proj > height()) {
			return false; // point outside height range
		}

		T radius_at_height = (proj / height()) * base_radius; // radius at this height
		Vector3<T> projected = apex + axis_ * proj; // point projected onto axis
		return projected.distance_squared(point) <= radius_at_height * radius_at_height; // check if within radius
	}

	MML_FORCE_INLINE Vector3<T> closest_point(const Vector3<T> &point) const {
		Vector3<T> to_point = point - apex; // vector from apex to point
		Vector3<T> axis_ = axis(); // cone axis
		T proj = to_point.dot(axis_); // projection onto axis

		if (proj <= T(0)) {
			return apex; // point is above apex
		}

		if (proj >= height()) {
			Vector3<T> to_base = point - base_center; // vector to point from base center
			T dist = to_base.length();
			if (Epsilon<T>::approx_zero(dist)) {
				return base_center; // point at base center
			}
			return base_center + to_base.normalized() * base_radius; // closest point on base rim
		}

		T radius_at_height = (proj / height()) * base_radius; // radius at this height
		Vector3<T> projected = apex + axis_ * proj; // point projected onto axis
		Vector3<T> to_surface = point - projected; // vector to surface
		T dist = to_surface.length();

		if (dist <= radius_at_height) {
			return projected + to_surface; // point inside cone
		}

		return projected + to_surface.normalized() * radius_at_height; // closest point on surface
	}

	MML_FORCE_INLINE T distance_squared(const Vector3<T> &point) const {
		Vector3<T> closest = closest_point(point);
		return (point - closest).length_squared();
	}

	MML_FORCE_INLINE T distance(const Vector3<T> &point) const {
		return Math<T>::safe_sqrt(distance_squared(point));
	}

	MML_FORCE_INLINE AABB<T> bounding_box() const {
		Vector3<T> axis_ = axis(); // cone axis
		T h = height(); // cone height

		Vector3<T> perp1, perp2;
		if (std::abs(axis_.x) > T(0.9)) {
			perp1 = Vector3<T>(T(0), T(1), T(0)); // use y-axis if axis is mostly x
		} else {
			perp1 = Vector3<T>(T(1), T(0), T(0)); // use x-axis otherwise
		}
		perp1 = perp1 - axis_ * perp1.dot(axis_); // orthogonalize to axis
		perp1 = perp1.normalized(); // normalize
		perp2 = axis_.cross(perp1).normalized(); // second orthogonal vector

		Vector3<T> base_extent = perp1 * base_radius + perp2 * base_radius; // base extent

		Vector3<T> min_v = (apex).min(base_center - base_extent);
		Vector3<T> max_v = (apex).max(base_center + base_extent);

		return AABB<T>(min_v, max_v);
	}

	MML_FORCE_INLINE static Cone from_apex_axis_radius(const Vector3<T> &apex_, const Vector3<T> &axis_, T height_, T radius_) {
		Vector3<T> base = apex_ + axis_.normalized() * height_;
		return Cone(apex_, base, radius_);
	}
};

using Conef = Cone<float>;
using Coned = Cone<double>;

} // namespace mml