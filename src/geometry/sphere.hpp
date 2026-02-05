/**************************************************************************/
/*  sphere.hpp                                                            */
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
#include "../core/constants.hpp"
#include "../core/epsilon.hpp"
#include "../utils/math.hpp"
#include "aabb.hpp"
#include "ray.hpp"
#include <cmath>
#include <type_traits>

namespace mml {

// Sphere in 3D space
template <typename T>
struct Sphere {
	static_assert(std::is_floating_point_v<T>, "Sphere requires floating-point type");

	Vector3<T> center;
	T radius;

	// Default constructor
	Sphere() = default;

	// Explicit constructor
	explicit Sphere(const Vector3<T> &center_, T radius_) :
			center(center_), radius(radius_) {}

	// Explicit copy constructor
	Sphere(const Sphere &other) = default;
	Sphere &operator=(const Sphere &other) = default;

	// Get volume
	MML_FORCE_INLINE T volume() const {
		// V = (4/3) * π * r³
		T r3 = radius * radius * radius;
		return (T(4) / T(3)) *
				Constants<T>::pi *
				r3;
	}

	// Get surface area
	MML_FORCE_INLINE T surface_area() const {
		// A = 4 * π * r²
		T r2 = radius * radius;
		return T(4) * Constants<T>::pi * r2;
	}

	// Check if point is inside sphere
	MML_FORCE_INLINE bool contains(const Vector3<T> &point) const {
		return center.distance_squared(point) <= radius * radius;
	}

	// Check if another sphere is fully inside this sphere
	MML_FORCE_INLINE bool contains(const Sphere &other) const {
		T distance = center.distance(other.center);
		return distance + other.radius <= radius; // check if other sphere fits inside
	}

	// Check if this sphere intersects with another sphere
	MML_FORCE_INLINE bool intersects(const Sphere &other) const {
		T distance_sq = center.distance_squared(other.center);
		T radius_sum = radius + other.radius;
		return distance_sq <= radius_sum * radius_sum;
	}

	// Check if sphere intersects with AABB
	MML_FORCE_INLINE bool intersects(const AABB<T> &aabb) const {
		Vector3<T> closest = aabb.closest_point(center);
		return center.distance_squared(closest) <= radius * radius;
	}

	// Ray-sphere intersection
	// Returns true if intersection exists, stores t values in out_t1 and out_t2
	MML_FORCE_INLINE bool intersect_ray(const Ray<T> &ray, T &out_t1, T &out_t2) const {
		Vector3<T> oc = ray.origin - center; // vector from center to ray origin

		T a = ray.direction.dot(ray.direction); // quadratic coefficient
		T b = T(2) * oc.dot(ray.direction); // linear coefficient
		T c = oc.dot(oc) - radius * radius; // constant term

		T discriminant = b * b - T(4) * a * c; // discriminant

		if (discriminant < T(0)) {
			return false; // no real roots, no intersection
		}

		T sqrt_discriminant = Math<T>::safe_sqrt(discriminant);
		T inv_2a = T(1) / (T(2) * a); // precompute reciprocal

		out_t1 = (-b - sqrt_discriminant) * inv_2a; // near intersection
		out_t2 = (-b + sqrt_discriminant) * inv_2a; // far intersection

		return true;
	}

	// Ray-sphere intersection (returns closest hit point)
	MML_FORCE_INLINE bool intersect_ray(const Ray<T> &ray, Vector3<T> &out_point) const {
		T t1, t2;
		if (intersect_ray(ray, t1, t2)) {
			// Use the nearest positive intersection
			T t = (t1 >= T(0)) ? t1 : t2;
			if (t >= T(0)) {
				out_point = ray.at(t);
				return true;
			}
		}
		return false;
	}

	// Get closest point on sphere surface to given point
	MML_FORCE_INLINE Vector3<T> closest_point(const Vector3<T> &point) const {
		Vector3<T> dir = point - center; // direction from center to point
		T dist = dir.length();

		if (Epsilon<T>::approx_zero(dist)) {
			return center + Vector3<T>(radius, T(0), T(0)); // point at center, return arbitrary surface point
		}

		return center + dir * (radius / dist); // point on surface along direction
	}

	// Get normal at point on sphere surface
	MML_FORCE_INLINE Vector3<T> normal_at(const Vector3<T> &point) const {
		return (point - center).normalized();
	}

	// Get bounding AABB
	MML_FORCE_INLINE AABB<T> bounding_box() const {
		Vector3<T> r(radius, radius, radius);
		return AABB<T>(center - r, center + r);
	}

	// Transform sphere by a 4x4 matrix
	MML_FORCE_INLINE Sphere transformed(const Matrix4<T> &matrix) const {
		Vector3<T> transformed_center = matrix.transform_point(center);

		Vector3<T> axis_x(T(1), T(0), T(0));
		Vector3<T> axis_y(T(0), T(1), T(0));
		Vector3<T> axis_z(T(0), T(0), T(1));

		Vector3<T> scaled_x = matrix.transform_direction(axis_x);
		Vector3<T> scaled_y = matrix.transform_direction(axis_y);
		Vector3<T> scaled_z = matrix.transform_direction(axis_z);

		T max_scale = std::max({ scaled_x.length(), scaled_y.length(), scaled_z.length() });
		T transformed_radius = radius * max_scale;

		return Sphere(transformed_center, transformed_radius);
	}

	// Union with another sphere - creates sphere enclosing both
	MML_FORCE_INLINE Sphere operator|(const Sphere &other) const {
		if (contains(other)) {
			return *this;
		}
		if (other.contains(*this)) {
			return other;
		}

		Vector3<T> delta = other.center - center;
		T dist = delta.length();

		if (dist < Epsilon<T>::value) {
			return Sphere(center, std::max(radius, other.radius));
		}

		Vector3<T> dir = delta / dist;
		Vector3<T> p1 = center - dir * radius;
		Vector3<T> p2 = other.center + dir * other.radius;
		Vector3<T> new_center = (p1 + p2) * T(0.5);
		T new_radius = p2.distance(p1) * T(0.5);

		return Sphere(new_center, new_radius);
	}

	// Distance to a point
	MML_FORCE_INLINE T distance_to(const Vector3<T> &point) const {
		T dist = center.distance(point);
		return std::max(T(0), dist - radius);
	}

	// Distance to another sphere
	MML_FORCE_INLINE T distance_to(const Sphere &other) const {
		T dist = center.distance(other.center);
		return std::max(T(0), dist - radius - other.radius);
	}
};

// Type aliases
using Spheref = Sphere<float>;
using Sphered = Sphere<double>;

} // namespace mml