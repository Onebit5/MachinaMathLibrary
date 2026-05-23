/**************************************************************************/
/*  bspline.hpp                                                           */
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

#include "../algebra/vector3.hpp"
#include "../core/config.hpp"
#include "../core/epsilon.hpp"
#include "../utils/math.hpp"
#include <algorithm>
#include <cmath>
#include <type_traits>
#include <vector>

namespace mml {

template <typename T>
struct BSpline {
	static_assert(std::is_floating_point_v<T>, "BSpline requires floating-point type");

	std::vector<Vector3<T>> control_points;
	std::vector<T> knots;
	size_t degree = 3;

	BSpline() = default;

	BSpline(const std::vector<Vector3<T>> &points, size_t deg = 3) :
			control_points(points), degree(deg) {
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
			if (i < knots.size() - 1 && t >= knots[i] && t < knots[i + 1]) {
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

	[[nodiscard]] Vector3<T> evaluate(T t) const {
		if (control_points.empty()) {
			return Vector3<T>::zero();
		}
		if (control_points.size() == 1) {
			return control_points[0];
		}

		t = Math<T>::clamp(t, knots[degree], knots[knots.size() - degree - 1]);

		Vector3<T> result = Vector3<T>::zero();
		for (size_t i = 0; i < control_points.size(); ++i) {
			T basis = basis_function(i, degree, t);
			if (basis > Epsilon<T>::value) {
				result = result + control_points[i] * basis;
			}
		}
		return result;
	}

	[[nodiscard]] Vector3<T> tangent(T t) const {
		if (control_points.size() < 2) {
			return Vector3<T>::zero();
		}

		BSpline derivative_spline = derivative();
		return derivative_spline.evaluate(t).normalized();
	}

	[[nodiscard]] BSpline derivative() const {
		if (control_points.size() <= 1) {
			return BSpline();
		}

		std::vector<Vector3<T>> derivative_points;
		size_t n = control_points.size();

		for (size_t i = 0; i < n - 1; ++i) {
			T span = knots[i + static_cast<size_t>(degree) + 1] - knots[i + 1];
			T factor = Epsilon<T>::approx_zero(span) ? T(0) : T(degree) / span;
			derivative_points.push_back((control_points[i + 1] - control_points[i]) * factor);
		}

		BSpline result(derivative_points, degree - 1);
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
};

using BSplinef = BSpline<float>;
using BSplined = BSpline<double>;

} //namespace mml