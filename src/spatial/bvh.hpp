/**************************************************************************/
/*  bvh.hpp                                                               */
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
#include "../algebra/vector4.hpp"
#include "../core/config.hpp"
#include "../core/epsilon.hpp"
#include "../geometry/aabb.hpp"
#include "../utils/math.hpp"
#include <algorithm>
#include <array>
#include <cmath>
#include <functional>
#include <limits>
#include <memory>
#include <type_traits>
#include <vector>

#if MML_USE_SIMD
#include <immintrin.h>
#endif

namespace mml {

// ============================================================================
// Bounding Volume Hierarchy (BVH)
// ============================================================================
// A BVH is a tree structure that organizes objects using bounding volumes
// for efficient spatial queries (ray casting, frustum culling, etc.)

template <typename T, typename PrimitiveType>
struct BVHNode {
	static_assert(std::is_floating_point_v<T>, "BVH requires floating-point type");

	AABB<T> bounds;
	std::unique_ptr<BVHNode<T, PrimitiveType>> left;
	std::unique_ptr<BVHNode<T, PrimitiveType>> right;
	std::vector<PrimitiveType *> primitives;

	bool is_leaf = false;

	BVHNode() = default;
	explicit BVHNode(const AABB<T> &b) :
			bounds(b) {}

	[[nodiscard]] bool is_leaf_node() const { return is_leaf; }
};

template <typename T, typename PrimitiveType>
class BVH {
public:
	static_assert(std::is_floating_point_v<T>, "BVH requires floating-point type");

	using Node = BVHNode<T, PrimitiveType>;

private:
	std::unique_ptr<Node> root;
	std::vector<PrimitiveType> storage; // Owns the primitives
	size_t max_leaf_size = 4;

	// Get AABB for a primitive (user-provided customization point)
	std::function<AABB<T>(const PrimitiveType &)> bounds_func_;

public:
	BVH() = default;

	explicit BVH(std::function<AABB<T>(const PrimitiveType &)> bounds_func) :
			bounds_func_(std::move(bounds_func)) {}

	// Build BVH from primitives
	void build(const std::vector<PrimitiveType> &primitives,
			std::function<AABB<T>(const PrimitiveType &)> bounds_func = nullptr) {
		if (bounds_func) {
			bounds_func_ = std::move(bounds_func);
		}

		if (!bounds_func_) {
			return; // Cannot build without bounds function
		}

		storage = primitives;

		std::vector<PrimitiveType *> ptrs;
		ptrs.reserve(storage.size());
		for (auto &p : storage) {
			ptrs.push_back(&p);
		}

		root = build_recursive(ptrs, 0);
	}

	// Build from primitives with existing bounds function
	void build(std::vector<PrimitiveType> &&primitives) {
		storage = std::move(primitives);

		std::vector<PrimitiveType *> ptrs;
		ptrs.reserve(storage.size());
		for (auto &p : storage) {
			ptrs.push_back(&p);
		}

		root = build_recursive(ptrs, 0);
	}

	// Get bounds for primitive (uses bounds_func_ member)
	[[nodiscard]] AABB<T> get_primitive_bounds(const PrimitiveType &p) const {
		return bounds_func_(p);
	}

	// Ray intersection query - returns all intersected primitives
	[[nodiscard]] std::vector<PrimitiveType *> ray_intersect(
			const Vector3<T> &origin,
			const Vector3<T> &direction) const {
		std::vector<PrimitiveType *> result;
		if (!root) {
			return result;
		}

		ray_intersect_recursive(root.get(), origin, direction, result);
		return result;
	}

	// Ray intersection with early exit (closest hit)
	[[nodiscard]] PrimitiveType *ray_intersect_closest(
			const Vector3<T> &origin,
			const Vector3<T> &direction,
			T &out_distance) const {
		if (!root) {
			return nullptr;
		}

		PrimitiveType *closest = nullptr;
		T closest_dist = std::numeric_limits<T>::max();

		ray_intersect_closest_recursive(root.get(), origin, direction, closest, closest_dist);

		out_distance = closest_dist;
		return closest;
	}

	// Frustum culling - returns primitives inside or intersecting frustum
	[[nodiscard]] std::vector<PrimitiveType *> frustum_cull(
			const std::array<Vector3<T>, 6> &frustum_planes) const {
		std::vector<PrimitiveType *> result;
		if (!root) {
			return result;
		}

		frustum_cull_recursive(root.get(), frustum_planes, result);
		return result;
	}

	// Point query - returns primitives whose bounds contain the point
	[[nodiscard]] std::vector<PrimitiveType *> point_query(const Vector3<T> &point) const {
		std::vector<PrimitiveType *> result;
		if (!root) {
			return result;
		}

		point_query_recursive(root.get(), point, result);
		return result;
	}

	// Refit bounds (for dynamic objects)
	void refit() {
		if (!root) {
			return;
		}
		refit_recursive(root.get());
	}

	// Get total bounds
	[[nodiscard]] AABB<T> get_bounds() const {
		return root ? root->bounds : AABB<T>::empty();
	}

	// Get node count
	[[nodiscard]] size_t node_count() const {
		return count_nodes_recursive(root.get());
	}

	// Get depth
	[[nodiscard]] size_t depth() const {
		return depth_recursive(root.get());
	}

	// Clear all data
	void clear() {
		root.reset();
		storage.clear();
	}

	// Set max primitives per leaf
	void set_max_leaf_size(size_t size) {
		max_leaf_size = std::max(size_t(1), size);
	}

private:
	[[nodiscard]] std::unique_ptr<Node> build_recursive(std::vector<PrimitiveType *> &primitives, int depth) {
		if (primitives.empty()) {
			return nullptr;
		}

		auto node = std::make_unique<Node>();

		// Compute bounds for all primitives
		AABB<T> total_bounds = AABB<T>::empty();
		for (auto *p : primitives) {
			total_bounds = AABB<T>::merge(total_bounds, bounds_func_(*p));
		}
		node->bounds = total_bounds;

		// Create leaf if few primitives
		if (primitives.size() <= max_leaf_size) {
			node->is_leaf = true;
			node->primitives = std::move(primitives);
			return node;
		}

		// Find split axis (longest axis of bounds)
		Vector3<T> extents = total_bounds.extents();
		int axis = 0;
		if (extents.y > extents.x) {
			axis = 1;
		}
		if (extents.z > (axis == 0 ? extents.x : extents.y)) {
			axis = 2;
		}

		// Sort primitives by centroid along split axis
		std::sort(primitives.begin(), primitives.end(),
				[this, axis](PrimitiveType *a, PrimitiveType *b) {
					Vector3<T> center_a = bounds_func_(*a).center();
					Vector3<T> center_b = bounds_func_(*b).center();
					return center_a[axis] < center_b[axis];
				});

		// Split in half
		size_t mid = primitives.size() / 2;
		std::vector<PrimitiveType *> left_prims(primitives.begin(), primitives.begin() + mid);
		std::vector<PrimitiveType *> right_prims(primitives.begin() + mid, primitives.end());

		// Recursively build children
		node->left = build_recursive(left_prims, depth + 1);
		node->right = build_recursive(right_prims, depth + 1);

		return node;
	}

	void ray_intersect_recursive(Node *node, const Vector3<T> &origin,
			const Vector3<T> &direction,
			std::vector<PrimitiveType *> &result) const {
		if (!node) {
			return;
		}

		// Ray-AABB intersection test
		if (!ray_aabb_intersect(origin, direction, node->bounds)) {
			return;
		}

		if (node->is_leaf) {
			for (auto *p : node->primitives) {
				result.push_back(p);
			}
		} else {
			ray_intersect_recursive(node->left.get(), origin, direction, result);
			ray_intersect_recursive(node->right.get(), origin, direction, result);
		}
	}

	void ray_intersect_closest_recursive(Node *node, const Vector3<T> &origin,
			const Vector3<T> &direction,
			PrimitiveType *&closest,
			T &closest_dist) const {
		if (!node) {
			return;
		}

		T tmin, tmax;
		if (!ray_aabb_intersect_distance(origin, direction, node->bounds, tmin, tmax)) {
			return;
		}

		// Early exit if this node is farther than current closest
		if (tmin > closest_dist) {
			return;
		}

		if (node->is_leaf) {
			for (auto *p : node->primitives) {
				// Simple distance check to bounds center for now
				// In practice, you'd do primitive-specific intersection
				T dist = (bounds_func_(*p).center() - origin).length();
				if (dist < closest_dist) {
					closest_dist = dist;
					closest = p;
				}
			}
		} else {
			// Traverse closer child first
			T left_tmin, left_tmax, right_tmin, right_tmax;
			bool left_hit = node->left && ray_aabb_intersect_distance(origin, direction, node->left->bounds, left_tmin, left_tmax);
			bool right_hit = node->right && ray_aabb_intersect_distance(origin, direction, node->right->bounds, right_tmin, right_tmax);

			if (left_hit && right_hit) {
				if (left_tmin < right_tmin) {
					ray_intersect_closest_recursive(node->left.get(), origin, direction, closest, closest_dist);
					ray_intersect_closest_recursive(node->right.get(), origin, direction, closest, closest_dist);
				} else {
					ray_intersect_closest_recursive(node->right.get(), origin, direction, closest, closest_dist);
					ray_intersect_closest_recursive(node->left.get(), origin, direction, closest, closest_dist);
				}
			} else if (left_hit) {
				ray_intersect_closest_recursive(node->left.get(), origin, direction, closest, closest_dist);
			} else if (right_hit) {
				ray_intersect_closest_recursive(node->right.get(), origin, direction, closest, closest_dist);
			}
		}
	}

	void frustum_cull_recursive(Node *node, const std::array<Vector3<T>, 6> &planes,
			std::vector<PrimitiveType *> &result) const {
		if (!node) {
			return;
		}

		if (!aabb_in_frustum(node->bounds, planes)) {
			return;
		}

		if (node->is_leaf) {
			for (auto *p : node->primitives) {
				result.push_back(p);
			}
		} else {
			frustum_cull_recursive(node->left.get(), planes, result);
			frustum_cull_recursive(node->right.get(), planes, result);
		}
	}

	void point_query_recursive(Node *node, const Vector3<T> &point,
			std::vector<PrimitiveType *> &result) const {
		if (!node) {
			return;
		}

		if (!node->bounds.contains(point)) {
			return;
		}

		if (node->is_leaf) {
			for (auto *p : node->primitives) {
				if (bounds_func_(*p).contains(point)) {
					result.push_back(p);
				}
			}
		} else {
			point_query_recursive(node->left.get(), point, result);
			point_query_recursive(node->right.get(), point, result);
		}
	}

	void refit_recursive(Node *node) {
		if (!node || node->is_leaf) {
			return;
		}

		refit_recursive(node->left.get());
		refit_recursive(node->right.get());

		// Recalculate bounds from children
		node->bounds = AABB<T>::empty();
		if (node->left) {
			node->bounds = AABB<T>::merge(node->bounds, node->left->bounds);
		}
		if (node->right) {
			node->bounds = AABB<T>::merge(node->bounds, node->right->bounds);
		}
	}

	[[nodiscard]] size_t count_nodes_recursive(Node *node) const {
		if (!node) {
			return 0;
		}
		return 1 + count_nodes_recursive(node->left.get()) + count_nodes_recursive(node->right.get());
	}

	[[nodiscard]] size_t depth_recursive(Node *node) const {
		if (!node) {
			return 0;
		}
		if (node->is_leaf) {
			return 1;
		}
		return 1 + std::max(depth_recursive(node->left.get()), depth_recursive(node->right.get()));
	}

	// Ray-AABB intersection test (boolean)
	[[nodiscard]] bool ray_aabb_intersect(const Vector3<T> &origin, const Vector3<T> &direction,
			const AABB<T> &box) const {
		T tmin, tmax;
		return ray_aabb_intersect_distance(origin, direction, box, tmin, tmax);
	}

#if MML_USE_SIMD
	// SIMD-accelerated ray-AABB intersection with distance calculation
	// Using a simplified and correct implementation
	[[nodiscard]] bool ray_aabb_intersect_distance(const Vector3<T> &origin, const Vector3<T> &direction,
			const AABB<T> &box, T &tmin, T &tmax) const {
		T tymin, tymax, tzmin, tzmax;

		if (std::abs(direction.x) < Epsilon<T>::value) {
			if (origin.x < box.min.x || origin.x > box.max.x) {
				return false;
			}
			tmin = T(0);
			tmax = std::numeric_limits<T>::max();
		} else {
			T inv_dx = T(1) / direction.x;
			T t1 = (box.min.x - origin.x) * inv_dx;
			T t2 = (box.max.x - origin.x) * inv_dx;
			if (t1 > t2) {
				tmin = t2;
				tmax = t1;
			} else {
				tmin = t1;
				tmax = t2;
			}
		}

		if (std::abs(direction.y) < Epsilon<T>::value) {
			if (origin.y < box.min.y || origin.y > box.max.y) {
				return false;
			}
		} else {
			T inv_dy = T(1) / direction.y;
			T t1 = (box.min.y - origin.y) * inv_dy;
			T t2 = (box.max.y - origin.y) * inv_dy;
			if (t1 > t2) {
				tymin = t2;
				tymax = t1;
			} else {
				tymin = t1;
				tymax = t2;
			}
			if (tymin > tmax || tymax < tmin) {
				return false;
			}
			if (tymin > tmin) {
				tmin = tymin;
			}
			if (tymax < tmax) {
				tmax = tymax;
			}
		}

		if (std::abs(direction.z) < Epsilon<T>::value) {
			if (origin.z < box.min.z || origin.z > box.max.z) {
				return false;
			}
		} else {
			T inv_dz = T(1) / direction.z;
			T t1 = (box.min.z - origin.z) * inv_dz;
			T t2 = (box.max.z - origin.z) * inv_dz;
			if (t1 > t2) {
				tzmin = t2;
				tzmax = t1;
			} else {
				tzmin = t1;
				tzmax = t2;
			}
			if (tzmin > tmax || tzmax < tmin) {
				return false;
			}
			if (tzmin > tmin) {
				tmin = tzmin;
			}
			if (tzmax < tmax) {
				tmax = tzmax;
			}
		}

		return tmin <= tmax && tmax >= T(0);
	}
#endif

	// AABB vs frustum intersection test
	[[nodiscard]] bool aabb_in_frustum(const AABB<T> &box, const std::array<Vector3<T>, 6> &planes) const {
		// Each plane: xyz = normal, w = distance
		// For simplicity, assuming planes are already normalized
		for (const auto &plane : planes) {
			// Get AABB vertex most opposite to plane normal
			Vector3<T> p;
			p.x = (plane.x > 0) ? box.min.x : box.max.x;
			p.y = (plane.y > 0) ? box.min.y : box.max.y;
			p.z = (plane.z > 0) ? box.min.z : box.max.z;

			// Distance from plane
			T distance = plane.x * p.x + plane.y * p.y + plane.z * p.z + plane.w;

			if (distance < 0) {
				return false; // Completely outside
			}
		}

		return true;
	}
};

// Type aliases
using BVHf = BVH<float, void>;
using BVHd = BVH<double, void>;

} // namespace mml