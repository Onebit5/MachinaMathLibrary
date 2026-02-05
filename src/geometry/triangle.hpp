/**************************************************************************/
/*  triangle.hpp                                                          */
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
#include "plane.hpp"
#include "ray.hpp"
#include <cmath>
#include <type_traits>

namespace mml {

template <typename T>
struct Triangle {
	static_assert(std::is_floating_point_v<T>, "Triangle requires floating-point type");

	Vector3<T> a;
	Vector3<T> b;
	Vector3<T> c;

	Triangle() = default;

	explicit Triangle(const Vector3<T> &a_, const Vector3<T> &b_, const Vector3<T> &c_) :
			a(a_), b(b_), c(c_) {}

	constexpr Triangle(const Triangle &other) = default;
	Triangle &operator=(const Triangle &other) = default;

	MML_FORCE_INLINE Vector3<T> center() const {
		return (a + b + c) * (T(1) / T(3));
	}

	MML_FORCE_INLINE T area() const {
		return (b - a).cross(c - a).length() * T(0.5);
	}

	MML_FORCE_INLINE Vector3<T> normal() const {
		return (b - a).cross(c - a).normalized();
	}

	MML_FORCE_INLINE Plane<T> plane() const {
		return Plane<T>::from_points(a, b, c);
	}

	MML_FORCE_INLINE bool contains(const Vector3<T> &point) const {
		Vector3<T> v0 = c - a; // edge from a to c
		Vector3<T> v1 = b - a; // edge from a to b
		Vector3<T> v2 = point - a; // vector from a to point

		T dot00 = v0.dot(v0); // |v0|²
		T dot01 = v0.dot(v1); // v0 · v1
		T dot02 = v0.dot(v2); // v0 · v2
		T dot11 = v1.dot(v1); // |v1|²
		T dot12 = v1.dot(v2); // v1 · v2

		T inv_denom = T(1) / (dot00 * dot11 - dot01 * dot01); // inverse denominator
		T u = (dot11 * dot02 - dot01 * dot12) * inv_denom; // barycentric coordinate u
		T v = (dot00 * dot12 - dot01 * dot02) * inv_denom; // barycentric coordinate v

		return (u >= T(0)) && (v >= T(0)) && (u + v <= T(1)); // check if point inside triangle
	}

	MML_FORCE_INLINE bool intersect_ray(const Ray<T> &ray, T &out_t) const {
		Vector3<T> edge1 = b - a; // triangle edge
		Vector3<T> edge2 = c - a; // triangle edge

		Vector3<T> h = ray.direction.cross(edge2); // cross product
		T a_f = edge1.dot(h); // determinant

		if (Epsilon<T>::approx_zero(a_f)) {
			return false; // ray is parallel to triangle
		}

		T f = T(1) / a_f; // reciprocal of determinant
		Vector3<T> s = ray.origin - a; // vector from vertex a to ray origin
		T u = f * s.dot(h); // barycentric coordinate u

		if (u < T(0) || u > T(1)) {
			return false; // intersection outside triangle
		}

		Vector3<T> q = s.cross(edge1); // second cross product
		T v = f * ray.direction.dot(q); // barycentric coordinate v

		if (v < T(0) || u + v > T(1)) {
			return false; // intersection outside triangle
		}

		T t = f * edge2.dot(q); // intersection parameter

		if (t > T(0)) {
			out_t = t;
			return true; // valid intersection
		}

		return false; // intersection behind ray origin
	}

	MML_FORCE_INLINE bool intersect_ray(const Ray<T> &ray, Vector3<T> &out_point) const {
		T t;
		if (intersect_ray(ray, t)) {
			out_point = ray.at(t);
			return true;
		}
		return false;
	}

	MML_FORCE_INLINE bool intersect_ray(const Ray<T> &ray, Vector3<T> &out_point, Vector3<T> &out_barycentric) const {
		T t;
		T u, v;
		if (intersect_ray(ray, t)) {
			out_point = ray.at(t);

			Vector3<T> v0 = c - a;
			Vector3<T> v1 = b - a;
			Vector3<T> v2 = out_point - a;

			T dot00 = v0.dot(v0);
			T dot01 = v0.dot(v1);
			T dot02 = v0.dot(v2);
			T dot11 = v1.dot(v1);
			T dot12 = v1.dot(v2);

			T inv_denom = T(1) / (dot00 * dot11 - dot01 * dot01);
			u = (dot11 * dot02 - dot01 * dot12) * inv_denom;
			v = (dot00 * dot12 - dot01 * dot02) * inv_denom;
			out_barycentric.x = T(1) - u - v;
			out_barycentric.y = u;
			out_barycentric.z = v;

			return true;
		}
		return false;
	}

	MML_FORCE_INLINE Vector3<T> closest_point(const Vector3<T> &point) const {
		Vector3<T> ab = b - a; // edge from a to b
		Vector3<T> ac = c - a; // edge from a to c
		Vector3<T> ap = point - a; // vector from a to point

		T d1 = ab.dot(ap); // projection onto ab
		T d2 = ac.dot(ap); // projection onto ac

		if (d1 <= T(0) && d2 <= T(0)) {
			return a; // closest to vertex a
		}

		Vector3<T> bp = point - b; // vector from b to point
		T d3 = ab.dot(bp);
		T d4 = ac.dot(bp);

		if (d3 >= T(0) && d4 <= d3) {
			return b; // closest to vertex b
		}

		Vector3<T> cp = point - c; // vector from c to point
		T d5 = ab.dot(cp);
		T d6 = ac.dot(cp);

		if (d6 >= T(0) && d5 <= d6) {
			return c; // closest to vertex c
		}

		T vc = d1 * d4 - d3 * d2; // voronoi region for edge bc
		if (vc <= T(0) && d1 >= T(0) && d3 <= T(0)) {
			T v = d1 / (d1 - d3); // projection onto ab
			return a + ab * v; // closest point on ab
		}

		T vb = d5 * d2 - d1 * d6; // voronoi region for edge ac
		if (vb <= T(0) && d2 >= T(0) && d6 <= T(0)) {
			T w = d2 / (d2 - d6); // projection onto ac
			return a + ac * w; // closest point on ac
		}

		T va = d3 * d6 - d5 * d4; // voronoi region for edge bc
		if (va <= T(0) && (d4 - d3) >= T(0) && (d5 - d6) >= T(0)) {
			T w = (d4 - d3) / ((d4 - d3) + (d5 - d6)); // projection onto bc
			return b + (c - b) * w; // closest point on bc
		}

		T denom = T(1) / (va + vb + vc); // total area factor
		T v = vb * denom; // barycentric coordinate v
		T w = vc * denom; // barycentric coordinate w
		return a + ab * v + ac * w; // closest point inside triangle
	}

	MML_FORCE_INLINE T distance_squared(const Vector3<T> &point) const {
		Vector3<T> closest = closest_point(point);
		return (point - closest).length_squared();
	}

	MML_FORCE_INLINE T distance(const Vector3<T> &point) const {
		return Math<T>::safe_sqrt(distance_squared(point));
	}

	MML_FORCE_INLINE T signed_distance(const Vector3<T> &point) const {
		Vector3<T> n = normal(); // triangle normal
		Plane<T> p = plane(); // triangle plane
		T d = p.distance_to(point); // distance to plane
		if (d < T(0)) {
			return -distance(point); // point behind plane
		}
		return distance(point); // point in front of plane
	}

	MML_FORCE_INLINE Triangle<T> flipped() const {
		return Triangle<T>(c, b, a);
	}

	MML_FORCE_INLINE void flip() {
		Vector3<T> temp = a;
		a = c;
		c = temp;
	}
};

using Trianglef = Triangle<float>;
using Triangled = Triangle<double>;

} // namespace mml