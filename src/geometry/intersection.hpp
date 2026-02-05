/**************************************************************************/
/*  intersection.hpp                                                      */
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
#include "../core/epsilon.hpp"
#include "aabb.hpp"
#include "capsule.hpp"
#include "ray.hpp"
#include "sphere.hpp"
#include "triangle.hpp"
#include <cmath>

namespace mml {

namespace intersection {

// Ray-Sphere intersection
// Returns true if hit, outputs distance t and hit point
template <typename T>
inline bool ray_sphere(const Ray<T> &ray, const Vector3<T> &center, T radius, T &out_t, Vector3<T> &out_point) {
	Vector3<T> oc = ray.origin - center;
	T a = ray.direction.dot(ray.direction);
	T b = T(2) * oc.dot(ray.direction);
	T c = oc.dot(oc) - radius * radius;
	T discriminant = b * b - T(4) * a * c;

	if (discriminant < T(0)) {
		return false;
	}

	T sqrt_disc = std::sqrt(discriminant);
	T t = (-b - sqrt_disc) / (T(2) * a);

	if (t < T(0)) {
		t = (-b + sqrt_disc) / (T(2) * a);
		if (t < T(0)) {
			return false;
		}
	}

	out_t = t;
	out_point = ray.at(t);
	return true;
}

// Ray-AABB intersection (slab method)
template <typename T>
inline bool ray_aabb(const Ray<T> &ray, const AABB<T> &aabb, T &out_t, Vector3<T> &out_point) {
	T t_min = T(0);
	T t_max = std::numeric_limits<T>::infinity();

	for (int i = 0; i < 3; ++i) {
		if (std::abs(ray.direction[i]) < Epsilon<T>::value) {
			if (ray.origin[i] < aabb.min[i] || ray.origin[i] > aabb.max[i]) {
				return false;
			}
		} else {
			T inv_d = T(1) / ray.direction[i];
			T t1 = (aabb.min[i] - ray.origin[i]) * inv_d;
			T t2 = (aabb.max[i] - ray.origin[i]) * inv_d;

			if (t1 > t2) {
				std::swap(t1, t2);
			}
			t_min = std::max(t_min, t1);
			t_max = std::min(t_max, t2);

			if (t_min > t_max) {
				return false;
			}
		}
	}

	out_t = t_min;
	out_point = ray.at(t_min);
	return true;
}

// Ray-Triangle intersection (Möller–Trumbore)
template <typename T>
inline bool ray_triangle(const Ray<T> &ray, const Triangle<T> &tri, T &out_t, Vector3<T> &out_point, Vector3<T> &out_barycentric) {
	Vector3<T> edge1 = tri.b - tri.a;
	Vector3<T> edge2 = tri.c - tri.a;
	Vector3<T> h = ray.direction.cross(edge2);
	T a = edge1.dot(h);

	if (std::abs(a) < Epsilon<T>::value) {
		return false;
	}

	T f = T(1) / a;
	Vector3<T> s = ray.origin - tri.a;
	T u = f * s.dot(h);

	if (u < T(0) || u > T(1)) {
		return false;
	}

	Vector3<T> q = s.cross(edge1);
	T v = f * ray.direction.dot(q);

	if (v < T(0) || u + v > T(1)) {
		return false;
	}

	T t = f * edge2.dot(q);

	if (t > Epsilon<T>::value) {
		out_t = t;
		out_point = ray.at(t);
		out_barycentric = Vector3<T>(T(1) - u - v, u, v);
		return true;
	}

	return false;
}

// Ray-Plane intersection
template <typename T>
inline bool ray_plane(const Ray<T> &ray, const Vector3<T> &plane_normal, T plane_d, T &out_t, Vector3<T> &out_point) {
	T denom = plane_normal.dot(ray.direction);

	if (std::abs(denom) < Epsilon<T>::value) {
		return false;
	}

	T t = -(plane_normal.dot(ray.origin) + plane_d) / denom;

	if (t >= T(0)) {
		out_t = t;
		out_point = ray.at(t);
		return true;
	}

	return false;
}

// Ray-Capsule intersection
template <typename T>
inline bool ray_capsule(const Ray<T> &ray, const Capsule<T> &capsule, T &out_t, Vector3<T> &out_point) {
	// Treat as infinite cylinder first
	Vector3<T> ca = capsule.b - capsule.a;
	Vector3<T> oc = ray.origin - capsule.a;

	T caca = ca.dot(ca);
	T card = ca.dot(ray.direction);
	T caoc = ca.dot(oc);

	T a = caca - card * card;
	T b = caca * oc.dot(ray.direction) - caoc * card;
	T c = caca * oc.dot(oc) - caoc * caoc - capsule.radius * capsule.radius * caca;

	T discriminant = b * b - a * c;

	if (discriminant < T(0)) {
		// Check spheres at endpoints
		bool hit_a = ray_sphere(ray, capsule.a, capsule.radius, out_t, out_point);
		bool hit_b = ray_sphere(ray, capsule.b, capsule.radius, out_t, out_point);
		return hit_a || hit_b;
	}

	T t = (-b - std::sqrt(discriminant)) / a;

	// Check if intersection is within capsule segment
	T y = caoc + t * card;

	if (y >= T(0) && y <= caca) {
		if (t >= T(0)) {
			out_t = t;
			out_point = ray.at(t);
			return true;
		}
	}

	// Check spheres at endpoints
	bool hit_a = ray_sphere(ray, capsule.a, capsule.radius, out_t, out_point);
	bool hit_b = ray_sphere(ray, capsule.b, capsule.radius, out_t, out_point);
	return hit_a || hit_b;
}

// Sphere-Sphere intersection
template <typename T>
inline bool sphere_sphere(const Vector3<T> &c1, T r1, const Vector3<T> &c2, T r2) {
	T dist_sq = c1.distance_squared(c2);
	T rad_sum = r1 + r2;
	return dist_sq <= rad_sum * rad_sum;
}

// Sphere-AABB intersection
template <typename T>
inline bool sphere_aabb(const Vector3<T> &center, T radius, const AABB<T> &aabb) {
	Vector3<T> closest = aabb.closest_point(center);
	return center.distance_squared(closest) <= radius * radius;
}

// AABB-AABB intersection
template <typename T>
inline bool aabb_aabb(const AABB<T> &a, const AABB<T> &b) {
	return a.intersects(b);
}

// Sphere-Triangle intersection
template <typename T>
inline bool sphere_triangle(const Vector3<T> &center, T radius, const Triangle<T> &tri, Vector3<T> &out_closest_point) {
	// Find closest point on triangle to sphere center
	Vector3<T> ab = tri.b - tri.a;
	Vector3<T> ac = tri.c - tri.a;
	Vector3<T> bc = tri.c - tri.b;

	// Project onto triangle plane
	Vector3<T> normal = ab.cross(ac).normalized();
	T dist_to_plane = (center - tri.a).dot(normal);
	Vector3<T> plane_point = center - normal * dist_to_plane;

	// Check if point is inside triangle using barycentric coordinates
	Vector3<T> ap = plane_point - tri.a;
	Vector3<T> bp = plane_point - tri.b;
	Vector3<T> cp = plane_point - tri.c;

	Vector3<T> n_ab = ab.cross(normal);
	Vector3<T> n_ac = ac.cross(normal);
	Vector3<T> n_bc = bc.cross(normal);

	bool inside = (n_ab.dot(ap) >= T(0)) && (n_ac.dot(ap) <= T(0)) && (n_bc.dot(bp) >= T(0));

	if (inside) {
		out_closest_point = plane_point;
	} else {
		// Find closest point on edges
		out_closest_point = tri.a;
		T min_dist = center.distance_squared(tri.a);

		// Edge a-b
		Vector3<T> closest_ab = tri.a + ab * std::max(T(0), std::min(T(1), (center - tri.a).dot(ab) / ab.length_squared()));
		T dist_ab = center.distance_squared(closest_ab);
		if (dist_ab < min_dist) {
			min_dist = dist_ab;
			out_closest_point = closest_ab;
		}

		// Edge b-c
		Vector3<T> closest_bc = tri.b + bc * std::max(T(0), std::min(T(1), (center - tri.b).dot(bc) / bc.length_squared()));
		T dist_bc = center.distance_squared(closest_bc);
		if (dist_bc < min_dist) {
			min_dist = dist_bc;
			out_closest_point = closest_bc;
		}

		// Edge c-a
		Vector3<T> ca = tri.a - tri.c;
		Vector3<T> closest_ca = tri.c + ca * std::max(T(0), std::min(T(1), (center - tri.c).dot(ca) / ca.length_squared()));
		T dist_ca = center.distance_squared(closest_ca);
		if (dist_ca < min_dist) {
			out_closest_point = closest_ca;
		}
	}

	return center.distance_squared(out_closest_point) <= radius * radius;
}

// Capsule-AABB intersection
template <typename T>
inline bool capsule_aabb(const Capsule<T> &capsule, const AABB<T> &aabb) {
	// Check if capsule line segment intersects AABB
	Vector3<T> closest_aabb = aabb.closest_point(capsule.a);
	if (capsule.a.distance_squared(closest_aabb) <= capsule.radius * capsule.radius) {
		return true;
	}

	closest_aabb = aabb.closest_point(capsule.b);
	if (capsule.b.distance_squared(closest_aabb) <= capsule.radius * capsule.radius) {
		return true;
	}

	// Check AABB edges against capsule
	Vector3<T> ab = capsule.b - capsule.a;
	T ab_len_sq = ab.length_squared();

	// Sample points along capsule and check against AABB
	const int samples = 8;
	for (int i = 0; i <= samples; ++i) {
		T t = T(i) / T(samples);
		Vector3<T> point = capsule.a + ab * t;
		Vector3<T> closest = aabb.closest_point(point);
		if (point.distance_squared(closest) <= capsule.radius * capsule.radius) {
			return true;
		}
	}

	return false;
}

// Capsule-Sphere intersection
template <typename T>
inline bool capsule_sphere(const Capsule<T> &capsule, const Vector3<T> &center, T radius) {
	// Find closest point on capsule segment to sphere center
	Vector3<T> ab = capsule.b - capsule.a;
	T t = std::max(T(0), std::min(T(1), (center - capsule.a).dot(ab) / ab.length_squared()));
	Vector3<T> closest = capsule.a + ab * t;

	T total_radius = capsule.radius + radius;
	return center.distance_squared(closest) <= total_radius * total_radius;
}

// Point in AABB
template <typename T>
inline bool point_aabb(const Vector3<T> &point, const AABB<T> &aabb) {
	return aabb.contains(point);
}

// Point in Sphere
template <typename T>
inline bool point_sphere(const Vector3<T> &point, const Vector3<T> &center, T radius) {
	return point.distance_squared(center) <= radius * radius;
}

// Point in Triangle (barycentric)
template <typename T>
inline bool point_triangle(const Vector3<T> &point, const Triangle<T> &tri) {
	Vector3<T> ab = tri.b - tri.a;
	Vector3<T> ac = tri.c - tri.a;
	Vector3<T> ap = point - tri.a;

	T d00 = ab.dot(ab);
	T d01 = ab.dot(ac);
	T d11 = ac.dot(ac);
	T d20 = ap.dot(ab);
	T d21 = ap.dot(ac);

	T denom = d00 * d11 - d01 * d01;
	if (std::abs(denom) < Epsilon<T>::value) {
		return false;
	}

	T v = (d11 * d20 - d01 * d21) / denom;
	T w = (d00 * d21 - d01 * d20) / denom;
	T u = T(1) - v - w;

	return (u >= T(0)) && (v >= T(0)) && (w >= T(0));
}

// Line segment intersection tests
template <typename T>
struct LineSegment {
	Vector3<T> start;
	Vector3<T> end;

	LineSegment() = default;
	LineSegment(const Vector3<T> &s, const Vector3<T> &e) :
			start(s), end(e) {}

	Vector3<T> closest_point(const Vector3<T> &p) const {
		Vector3<T> ab = end - start;
		T t = std::max(T(0), std::min(T(1), (p - start).dot(ab) / ab.length_squared()));
		return start + ab * t;
	}
};

// Line segment - Sphere intersection
template <typename T>
inline bool segment_sphere(const LineSegment<T> &seg, const Vector3<T> &center, T radius) {
	Vector3<T> closest = seg.closest_point(center);
	return center.distance_squared(closest) <= radius * radius;
}

// Line segment - AABB intersection
template <typename T>
inline bool segment_aabb(const LineSegment<T> &seg, const AABB<T> &aabb) {
	// Check if either endpoint is inside
	if (aabb.contains(seg.start) || aabb.contains(seg.end)) {
		return true;
	}

	// Check ray intersection
	Vector3<T> dir = seg.end - seg.start;
	T len = dir.length();
	if (len < Epsilon<T>::value) {
		return false;
	}

	Ray<T> ray(seg.start, dir / len);
	T t;
	Vector3<T> point;

	if (!ray_aabb(ray, aabb, t, point)) {
		return false;
	}

	return t <= len;
}

// Swept sphere - AABB (for continuous collision detection)
template <typename T>
inline bool swept_sphere_aabb(const Vector3<T> &start, const Vector3<T> &end, T radius, const AABB<T> &aabb, T &out_t) {
	// Expand AABB by radius
	AABB<T> expanded(aabb.min - Vector3<T>(radius, radius, radius),
			aabb.max + Vector3<T>(radius, radius, radius));

	Vector3<T> dir = end - start;
	T len = dir.length();
	if (len < Epsilon<T>::value) {
		out_t = T(0);
		return expanded.contains(start);
	}

	Ray<T> ray(start, dir / len);
	T t;
	Vector3<T> point;

	if (!ray_aabb(ray, expanded, t, point)) {
		return false;
	}

	out_t = t / len;
	return out_t <= T(1);
}

// Distance queries
template <typename T>
inline T distance_point_triangle(const Vector3<T> &point, const Triangle<T> &tri) {
	Vector3<T> closest;
	Vector3<T> dummy;
	sphere_triangle(point, T(0), tri, closest);
	return point.distance(closest);
}

template <typename T>
inline T distance_segment_segment(const LineSegment<T> &s1, const LineSegment<T> &s2) {
	Vector3<T> u = s1.end - s1.start;
	Vector3<T> v = s2.end - s2.start;
	Vector3<T> w = s1.start - s2.start;

	T a = u.dot(u);
	T b = u.dot(v);
	T c = v.dot(v);
	T d = u.dot(w);
	T e = v.dot(w);

	T D = a * c - b * b;
	T sc, sN, sD = D;
	T tc, tN, tD = D;

	const T SMALL_NUM = Epsilon<T>::value;

	if (D < SMALL_NUM) {
		sN = T(0);
		sD = T(1);
		tN = e;
		tD = c;
	} else {
		sN = (b * e - c * d);
		tN = (a * e - b * d);
		if (sN < T(0)) {
			sN = T(0);
			tN = e;
			tD = c;
		} else if (sN > sD) {
			sN = sD;
			tN = e + b;
			tD = c;
		}
	}

	if (tN < T(0)) {
		tN = T(0);
		if (-d < T(0)) {
			sN = T(0);
		} else if (-d > a) {
			sN = sD;
		} else {
			sN = -d;
			sD = a;
		}
	} else if (tN > tD) {
		tN = tD;
		if ((-d + b) < T(0)) {
			sN = T(0);
		} else if ((-d + b) > a) {
			sN = sD;
		} else {
			sN = (-d + b);
			sD = a;
		}
	}

	sc = (std::abs(sN) < SMALL_NUM) ? T(0) : sN / sD;
	tc = (std::abs(tN) < SMALL_NUM) ? T(0) : tN / tD;

	Vector3<T> dP = w + (u * sc) - (v * tc);
	return dP.length();
}

// Type aliases
using LineSegmentf = LineSegment<float>;
using LineSegmentd = LineSegment<double>;

} // namespace intersection

} // namespace mml