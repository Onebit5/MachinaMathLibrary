/**************************************************************************/
/*  kdtree.hpp                                                            */
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
#include <cmath>
#include <functional>
#include <limits>
#include <memory>
#include <numeric>
#include <type_traits>
#include <vector>

namespace mml {

enum class SplitAxis {
	X = 0,
	Y = 1,
	Z = 2
};

template <typename T>
struct KDNode {
	AABB<T> bounds;
	std::unique_ptr<KDNode> left;
	std::unique_ptr<KDNode> right;
	std::vector<size_t> indices;
	SplitAxis split_axis = SplitAxis::X;
	T split_position = T(0);
	bool is_leaf = false;

	KDNode() = default;
	explicit KDNode(const AABB<T> &b) :
			bounds(b) {}
};

template <typename T, typename PrimitiveType>
class KDTree {
public:
	static_assert(std::is_floating_point_v<T>, "KDTree requires floating-point type");

	using Node = KDNode<T>;

private:
	std::unique_ptr<Node> root;
	std::vector<PrimitiveType> primitives;
	std::function<AABB<T>(const PrimitiveType &)> bounds_func_;
	size_t max_depth = 12;
	size_t max_indices_per_leaf = 16;

public:
	KDTree() = default;

	explicit KDTree(std::function<AABB<T>(const PrimitiveType &)> bounds_func) :
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
		for (size_t i = 0; i < primitives.size(); ++i) {
			total_bounds = AABB<T>::merge(total_bounds, bounds_func_(primitives[i]));
		}

		std::vector<size_t> all_indices(primitives.size());
		std::iota(all_indices.begin(), all_indices.end(), 0);

		root = std::make_unique<Node>(total_bounds);
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

	[[nodiscard]] size_t depth() const {
		return root ? get_depth(root.get()) : 0;
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
	static SplitAxis get_longest_axis(const AABB<T> &bounds) {
		Vector3<T> extents = bounds.extents();
		if (extents.x >= extents.y && extents.x >= extents.z) {
			return SplitAxis::X;
		} else if (extents.y >= extents.z) {
			return SplitAxis::Y;
		} else {
			return SplitAxis::Z;
		}
	}

	static T get_center(const AABB<T> &bounds, SplitAxis axis) {
		return bounds.min[static_cast<int>(axis)] + bounds.extents()[static_cast<int>(axis)] * T(0.5);
	}

	static T get_centroid(const PrimitiveType &prim, const std::function<AABB<T>(const PrimitiveType &)> &bounds_func, SplitAxis axis) {
		AABB<T> bounds = bounds_func(prim);
		return bounds.center()[static_cast<int>(axis)];
	}

	void build_recursive(Node *node, std::vector<size_t> &indices, size_t depth) {
		if (indices.size() <= max_indices_per_leaf || depth >= max_depth) {
			node->indices = indices;
			node->is_leaf = true;
			return;
		}

		node->split_axis = get_longest_axis(node->bounds);

		std::sort(indices.begin(), indices.end(),
				[this, node](size_t a, size_t b) {
					return get_centroid(primitives[a], bounds_func_, node->split_axis) <
							get_centroid(primitives[b], bounds_func_, node->split_axis);
				});

		size_t mid = indices.size() / 2;
		node->split_position = get_centroid(primitives[indices[mid]], bounds_func_, node->split_axis);

		std::vector<size_t> left_indices(indices.begin(), indices.begin() + mid);
		std::vector<size_t> right_indices(indices.begin() + mid, indices.end());

		AABB<T> left_bounds = node->bounds;
		AABB<T> right_bounds = node->bounds;

		switch (node->split_axis) {
			case SplitAxis::X:
				left_bounds.max.x = node->split_position;
				right_bounds.min.x = node->split_position;
				break;
			case SplitAxis::Y:
				left_bounds.max.y = node->split_position;
				right_bounds.min.y = node->split_position;
				break;
			case SplitAxis::Z:
				left_bounds.max.z = node->split_position;
				right_bounds.min.z = node->split_position;
				break;
		}

		if (!left_indices.empty()) {
			node->left = std::make_unique<Node>(left_bounds);
			build_recursive(node->left.get(), left_indices, depth + 1);
		}

		if (!right_indices.empty()) {
			node->right = std::make_unique<Node>(right_bounds);
			build_recursive(node->right.get(), right_indices, depth + 1);
		}

		if (left_indices.empty() || right_indices.empty()) {
			node->indices = indices;
			node->is_leaf = true;
			node->left.reset();
			node->right.reset();
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
			T split_t = (node->split_position - origin[static_cast<int>(node->split_axis)]) /
					direction[static_cast<int>(node->split_axis)];

			KDNode<T> *first = node->left.get();
			KDNode<T> *second = node->right.get();

			if (direction[static_cast<int>(node->split_axis)] > T(0)) {
				if (origin[static_cast<int>(node->split_axis)] < node->split_position) {
					first = node->left.get();
					second = node->right.get();
				} else {
					first = node->right.get();
					second = node->left.get();
				}
			} else {
				if (origin[static_cast<int>(node->split_axis)] > node->split_position) {
					first = node->left.get();
					second = node->right.get();
				} else {
					first = node->right.get();
					second = node->left.get();
				}
			}

			if (first) {
				ray_intersect_recursive(first, origin, direction, result);
			}
			if (split_t > tmin && second) {
				ray_intersect_recursive(second, origin, direction, result);
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
			point_query_recursive(node->left.get(), point, result);
			point_query_recursive(node->right.get(), point, result);
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
			aabb_query_recursive(node->left.get(), aabb, result);
			aabb_query_recursive(node->right.get(), aabb, result);
		}
	}

	size_t count_nodes(Node *node) const {
		if (!node) {
			return 0;
		}
		return 1 + count_nodes(node->left.get()) + count_nodes(node->right.get());
	}

	size_t get_depth(Node *node) const {
		if (!node) {
			return 0;
		}
		size_t left_depth = get_depth(node->left.get());
		size_t right_depth = get_depth(node->right.get());
		return 1 + std::max(left_depth, right_depth);
	}
};

using KDTreef = KDTree<float, void>;
using KDDreed = KDTree<double, void>;

} // namespace mml