/**************************************************************************/
/*  polygon.hpp                                                           */
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

#include "../algebra/vector2.hpp"
#include "../algebra/vector3.hpp"
#include "../core/config.hpp"
#include "../core/constants.hpp"
#include "../core/epsilon.hpp"
#include "../utils/math.hpp"
#include "aabb.hpp"
#include <cmath>
#include <type_traits>
#include <vector>

namespace mml {

template <typename T>
struct Polygon2D {
	static_assert(std::is_floating_point_v<T>, "Polygon2D requires floating-point type");

	std::vector<Vector2<T>> vertices;

	Polygon2D() = default;

	explicit Polygon2D(size_t count) :
			vertices(count) {}

	template <typename It>
	explicit Polygon2D(It begin, It end) :
			vertices(begin, end) {}

	MML_FORCE_INLINE size_t vertex_count() const {
		return vertices.size();
	}

	MML_FORCE_INLINE bool is_empty() const {
		return vertices.empty();
	}

	MML_FORCE_INLINE void clear() {
		vertices.clear();
	}

	MML_FORCE_INLINE void reserve(size_t capacity) {
		vertices.reserve(capacity);
	}

	MML_FORCE_INLINE void add_vertex(const Vector2<T> &vertex) {
		vertices.push_back(vertex);
	}

	MML_FORCE_INLINE void set_vertex(size_t index, const Vector2<T> &vertex) {
		vertices[index] = vertex;
	}

	MML_FORCE_INLINE const Vector2<T> &get_vertex(size_t index) const {
		return vertices[index];
	}

	MML_FORCE_INLINE Vector2<T> get_vertex(size_t index) {
		return vertices[index];
	}

	MML_FORCE_INLINE Vector2<T> get_edge(size_t index) const {
		size_t next_index = (index + 1) % vertices.size();
		return vertices[next_index] - vertices[index];
	}

	MML_FORCE_INLINE T signed_area() const {
		T area = T(0);
		size_t n = vertices.size();

		for (size_t i = 0; i < n; ++i) {
			size_t j = (i + 1) % n; // next vertex (wrapping around)
			area += (vertices[j].x - vertices[i].x) * (vertices[j].y + vertices[i].y); // shoelace formula
		}

		return area * T(0.5); // half of sum
	}

	MML_FORCE_INLINE T area() const {
		return std::abs(signed_area());
	}

	MML_FORCE_INLINE T perimeter() const {
		T perimeter = T(0);
		size_t n = vertices.size();

		for (size_t i = 0; i < n; ++i) {
			size_t j = (i + 1) % n;
			perimeter += vertices[i].distance(vertices[j]);
		}

		return perimeter;
	}

	MML_FORCE_INLINE Vector2<T> centroid() const {
		if (vertices.empty()) {
			return Vector2<T>(T(0), T(0)); // empty polygon
		}

		T a = T(0), x = T(0), y = T(0);
		size_t n = vertices.size();

		for (size_t i = 0; i < n; ++i) {
			size_t j = (i + 1) % n; // next vertex
			T f = vertices[i].x * vertices[j].y - vertices[j].x * vertices[i].y; // cross term
			a += f; // sum of cross terms
			x += (vertices[i].x + vertices[j].x) * f; // weighted x sum
			y += (vertices[i].y + vertices[j].y) * f; // weighted y sum
		}

		if (std::abs(a) < T(0.0001)) {
			return Vector2<T>(T(0), T(0)); // degenerate polygon
		}

		a *= T(0.5); // signed area
		return Vector2<T>(x / (T(6) * a), y / (T(6) * a)); // centroid formula
	}

	MML_FORCE_INLINE bool is_convex() const {
		if (vertices.size() < 3) {
			return false; // not enough vertices
		}

		Vector2<T> prev = get_edge(vertices.size() - 1); // previous edge

		for (size_t i = 0; i < vertices.size(); ++i) {
			Vector2<T> curr = get_edge(i); // current edge
			T cross = prev.x * curr.y - prev.y * curr.x; // 2D cross product

			if (cross < T(0)) {
				return false; // concave vertex found
			}

			prev = curr;
		}

		return true; // all turns are in same direction
	}

	MML_FORCE_INLINE bool is_clockwise() const {
		return signed_area() < T(0);
	}

	MML_FORCE_INLINE bool is_counter_clockwise() const {
		return signed_area() > T(0);
	}

	MML_FORCE_INLINE void reverse() {
		std::reverse(vertices.begin(), vertices.end());
	}

	MML_FORCE_INLINE void make_counter_clockwise() {
		if (is_clockwise()) {
			reverse();
		}
	}

	MML_FORCE_INLINE void make_clockwise() {
		if (is_counter_clockwise()) {
			reverse();
		}
	}

	MML_FORCE_INLINE bool contains(const Vector2<T> &point) const {
		size_t n = vertices.size();
		if (n < 3) {
			return false; // not a polygon
		}

		bool inside = false; // start with outside
		for (size_t i = 0, j = n - 1; i < n; j = i++) {
			const Vector2<T> &vi = vertices[i]; // current vertex
			const Vector2<T> &vj = vertices[j]; // previous vertex

			if (((vi.y > point.y) != (vj.y > point.y)) &&
					(point.x < (vj.x - vi.x) * (point.y - vi.y) / (vj.y - vi.y) + vi.x)) {
				inside = !inside; // toggle inside status
			}
		}

		return inside; // ray casting algorithm
	}

	MML_FORCE_INLINE bool contains_boundary(const Vector2<T> &point, T epsilon = Epsilon<T>::value) const {
		if (contains(point)) {
			return true;
		}

		for (size_t i = 0; i < vertices.size(); ++i) {
			size_t j = (i + 1) % vertices.size();
			Vector2<T> edge = vertices[j] - vertices[i];
			Vector2<T> to_point = point - vertices[i];
			T proj = to_point.dot(edge);
			T length_sq = edge.length_squared();

			if (proj >= T(0) && proj <= length_sq) {
				Vector2<T> closest = vertices[i] + edge * (proj / length_sq);
				if (closest.distance_squared(point) <= epsilon * epsilon) {
					return true;
				}
			}
		}

		return false;
	}

	MML_FORCE_INLINE Vector2<T> closest_point(const Vector2<T> &point) const {
		Vector2<T> closest = vertices[0];
		T min_dist_sq = (point - closest).length_squared();

		for (size_t i = 0; i < vertices.size(); ++i) {
			size_t j = (i + 1) % vertices.size();
			Vector2<T> edge_start = vertices[i];
			Vector2<T> edge_end = vertices[j];
			Vector2<T> edge = edge_end - edge_start;
			Vector2<T> to_point = point - edge_start;

			T t = to_point.dot(edge) / edge.length_squared();
			t = t < T(0) ? T(0) : (t > T(1) ? T(1) : t);

			Vector2<T> edge_closest = edge_start + edge * t;
			T dist_sq = (point - edge_closest).length_squared();

			if (dist_sq < min_dist_sq) {
				min_dist_sq = dist_sq;
				closest = edge_closest;
			}
		}

		return closest;
	}

	MML_FORCE_INLINE T distance_squared(const Vector2<T> &point) const {
		Vector2<T> closest = closest_point(point);
		return (point - closest).length_squared();
	}

	MML_FORCE_INLINE T distance(const Vector2<T> &point) const {
		return Math<T>::safe_sqrt(distance_squared(point));
	}

	MML_FORCE_INLINE AABB<T> bounding_box() const {
		if (vertices.empty()) {
			return AABB<T>(Vector2<T>(T(0), T(0)), Vector2<T>(T(0), T(0)));
		}

		Vector2<T> min_v = vertices[0];
		Vector2<T> max_v = vertices[0];

		for (const auto &v : vertices) {
			min_v = min_v.min(v);
			max_v = max_v.max(v);
		}

		return AABB<T>(min_v, max_v);
	}

	MML_FORCE_INLINE static Polygon2D triangle(const Vector2<T> &a, const Vector2<T> &b, const Vector2<T> &c) {
		Polygon2D poly(3);
		poly.add_vertex(a);
		poly.add_vertex(b);
		poly.add_vertex(c);
		return poly;
	}

	MML_FORCE_INLINE static Polygon2D rectangle(const Vector2<T> &min_v, const Vector2<T> &max_v) {
		Polygon2D poly(4);
		poly.add_vertex(Vector2<T>(min_v.x, min_v.y));
		poly.add_vertex(Vector2<T>(max_v.x, min_v.y));
		poly.add_vertex(Vector2<T>(max_v.x, max_v.y));
		poly.add_vertex(Vector2<T>(min_v.x, max_v.y));
		return poly;
	}

	MML_FORCE_INLINE static Polygon2D regular(T radius, size_t sides, const Vector2<T> &center = Vector2<T>(T(0), T(0)), T rotation = T(0)) {
		Polygon2D poly(sides);
		T angle_step = Constants<T>::two_pi / T(sides); // angle between vertices

		for (size_t i = 0; i < sides; ++i) {
			T angle = rotation + angle_step * T(i); // current angle
			Vector2<T> vertex(
					center.x + radius * std::cos(angle), // x = cx + r*cos(θ)
					center.y + radius * std::sin(angle)); // y = cy + r*sin(θ)
			poly.add_vertex(vertex);
		}

		return poly;
	}
};

using Polygon2Df = Polygon2D<float>;
using Polygon2Dd = Polygon2D<double>;

} // namespace mml