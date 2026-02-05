/**************************************************************************/
/*  octree.hpp                                                            */
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

namespace mml {

template <typename T>
struct OctreeNode {
	AABB<T> bounds;
	std::unique_ptr<OctreeNode> children[8];
	std::vector<size_t> indices;
	bool is_leaf = true;

	OctreeNode() = default;
	explicit OctreeNode(const AABB<T> &b) :
			bounds(b) {}
};

template <typename T, typename PrimitiveType>
class Octree {
public:
	static_assert(std::is_floating_point_v<T>, "Octree requires floating-point type");

	using Node = OctreeNode<T>;

private:
	std::unique_ptr<Node> root;
	std::vector<PrimitiveType> primitives;
	std::function<AABB<T>(const PrimitiveType &)> bounds_func_;
	size_t max_depth = 8;
	size_t max_indices_per_leaf = 8;
	T min_node_size = T(0.001);

public:
	Octree() = default;

	explicit Octree(std::function<AABB<T>(const PrimitiveType &)> bounds_func) :
			bounds_func_(std::move(bounds_func)) {}

	void build(const std::vector<PrimitiveType> &prims,
			std::function<AABB<T>(const PrimitiveType &)> bounds_func = nullptr) {
		if (bounds_func) {
			bounds_func_ = std::move(bounds_func);
		}

		if (!bounds_func_ || prims.empty()) {
			return;
		}

		primitives = prims;

		AABB<T> total_bounds = AABB<T>::empty();
		for (const auto &p : primitives) {
			total_bounds = AABB<T>::merge(total_bounds, bounds_func_(p));
		}

		root = std::make_unique<Node>(total_bounds);
		std::vector<size_t> all_indices;
		all_indices.reserve(primitives.size());
		for (size_t i = 0; i < primitives.size(); ++i) {
			all_indices.push_back(i);
		}

		build_recursive(root.get(), all_indices, 0);
	}

	[[nodiscard]] std::vector<PrimitiveType *> ray_intersect(const Vector3<T> &origin, const Vector3<T> &direction) const {
		std::vector<PrimitiveType *> result;
		if (!root) {
			return result;
		}
		ray_intersect_recursive(root.get(), origin, direction, result);
		return result;
	}

	[[nodiscard]] std::vector<PrimitiveType *> point_query(const Vector3<T> &point) const {
		std::vector<PrimitiveType *> result;
		if (!root) {
			return result;
		}
		point_query_recursive(root.get(), point, result);
		return result;
	}

	[[nodiscard]] std::vector<PrimitiveType *> aabb_query(const AABB<T> &aabb) const {
		std::vector<PrimitiveType *> result;
		if (!root) {
			return result;
		}
		aabb_query_recursive(root.get(), aabb, result);
		return result;
	}

	[[nodiscard]] AABB<T> get_bounds() const {
		return root ? root->bounds : AABB<T>::empty();
	}

	[[nodiscard]] size_t node_count() const {
		return root ? count_nodes(root.get()) : 0;
	}

	void clear() {
		root.reset();
		primitives.clear();
	}

	void set_max_depth(size_t depth) {
		max_depth = std::max(size_t(1), depth);
	}

	void set_max_indices_per_leaf(size_t max_idx) {
		max_indices_per_leaf = std::max(size_t(1), max_idx);
	}

private:
	static int get_octant(const AABB<T> &node_bounds, const Vector3<T> &point) {
		Vector3<T> center = node_bounds.center();
		int octant = 0;
		if (point.x >= center.x) {
			octant |= 4;
		}
		if (point.y >= center.y) {
			octant |= 2;
		}
		if (point.z >= center.z) {
			octant |= 1;
		}
		return octant;
	}

	void build_recursive(Node *node, const std::vector<size_t> &indices, size_t depth) {
		if (indices.size() <= max_indices_per_leaf || depth >= max_depth ||
				node->bounds.extents().min_component() < min_node_size * T(2)) {
			node->indices = indices;
			node->is_leaf = true;
			return;
		}

		node->is_leaf = false;

		AABB<T> child_bounds[8];
		Vector3<T> center = node->bounds.center();
		Vector3<T> min_pt = node->bounds.min;
		Vector3<T> max_pt = node->bounds.max;

		child_bounds[0] = AABB<T>(min_pt, center);
		child_bounds[1] = AABB<T>(Vector3<T>(center.x, min_pt.y, min_pt.z), Vector3<T>(max_pt.x, center.y, center.z));
		child_bounds[2] = AABB<T>(Vector3<T>(min_pt.x, center.y, min_pt.z), Vector3<T>(center.x, max_pt.y, center.z));
		child_bounds[3] = AABB<T>(Vector3<T>(center.x, center.y, min_pt.z), Vector3<T>(max_pt.x, max_pt.y, center.z));
		child_bounds[4] = AABB<T>(Vector3<T>(min_pt.x, min_pt.y, center.z), Vector3<T>(center.x, center.y, max_pt.z));
		child_bounds[5] = AABB<T>(Vector3<T>(center.x, min_pt.y, center.z), Vector3<T>(max_pt.x, center.y, max_pt.z));
		child_bounds[6] = AABB<T>(Vector3<T>(min_pt.x, center.y, center.z), Vector3<T>(center.x, max_pt.y, max_pt.z));
		child_bounds[7] = AABB<T>(center, max_pt);

		std::vector<size_t> child_indices[8];
		for (size_t idx : indices) {
			const AABB<T> &prim_bounds = bounds_func_(primitives[idx]);
			Vector3<T> center_pt = prim_bounds.center();
			int octant = get_octant(node->bounds, center_pt);
			child_indices[octant].push_back(idx);
		}

		for (int i = 0; i < 8; ++i) {
			if (!child_indices[i].empty()) {
				node->children[i] = std::make_unique<Node>(child_bounds[i]);
				build_recursive(node->children[i].get(), child_indices[i], depth + 1);
			}
		}

		if (indices.empty()) {
			node->indices.clear();
			node->is_leaf = true;
		}
	}

	void ray_intersect_recursive(Node *node, const Vector3<T> &origin,
			const Vector3<T> &direction, std::vector<PrimitiveType *> &result) const {
		if (!node) {
			return;
		}

		T tmin, tmax;
		if (!node->bounds.intersect_ray(Ray<T>(origin, direction), tmin, tmax)) {
			return;
		}

		if (node->is_leaf) {
			for (size_t idx : node->indices) {
				result.push_back(const_cast<PrimitiveType *>(&primitives[idx]));
			}
		} else {
			for (int i = 0; i < 8; ++i) {
				if (node->children[i]) {
					ray_intersect_recursive(node->children[i].get(), origin, direction, result);
				}
			}
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
			for (size_t idx : node->indices) {
				if (bounds_func_(primitives[idx]).contains(point)) {
					result.push_back(const_cast<PrimitiveType *>(&primitives[idx]));
				}
			}
		} else {
			for (int i = 0; i < 8; ++i) {
				if (node->children[i]) {
					point_query_recursive(node->children[i].get(), point, result);
				}
			}
		}
	}

	void aabb_query_recursive(Node *node, const AABB<T> &aabb,
			std::vector<PrimitiveType *> &result) const {
		if (!node) {
			return;
		}

		AABB<T> intersection = AABB<T>::intersection(node->bounds, aabb);
		if (intersection.is_empty()) {
			return;
		}

		if (node->is_leaf) {
			for (size_t idx : node->indices) {
				const AABB<T> &prim_bounds = bounds_func_(primitives[idx]);
				AABB<T> prim_intersection = AABB<T>::intersection(prim_bounds, aabb);
				if (!prim_intersection.is_empty()) {
					result.push_back(const_cast<PrimitiveType *>(&primitives[idx]));
				}
			}
		} else {
			for (int i = 0; i < 8; ++i) {
				if (node->children[i]) {
					aabb_query_recursive(node->children[i].get(), aabb, result);
				}
			}
		}
	}

	size_t count_nodes(Node *node) const {
		if (!node) {
			return 0;
		}
		size_t count = 1;
		for (int i = 0; i < 8; ++i) {
			if (node->children[i]) {
				count += count_nodes(node->children[i].get());
			}
		}
		return count;
	}
};

using Octreef = Octree<float, void>;
using Octreed = Octree<double, void>;

} //namespace mml