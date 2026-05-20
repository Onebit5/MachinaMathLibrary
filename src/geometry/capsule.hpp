/**************************************************************************/
/*  capsule.hpp                                                           */
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
#include "sphere.hpp"
#include <cmath>
#include <type_traits>

namespace mml {

template <typename T>
struct Capsule {
	static_assert(std::is_floating_point_v<T>, "Capsule requires floating-point type");

	Vector3<T> a;
	Vector3<T> b;
	T radius;

	Capsule() = default;

	explicit Capsule(const Vector3<T> &a_, const Vector3<T> &b_, T radius_) :
			a(a_), b(b_), radius(radius_) {}

	constexpr Capsule(const Capsule &other) = default;
	Capsule &operator=(const Capsule &other) = default;

	MML_FORCE_INLINE Vector3<T> center() const {
		return (a + b) * T(0.5);
	}

	MML_FORCE_INLINE T height() const {
		return a.distance(b);
	}

	MML_FORCE_INLINE T volume() const {
		T h = height();
		T sphere_vol = (T(4) / T(3)) * Constants<T>::pi * radius * radius * radius;
		T cylinder_vol = Constants<T>::pi * radius * radius * h;
		return sphere_vol + cylinder_vol;
	}

	MML_FORCE_INLINE T surface_area() const {
		T h = height();
		T sphere_area = T(4) * Constants<T>::pi * radius * radius;
		T cylinder_area = T(2) * Constants<T>::pi * radius * h;
		return sphere_area + cylinder_area;
	}

	MML_FORCE_INLINE Vector3<T> axis() const {
		return (b - a).normalized();
	}

	MML_FORCE_INLINE bool contains(const Vector3<T> &point) const {
		Vector3<T> ab = b - a;
		Vector3<T> ap = point - a;

		T t = ap.dot(ab) / ab.length_squared();
		t = t < T(0) ? T(0) : (t > T(1) ? T(1) : t);

		Vector3<T> closest = a + ab * t;
		return closest.distance_squared(point) <= radius * radius;
	}

	MML_FORCE_INLINE Vector3<T> closest_point(const Vector3<T> &point) const {
		Vector3<T> ab = b - a;
		Vector3<T> ap = point - a;

		T t = ap.dot(ab) / ab.length_squared();
		t = t < T(0) ? T(0) : (t > T(1) ? T(1) : t);

		Vector3<T> closest = a + ab * t;
		Vector3<T> dir = point - closest;
		T dist = dir.length();

		if (Epsilon<T>::approx_zero(dist)) {
			dir = Vector3<T>::unit_y();
			dist = radius;
		}

		return closest + dir.normalized() * radius;
	}

	MML_FORCE_INLINE T distance_squared(const Vector3<T> &point) const {
		Vector3<T> ab = b - a;
		Vector3<T> ap = point - a;

		T t = ap.dot(ab) / ab.length_squared();
		t = t < T(0) ? T(0) : (t > T(1) ? T(1) : t);

		Vector3<T> closest = a + ab * t;
		T dist_sq = closest.distance_squared(point);
		T dist = Math<T>::safe_sqrt(dist_sq);

		T dist_to_surface = dist - radius;
		return dist_to_surface < T(0) ? T(0) : dist_to_surface * dist_to_surface;
	}

	MML_FORCE_INLINE T distance(const Vector3<T> &point) const {
		return Math<T>::safe_sqrt(distance_squared(point));
	}

	MML_FORCE_INLINE bool intersects(const Sphere<T> &sphere) const {
		Vector3<T> ab = b - a;
		Vector3<T> ap = sphere.center - a;

		T t = ap.dot(ab) / ab.length_squared();
		t = t < T(0) ? T(0) : (t > T(1) ? T(1) : t);

		Vector3<T> closest = a + ab * t;
		T dist_sq = closest.distance_squared(sphere.center);
		T radius_sum = radius + sphere.radius;

		return dist_sq <= radius_sum * radius_sum;
	}

	MML_FORCE_INLINE bool intersects(const AABB<T> &aabb) const {
		T radius_sq = radius * radius;

		if (aabb.contains(a) || aabb.contains(b)) {
			return true;
		}

		Vector3<T> closest = aabb.closest_point(a);
		if ((a - closest).length_squared() <= radius_sq) {
			return true;
		}

		closest = aabb.closest_point(b);
		if ((b - closest).length_squared() <= radius_sq) {
			return true;
		}

		Vector3<T> ab = b - a;
		Vector3<T> test_points[] = {
			aabb.closest_point(a + ab * T(0.25)),
			aabb.closest_point(a + ab * T(0.5)),
			aabb.closest_point(a + ab * T(0.75))
		};

		for (int i = 0; i < 3; ++i) {
			if ((test_points[i] - (a + ab * ((i + 1) * T(0.25)))).length_squared() <= radius_sq) {
				return true;
			}
		}

		return false;
	}

	MML_FORCE_INLINE AABB<T> bounding_box() const {
		AABB<T> box(a, a);
		box.expand(b);
		Vector3<T> r(radius, radius, radius);
		return AABB<T>(box.min - r, box.max + r);
	}

	MML_FORCE_INLINE static Capsule from_center_height_radius(const Vector3<T> &center, T height, T radius_, const Vector3<T> &direction = Vector3<T>::up()) {
		Vector3<T> half_axis = direction.normalized() * (height * T(0.5));
		return Capsule(center - half_axis, center + half_axis, radius_);
	}
};

using Capsulef = Capsule<float>;
using Capsuled = Capsule<double>;

} //namespace mml