/**************************************************************************/
/*  plane.hpp                                                             */
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
#include "../core/epsilon.hpp"
#include "ray.hpp"
#include <cmath>
#include <type_traits>

namespace mml {

template <typename T>
struct Plane {
	static_assert(std::is_floating_point_v<T>, "Plane requires floating-point type");

	Vector3<T> normal;
	T distance;

	Plane() = default;

	explicit Plane(const Vector3<T> &normal_, T distance_) :
			normal(normal_.normalized()), distance(distance_) {}

	static Plane from_point_normal(const Vector3<T> &point, const Vector3<T> &normal_) {
		Vector3<T> n = normal_.normalized();
		return Plane(n, n.dot(point));
	}

	static Plane from_points(const Vector3<T> &a, const Vector3<T> &b, const Vector3<T> &c) {
		Vector3<T> ab = b - a;
		Vector3<T> ac = c - a;
		Vector3<T> n = ab.cross(ac).normalized();
		return Plane(n, n.dot(a));
	}

	Plane(const Plane &other) = default;
	Plane &operator=(const Plane &other) = default;

	constexpr MML_FORCE_INLINE T distance_to(const Vector3<T> &point) const {
		return normal.dot(point) - distance;
	}

	constexpr MML_FORCE_INLINE Vector3<T> project(const Vector3<T> &point) const {
		return point - normal * distance_to(point);
	}

	MML_FORCE_INLINE bool contains(const Vector3<T> &point) const {
		return Epsilon<T>::approx_zero(distance_to(point));
	}

	MML_FORCE_INLINE bool intersect_ray(const Ray<T> &ray, T &out_t) const {
		T denom = normal.dot(ray.direction);

		if (Epsilon<T>::approx_zero(denom)) {
			return false;
		}

		out_t = (distance - normal.dot(ray.origin)) / denom;
		return true;
	}

	MML_FORCE_INLINE bool intersect_ray(const Ray<T> &ray, Vector3<T> &out_point) const {
		T t;
		if (intersect_ray(ray, t)) {
			out_point = ray.at(t);
			return true;
		}
		return false;
	}

	constexpr MML_FORCE_INLINE Plane flip() const {
		return Plane(-normal, -distance);
	}
};

using Planef = Plane<float>;
using Planed = Plane<double>;

} //namespace mml