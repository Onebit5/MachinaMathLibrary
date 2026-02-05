/**************************************************************************/
/*  nurbs.hpp                                                             */
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
#include "../core/constants.hpp"
#include "../core/epsilon.hpp"
#include "../utils/math.hpp"
#include <algorithm>
#include <cmath>
#include <type_traits>
#include <vector>

namespace mml {

template <typename T>
struct NURBS {
	static_assert(std::is_floating_point_v<T>, "NURBS requires floating-point type");

	std::vector<Vector3<T>> control_points;
	std::vector<T> weights;
	std::vector<T> knots;
	size_t degree = 3;

	NURBS() = default;

	NURBS(const std::vector<Vector3<T>> &points, const std::vector<T> &w = {}, size_t deg = 3) :
			control_points(points), degree(deg) {
		size_t n = points.size();
		weights.resize(n, T(1));
		for (size_t i = 0; i < std::min(w.size(), n); ++i) {
			weights[i] = w[i];
		}
		initialize_knots();
	}

	void initialize_knots() {
		size_t n = control_points.size();
		if (n <= degree) {
			knots.clear();
			return;
		}
		knots.resize(n + degree + 1);
		size_t m = n + degree + 1;
		for (size_t i = 0; i < m; ++i) {
			if (i <= degree) {
				knots[i] = T(0);
			} else if (i >= n) {
				knots[i] = T(1);
			} else {
				knots[i] = T(i - degree) / T(n - degree);
			}
		}
	}

	[[nodiscard]] T basis_function(size_t i, size_t p, T t) const {
		if (p == 0) {
			if (i < knots.size() - 1 && t >= knots[i] && (t < knots[i + 1] || Math<T>::abs(t - knots[i + 1]) < Epsilon<T>::value)) {
				return T(1);
			}
			return T(0);
		}

		T left = T(0);
		T right = T(0);

		if (i + p < knots.size() && knots[i + p] != knots[i]) {
			left = (t - knots[i]) / (knots[i + p] - knots[i]) * basis_function(i, p - 1, t);
		}
		if (i + p + 1 < knots.size() && knots[i + p + 1] != knots[i + 1]) {
			right = (knots[i + p + 1] - t) / (knots[i + p + 1] - knots[i + 1]) * basis_function(i + 1, p - 1, t);
		}

		return left + right;
	}

	[[nodiscard]] T rational_basis_function(size_t i, size_t p, T t) const {
		T basis = basis_function(i, p, t);
		if (basis <= Epsilon<T>::value) {
			return T(0);
		}

		T w = T(0);
		for (size_t j = 0; j < control_points.size(); ++j) {
			T b = basis_function(j, p, t);
			w += weights[j] * b;
		}

		if (w <= Epsilon<T>::value) {
			return T(0);
		}

		return (weights[i] * basis) / w;
	}

	[[nodiscard]] Vector3<T> evaluate(T t) const {
		if (control_points.empty()) {
			return Vector3<T>::zero();
		}
		if (control_points.size() == 1) {
			return control_points[0];
		}

		T t_min = knots[degree];
		T t_max = knots[knots.size() - degree - 1];

		if (t <= t_min) {
			return control_points[0];
		}
		if (t >= t_max) {
			return control_points.back();
		}

		Vector3<T> numerator = Vector3<T>::zero();
		T denominator = T(0);

		for (size_t i = 0; i < control_points.size(); ++i) {
			T rbf = rational_basis_function(i, degree, t);
			if (rbf > Epsilon<T>::value) {
				numerator = numerator + control_points[i] * rbf;
				denominator += rbf;
			}
		}

		if (denominator < Epsilon<T>::value) {
			return Vector3<T>::zero();
		}

		return numerator / denominator;
	}

	[[nodiscard]] Vector3<T> tangent(T t) const {
		if (control_points.size() < 2) {
			return Vector3<T>::zero();
		}

		NURBS derivative_nurbs = derivative();
		Vector3<T> deriv = derivative_nurbs.evaluate(t);
		T len = deriv.length();
		if (len < Epsilon<T>::value) {
			return Vector3<T>::zero();
		}
		return deriv / len;
	}

	[[nodiscard]] NURBS derivative() const {
		if (control_points.size() <= 1) {
			return NURBS();
		}

		std::vector<Vector3<T>> derivative_points;
		std::vector<T> derivative_weights;
		size_t n = control_points.size();

		for (size_t i = 0; i < n - 1; ++i) {
			T factor = degree * T(n) / T(n - 1);
			Vector3<T> p1 = control_points[i] * weights[i];
			Vector3<T> p2 = control_points[i + 1] * weights[i + 1];
			Vector3<T> diff = (p2 - p1) * factor;
			T w_diff = weights[i + 1] - weights[i];

			derivative_points.push_back(diff);
			derivative_weights.push_back(w_diff);
		}

		NURBS result(derivative_points, derivative_weights, degree - 1);
		result.knots = std::vector<T>(knots.begin() + 1, knots.end() - 1);

		return result;
	}

	[[nodiscard]] std::vector<Vector3<T>> sample(size_t num_samples) const {
		std::vector<Vector3<T>> samples;
		if (control_points.empty()) {
			return samples;
		}

		T t_min = knots[degree];
		T t_max = knots[knots.size() - degree - 1];

		for (size_t i = 0; i < num_samples; ++i) {
			T t = t_min + (t_max - t_min) * T(i) / T(num_samples - 1);
			samples.push_back(evaluate(t));
		}

		return samples;
	}

	[[nodiscard]] T length(size_t samples = 100) const {
		auto pts = sample(samples);
		T len = T(0);
		for (size_t i = 1; i < pts.size(); ++i) {
			len += (pts[i] - pts[i - 1]).length();
		}
		return len;
	}

	static NURBS circle(T radius, size_t segments = 32, size_t deg = 2) {
		std::vector<Vector3<T>> points;
		std::vector<T> w;

		for (size_t i = 0; i < segments; ++i) {
			T angle = T(2) * Constants<T>::pi * T(i) / T(segments);
			points.emplace_back(Math<T>::cos(angle) * radius, T(0), Math<T>::sin(angle) * radius);
			w.push_back(Math<T>::cos(Constants<T>::pi * T(i) / T(segments)) + T(1));
		}

		std::vector<T> knots = {
			T(0), T(0), T(0),
			T(1) / T(3), T(1) / T(3),
			T(2) / T(3), T(2) / T(3),
			T(1), T(1), T(1)
		};

		NURBS circle_nurbs(points, w, deg);
		circle_nurbs.knots = knots;

		return circle_nurbs;
	}

	static NURBS circle_2d(T radius, size_t segments = 32, size_t deg = 2) {
		std::vector<Vector3<T>> points;
		std::vector<T> w;

		for (size_t i = 0; i < segments; ++i) {
			T angle = T(2) * Constants<T>::pi * T(i) / T(segments);
			points.emplace_back(Math<T>::cos(angle) * radius, Math<T>::sin(angle) * radius, T(0));
			w.push_back(Math<T>::cos(Constants<T>::pi * T(i) / T(segments)) + T(1));
		}

		std::vector<T> knots = {
			T(0), T(0), T(0),
			T(1) / T(3), T(1) / T(3),
			T(2) / T(3), T(2) / T(3),
			T(1), T(1), T(1)
		};

		NURBS circle_nurbs(points, w, deg);
		circle_nurbs.knots = knots;

		return circle_nurbs;
	}

	static NURBS cone(T height, T radius, size_t segments = 32, size_t deg = 2) {
		std::vector<Vector3<T>> points;
		std::vector<T> w;

		points.emplace_back(T(0), T(0), T(0));
		w.push_back(T(1));

		for (size_t i = 0; i < segments; ++i) {
			T angle = T(2) * Math<T>::pi() * T(i) / T(segments);
			points.emplace_back(Math<T>::cos(angle) * radius, height, Math<T>::sin(angle) * radius);
			w.push_back(T(1));
		}

		points.emplace_back(T(0), T(0), T(0));
		w.push_back(T(1));

		std::vector<T> knots = {
			T(0), T(0), T(0),
			T(1) / T(3), T(1) / T(3),
			T(2) / T(3), T(2) / T(3),
			T(1), T(1), T(1)
		};

		NURBS cone_nurbs(points, w, deg);
		cone_nurbs.knots = knots;

		return cone_nurbs;
	}

	static NURBS sphere(T radius, size_t segments = 32, size_t rings = 16, size_t deg = 2) {
		std::vector<Vector3<T>> points;
		std::vector<T> w;

		for (size_t ring = 0; ring <= rings; ++ring) {
			T phi = Math<T>::pi() * T(ring) / T(rings);
			T y = Math<T>::cos(phi) * radius;
			T ring_radius = Math<T>::sin(phi) * radius;
			T ring_weight = Math<T>::cos(phi / T(2)) + T(1);

			for (size_t i = 0; i <= segments; ++i) {
				T theta = T(2) * Math<T>::pi() * T(i) / T(segments);
				points.emplace_back(Math<T>::cos(theta) * ring_radius, y, Math<T>::sin(theta) * ring_radius);
				w.push_back(ring_weight);
			}
		}

		std::vector<T> knots_u = { T(0), T(0), T(0), T(1) / T(2), T(1) / T(2), T(1), T(1), T(1) };
		std::vector<T> knots_v = { T(0), T(0), T(0), T(1) / T(2), T(1) / T(2), T(1), T(1), T(1) };

		NURBS sphere_nurbs(points, w, deg);
		sphere_nurbs.knots = knots_u;

		return sphere_nurbs;
	}
};

using NURBSf = NURBS<float>;
using NURBSd = NURBS<double>;

} //namespace mml