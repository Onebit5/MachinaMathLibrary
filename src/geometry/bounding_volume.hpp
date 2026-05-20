/**************************************************************************/
/*  bounding_volume.hpp                                                   */
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

#include "../algebra/matrix4.hpp"
#include "aabb.hpp"
#include "sphere.hpp"
#include <type_traits>

namespace mml {

template <typename T>
struct BoundingVolumeUtils {
	static constexpr bool is_floating_point_v = std::is_floating_point_v<T>;

	static inline AABB<T> union_aabb(const AABB<T> &a, const AABB<T> &b) {
		return AABB<T>::merge(a, b);
	}

	static inline Sphere<T> union_sphere(const Sphere<T> &a, const Sphere<T> &b) {
		Vector3<T> center = a.center + b.center;
		center = center * T(0.5);
		T distance = a.center.distance(b.center);
		T radius = distance * T(0.5) + std::max(a.radius, b.radius);
		return Sphere<T>(center, radius);
	}

	static inline AABB<T> union_aabb_sphere(const AABB<T> &aabb, const Sphere<T> &sphere) {
		AABB<T> sphere_aabb = sphere.bounding_box();
		return AABB<T>::merge(aabb, sphere_aabb);
	}

	static inline AABB<T> union_sphere_aabb(const Sphere<T> &sphere, const AABB<T> &aabb) {
		return union_aabb_sphere(aabb, sphere);
	}

	static inline Sphere<T> union_sphere_aabb_enclosing(const Sphere<T> &sphere, const AABB<T> &aabb) {
		Vector3<T> center = sphere.center;
		T radius = sphere.radius;
		Vector3<T> corners[8] = {
			aabb.min,
			Vector3<T>(aabb.max.x, aabb.min.y, aabb.min.z),
			Vector3<T>(aabb.min.x, aabb.max.y, aabb.min.z),
			Vector3<T>(aabb.max.x, aabb.max.y, aabb.min.z),
			Vector3<T>(aabb.min.x, aabb.min.y, aabb.max.z),
			Vector3<T>(aabb.max.x, aabb.min.y, aabb.max.z),
			Vector3<T>(aabb.min.x, aabb.max.y, aabb.max.z),
			aabb.max
		};
		for (const auto &corner : corners) {
			T dist = center.distance(corner);
			if (dist > radius) {
				radius = dist;
			}
		}
		return Sphere<T>(center, radius);
	}

	static inline bool intersects_aabb_aabb(const AABB<T> &a, const AABB<T> &b) {
		return a.intersects(b);
	}

	static inline bool intersects_sphere_sphere(const Sphere<T> &a, const Sphere<T> &b) {
		return a.intersects(b);
	}

	static inline bool intersects_aabb_sphere(const AABB<T> &aabb, const Sphere<T> &sphere) {
		return sphere.intersects(aabb);
	}

	static inline bool intersects_sphere_aabb(const Sphere<T> &sphere, const AABB<T> &aabb) {
		return sphere.intersects(aabb);
	}

	static inline bool intersects_sphere_aabb(const AABB<T> &aabb, const Sphere<T> &sphere) {
		return sphere.intersects(aabb);
	}

	static inline AABB<T> intersect_aabb_aabb(const AABB<T> &a, const AABB<T> &b) {
		Vector3<T> min(
				std::max(a.min.x, b.min.x),
				std::max(a.min.y, b.min.y),
				std::max(a.min.z, b.min.z));
		Vector3<T> max(
				std::min(a.max.x, b.max.x),
				std::min(a.max.y, b.max.y),
				std::min(a.max.z, b.max.z));

		if (min.x > max.x || min.y > max.y || min.z > max.z) {
			return AABB<T>::from_center_extents(Vector3<T>(), Vector3<T>());
		}

		return AABB<T>(min, max);
	}

	static inline AABB<T> transform_aabb(const AABB<T> &aabb, const Matrix4<T> &matrix) {
		Vector3<T> corners[8] = {
			Vector3<T>(aabb.min.x, aabb.min.y, aabb.min.z),
			Vector3<T>(aabb.max.x, aabb.min.y, aabb.min.z),
			Vector3<T>(aabb.min.x, aabb.max.y, aabb.min.z),
			Vector3<T>(aabb.max.x, aabb.max.y, aabb.min.z),
			Vector3<T>(aabb.min.x, aabb.min.y, aabb.max.z),
			Vector3<T>(aabb.max.x, aabb.min.y, aabb.max.z),
			Vector3<T>(aabb.min.x, aabb.max.y, aabb.max.z),
			Vector3<T>(aabb.max.x, aabb.max.y, aabb.max.z)
		};

		Vector3<T> transformed_corners[8];
		for (int i = 0; i < 8; ++i) {
			transformed_corners[i] = matrix.transform_point(corners[i]);
		}

		Vector3<T> new_min = transformed_corners[0];
		Vector3<T> new_max = transformed_corners[0];
		for (int i = 1; i < 8; ++i) {
			new_min.x = std::min(new_min.x, transformed_corners[i].x);
			new_min.y = std::min(new_min.y, transformed_corners[i].y);
			new_min.z = std::min(new_min.z, transformed_corners[i].z);
			new_max.x = std::max(new_max.x, transformed_corners[i].x);
			new_max.y = std::max(new_max.y, transformed_corners[i].y);
			new_max.z = std::max(new_max.z, transformed_corners[i].z);
		}

		return AABB<T>(new_min, new_max);
	}

	static inline Sphere<T> transform_sphere(const Sphere<T> &sphere, const Matrix4<T> &matrix) {
		Vector3<T> transformed_center = matrix.transform_point(sphere.center);

		Vector3<T> axis_x(T(1), T(0), T(0));
		Vector3<T> axis_y(T(0), T(1), T(0));
		Vector3<T> axis_z(T(0), T(0), T(1));

		Vector3<T> scaled_x = matrix.transform_direction(axis_x);
		Vector3<T> scaled_y = matrix.transform_direction(axis_y);
		Vector3<T> scaled_z = matrix.transform_direction(axis_z);

		T max_scale = std::max({ scaled_x.length(), scaled_y.length(), scaled_z.length() });
		T transformed_radius = sphere.radius * max_scale;

		return Sphere<T>(transformed_center, transformed_radius);
	}

	static inline AABB<T> transform_aabb_rotation_scale(const AABB<T> &aabb, const Matrix4<T> &matrix) {
		Vector3<T> corners[8] = {
			Vector3<T>(aabb.min.x, aabb.min.y, aabb.min.z),
			Vector3<T>(aabb.max.x, aabb.min.y, aabb.min.z),
			Vector3<T>(aabb.min.x, aabb.max.y, aabb.min.z),
			Vector3<T>(aabb.max.x, aabb.max.y, aabb.min.z),
			Vector3<T>(aabb.min.x, aabb.min.y, aabb.max.z),
			Vector3<T>(aabb.max.x, aabb.min.y, aabb.max.z),
			Vector3<T>(aabb.min.x, aabb.max.y, aabb.max.z),
			Vector3<T>(aabb.max.x, aabb.max.y, aabb.max.z)
		};

		AABB<T> result;
		for (const auto &corner : corners) {
			Vector3<T> transformed = matrix * corner;
			result.expand(transformed);
		}

		return result;
	}

	static inline T distance_aabb_aabb(const AABB<T> &a, const AABB<T> &b) {
		Vector3<T> d(
				std::max(T(0), std::max(a.min.x - b.max.x, b.min.x - a.max.x)),
				std::max(T(0), std::max(a.min.y - b.max.y, b.min.y - a.max.y)),
				std::max(T(0), std::max(a.min.z - b.max.z, b.min.z - a.max.z)));
		return d.length();
	}

	static inline T distance_sphere_sphere(const Sphere<T> &a, const Sphere<T> &b) {
		T dist = a.center.distance(b.center);
		return std::max(T(0), dist - a.radius - b.radius);
	}

	static inline T distance_aabb_sphere(const AABB<T> &aabb, const Sphere<T> &sphere) {
		Vector3<T> closest = aabb.closest_point(sphere.center);
		return std::max(T(0), closest.distance(sphere.center) - sphere.radius);
	}

	static inline T distance_sphere_aabb(const Sphere<T> &sphere, const AABB<T> &aabb) {
		return distance_aabb_sphere(aabb, sphere);
	}

	static inline bool contains_aabb_point(const AABB<T> &aabb, const Vector3<T> &point) {
		return aabb.contains(point);
	}

	static inline bool contains_sphere_point(const Sphere<T> &sphere, const Vector3<T> &point) {
		return sphere.contains(point);
	}

	static inline bool contains_aabb_aabb(const AABB<T> &a, const AABB<T> &b) {
		return a.contains(b);
	}

	static inline bool contains_sphere_sphere(const Sphere<T> &a, const Sphere<T> &b) {
		return a.contains(b);
	}

	static inline bool contains_aabb_sphere(const AABB<T> &aabb, const Sphere<T> &sphere) {
		AABB<T> sphere_aabb = sphere.bounding_box();
		return aabb.contains(sphere_aabb);
	}

	static inline T get_aabb_surface_area(const AABB<T> &aabb) {
		return aabb.surface_area();
	}

	static inline T get_sphere_surface_area(const Sphere<T> &sphere) {
		return sphere.surface_area();
	}

	static inline T get_aabb_volume(const AABB<T> &aabb) {
		return aabb.volume();
	}

	static inline T get_sphere_volume(const Sphere<T> &sphere) {
		return sphere.volume();
	}
};

template <typename T>
AABB<T> operator|(const AABB<T> &a, const AABB<T> &b) {
	return BoundingVolumeUtils<T>::union_aabb(a, b);
}

template <typename T>
Sphere<T> operator|(const Sphere<T> &a, const Sphere<T> &b) {
	return BoundingVolumeUtils<T>::union_sphere(a, b);
}

template <typename T>
AABB<T> transform(const AABB<T> &aabb, const Matrix4<T> &matrix) {
	return BoundingVolumeUtils<T>::transform_aabb(aabb, matrix);
}

template <typename T>
Sphere<T> transform(const Sphere<T> &sphere, const Matrix4<T> &matrix) {
	return BoundingVolumeUtils<T>::transform_sphere(sphere, matrix);
}

template <typename T>
struct BoundingFrustum {
	Vector3<T> planes[6];
	T normals[6][4];

	enum class Plane {
		Left = 0,
		Right = 1,
		Bottom = 2,
		Top = 3,
		Near = 4,
		Far = 5
	};

	BoundingFrustum() = default;

	void extract_from_matrix(const Matrix4<T> &matrix) {
		Vector4<T> rows[4] = {
			Vector4<T>(matrix[0][0], matrix[0][1], matrix[0][2], matrix[0][3]),
			Vector4<T>(matrix[1][0], matrix[1][1], matrix[1][2], matrix[1][3]),
			Vector4<T>(matrix[2][0], matrix[2][1], matrix[2][2], matrix[2][3]),
			Vector4<T>(matrix[3][0], matrix[3][1], matrix[3][2], matrix[3][3])
		};

		normals[0][0] = rows[3].x + rows[0].x;
		normals[0][1] = rows[3].y + rows[0].y;
		normals[0][2] = rows[3].z + rows[0].z;
		normals[0][3] = rows[3].w + rows[0].w;

		normals[1][0] = rows[3].x - rows[0].x;
		normals[1][1] = rows[3].y - rows[0].y;
		normals[1][2] = rows[3].z - rows[0].z;
		normals[1][3] = rows[3].w - rows[0].w;

		normals[2][0] = rows[3].x + rows[1].x;
		normals[2][1] = rows[3].y + rows[1].y;
		normals[2][2] = rows[3].z + rows[1].z;
		normals[2][3] = rows[3].w + rows[1].w;

		normals[3][0] = rows[3].x - rows[1].x;
		normals[3][1] = rows[3].y - rows[1].y;
		normals[3][2] = rows[3].z - rows[1].z;
		normals[3][3] = rows[3].w - rows[1].w;

		normals[4][0] = rows[3].x + rows[2].x;
		normals[4][1] = rows[3].y + rows[2].y;
		normals[4][2] = rows[3].z + rows[2].z;
		normals[4][3] = rows[3].w + rows[2].w;

		normals[5][0] = rows[3].x - rows[2].x;
		normals[5][1] = rows[3].y - rows[2].y;
		normals[5][2] = rows[3].z - rows[2].z;
		normals[5][3] = rows[3].w - rows[2].w;

		for (int i = 0; i < 6; ++i) {
			Vector3<T> normal(normals[i][0], normals[i][1], normals[i][2]);
			T length = normal.length();
			if (length > Epsilon<T>::value) {
				normals[i][0] /= length;
				normals[i][1] /= length;
				normals[i][2] /= length;
				planes[i] = normal;
			}
		}
	}

	bool contains(const Vector3<T> &point) const {
		for (int i = 0; i < 6; ++i) {
			Vector3<T> normal(normals[i][0], normals[i][1], normals[i][2]);
			Vector3<T> offset = point;
			if (normal.dot(offset) + normals[i][3] < T(0)) {
				return false;
			}
		}
		return true;
	}

	bool intersects(const AABB<T> &aabb) const {
		for (int i = 0; i < 6; ++i) {
			Vector3<T> normal(normals[i][0], normals[i][1], normals[i][2]);
			Vector3<T> pos_normal = Vector3<T>(
					normal.x > T(0) ? aabb.max.x : aabb.min.x,
					normal.y > T(0) ? aabb.max.y : aabb.min.y,
					normal.z > T(0) ? aabb.max.z : aabb.min.z);
			if (pos_normal.dot(normal) + normals[i][3] < T(0)) {
				return false;
			}
		}
		return true;
	}

	bool intersects(const Sphere<T> &sphere) const {
		for (int i = 0; i < 6; ++i) {
			Vector3<T> normal(normals[i][0], normals[i][1], normals[i][2]);
			T dist = sphere.center.dot(normal) + normals[i][3];
			if (dist < -sphere.radius) {
				return false;
			}
		}
		return true;
	}

	bool contains(const AABB<T> &aabb) const {
		return intersects(aabb);
	}

	bool contains(const Sphere<T> &sphere) const {
		return intersects(sphere);
	}
};

using BoundingFrustumf = BoundingFrustum<float>;
using BoundingFrustumd = BoundingFrustum<double>;

} //namespace mml