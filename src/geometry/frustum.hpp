/**************************************************************************/
/*  frustum.hpp                                                           */
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
#include "aabb.hpp"
#include "plane.hpp"
#include "sphere.hpp"
#include <type_traits>

namespace mml {

// View frustum for culling (defined by 6 planes)
template <typename T>
struct Frustum {
	static_assert(std::is_floating_point_v<T>, "Frustum requires floating-point type");

	// Frustum planes: left, right, bottom, top, near, far
	enum PlaneIndex {
		LEFT = 0,
		RIGHT = 1,
		BOTTOM = 2,
		TOP = 3,
		NEAR = 4,
		FAR = 5,
		PLANE_COUNT = 6
	};

	Plane<T> planes[PLANE_COUNT];

	// Default constructor
	Frustum() = default;

	// Explicit constructor from view-projection matrix
	static Frustum from_matrix(const Matrix4<T> &view_proj) {
		Frustum f;

		Vector3<T> left_normal(
				view_proj[0][3] + view_proj[0][0],
				view_proj[1][3] + view_proj[1][0],
				view_proj[2][3] + view_proj[2][0]);
		T left_dist = view_proj[3][3] + view_proj[3][0];
		T left_len = left_normal.length();
		f.planes[LEFT] = Plane<T>(left_normal / left_len, left_dist / left_len);

		Vector3<T> right_normal(
				view_proj[0][3] - view_proj[0][0],
				view_proj[1][3] - view_proj[1][0],
				view_proj[2][3] - view_proj[2][0]);
		T right_dist = view_proj[3][3] - view_proj[3][0];
		T right_len = right_normal.length();
		f.planes[RIGHT] = Plane<T>(right_normal / right_len, right_dist / right_len);

		Vector3<T> bottom_normal(
				view_proj[0][3] + view_proj[0][1],
				view_proj[1][3] + view_proj[1][1],
				view_proj[2][3] + view_proj[2][1]);
		T bottom_dist = view_proj[3][3] + view_proj[3][1];
		T bottom_len = bottom_normal.length();
		f.planes[BOTTOM] = Plane<T>(bottom_normal / bottom_len, bottom_dist / bottom_len);

		Vector3<T> top_normal(
				view_proj[0][3] - view_proj[0][1],
				view_proj[1][3] - view_proj[1][1],
				view_proj[2][3] - view_proj[2][1]);
		T top_dist = view_proj[3][3] - view_proj[3][1];
		T top_len = top_normal.length();
		f.planes[TOP] = Plane<T>(top_normal / top_len, top_dist / top_len);

		Vector3<T> near_normal(
				view_proj[0][3] + view_proj[0][2],
				view_proj[1][3] + view_proj[1][2],
				view_proj[2][3] + view_proj[2][2]);
		T near_dist = view_proj[3][3] + view_proj[3][2];
		T near_len = near_normal.length();
		f.planes[NEAR] = Plane<T>(near_normal / near_len, near_dist / near_len);

		Vector3<T> far_normal(
				view_proj[0][3] - view_proj[0][2],
				view_proj[1][3] - view_proj[1][2],
				view_proj[2][3] - view_proj[2][2]);
		T far_dist = view_proj[3][3] - view_proj[3][2];
		T far_len = far_normal.length();
		f.planes[FAR] = Plane<T>(far_normal / far_len, far_dist / far_len);

		return f;
	}

	// Explicit copy constructor
	Frustum(const Frustum &other) = default;
	Frustum &operator=(const Frustum &other) = default;

	// Check if point is inside frustum
	MML_FORCE_INLINE bool contains(const Vector3<T> &point) const {
		for (size_t i = 0; i < PLANE_COUNT; ++i) {
			if (planes[i].distance_to(point) < T(0)) {
				return false; // point is outside this plane
			}
		}
		return true;
	}

	// Check if sphere is inside or intersects frustum
	MML_FORCE_INLINE bool intersects(const Sphere<T> &sphere) const {
		for (size_t i = 0; i < PLANE_COUNT; ++i) {
			T distance = planes[i].distance_to(sphere.center);
			if (distance < -sphere.radius) {
				return false; // sphere is completely outside this plane
			}
		}
		return true; // sphere intersects or is inside frustum
	}

	// Check if AABB is inside or intersects frustum
	MML_FORCE_INLINE bool intersects(const AABB<T> &aabb) const {
		for (size_t i = 0; i < PLANE_COUNT; ++i) {
			Vector3<T> positive_vertex(
					(planes[i].normal.x >= T(0)) ? aabb.max.x : aabb.min.x,
					(planes[i].normal.y >= T(0)) ? aabb.max.y : aabb.min.y,
					(planes[i].normal.z >= T(0)) ? aabb.max.z : aabb.min.z);

			if (planes[i].distance_to(positive_vertex) < T(0)) {
				return false; // AABB is completely outside this plane
			}
		}
		return true; // AABB intersects or is inside frustum
	}

	// Check if AABB is fully inside frustum
	MML_FORCE_INLINE bool contains(const AABB<T> &aabb) const {
		for (size_t i = 0; i < PLANE_COUNT; ++i) {
			Vector3<T> negative_vertex(
					(planes[i].normal.x >= T(0)) ? aabb.min.x : aabb.max.x,
					(planes[i].normal.y >= T(0)) ? aabb.min.y : aabb.max.y,
					(planes[i].normal.z >= T(0)) ? aabb.min.z : aabb.max.z);

			if (planes[i].distance_to(negative_vertex) < T(0)) {
				return false; // AABB is not fully inside
			}
		}
		return true; // AABB is fully inside frustum
	}
};

// Type aliases
using Frustumf = Frustum<float>;
using Frustumd = Frustum<double>;

} // namespace mml