/**************************************************************************/
/*  curves.hpp                                                            */
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
#include "../utils/math.hpp"
#include <array>
#include <cmath>
#include <type_traits>
#include <utility>
#include <vector>

namespace mml {

// ============================================================================
// Curve Frame (Tangent, Normal, Binormal)
// ============================================================================

template <typename T>
struct CurveFrame {
	Vector3<T> position;
	Vector3<T> tangent;
	Vector3<T> normal;
	Vector3<T> binormal;

	CurveFrame() = default;

	CurveFrame(const Vector3<T> &pos, const Vector3<T> &tan, const Vector3<T> &norm, const Vector3<T> &bin) :
			position(pos), tangent(tan), normal(norm), binormal(bin) {}
};

// ============================================================================
// Bezier Curves
// ============================================================================

// Quadratic Bezier curve (3 control points)
template <typename T>
struct BezierQuadratic {
	static_assert(std::is_floating_point_v<T>, "BezierQuadratic requires floating-point type");

	std::array<Vector3<T>, 3> points;

	BezierQuadratic() = default;

	BezierQuadratic(const Vector3<T> &p0, const Vector3<T> &p1, const Vector3<T> &p2) :
			points{ p0, p1, p2 } {}

	// Evaluate at parameter t (0 to 1)
	[[nodiscard]] Vector3<T> evaluate(T t) const {
		T u = T(1) - t;
		T tt = t * t;
		T uu = u * u;

		return points[0] * uu + points[1] * (T(2) * u * t) + points[2] * tt;
	}

	// Evaluate tangent at parameter t
	[[nodiscard]] Vector3<T> tangent(T t) const {
		return (points[1] - points[0]) * (T(2) * (T(1) - t)) +
				(points[2] - points[1]) * (T(2) * t);
	}

	// Evaluate second derivative (constant for quadratic)
	[[nodiscard]] Vector3<T> second_derivative() const {
		return (points[2] - points[1] * T(2) + points[0]) * T(2);
	}

	// Subdivide curve at parameter t into two curves
	[[nodiscard]] std::pair<BezierQuadratic, BezierQuadratic> subdivide(T t) const {
		Vector3<T> p01 = points[0].lerp(points[1], t);
		Vector3<T> p12 = points[1].lerp(points[2], t);
		Vector3<T> p012 = p01.lerp(p12, t);

		return {
			BezierQuadratic(points[0], p01, p012),
			BezierQuadratic(p012, p12, points[2])
		};
	}

	// Calculate approximate length using n segments
	[[nodiscard]] T length(int segments = 10) const {
		T total_length = T(0);
		Vector3<T> prev = points[0];

		for (int i = 1; i <= segments; ++i) {
			T t = T(i) / T(segments);
			Vector3<T> curr = evaluate(t);
			total_length += (curr - prev).length();
			prev = curr;
		}

		return total_length;
	}

	// Get control point
	[[nodiscard]] const Vector3<T> &operator[](size_t i) const { return points[i]; }
	Vector3<T> &operator[](size_t i) { return points[i]; }

	// Flatten curve into line segments for rendering
	[[nodiscard]] std::vector<Vector3<T>> flatten(T tolerance = T(0.1)) const {
		std::vector<Vector3<T>> result;
		result.push_back(points[0]);
		flatten_recursive(points[0], points[1], points[2], tolerance, result);
		return result;
	}

private:
	void flatten_recursive(const Vector3<T> &p0, const Vector3<T> &p1, const Vector3<T> &p2, T tolerance, std::vector<Vector3<T>> &result) const {
		Vector3<T> mid01 = (p0 + p1) * T(0.5);
		Vector3<T> mid12 = (p1 + p2) * T(0.5);
		Vector3<T> mid012 = (mid01 + mid12) * T(0.5);

		Vector3<T> segment_mid = (p0 + p2) * T(0.5);
		Vector3<T> dist_vec = mid012 - segment_mid;

		if (dist_vec.length() > tolerance) {
			flatten_recursive(p0, mid01, mid012, tolerance, result);
			flatten_recursive(mid012, mid12, p2, tolerance, result);
		} else {
			result.push_back(p2);
		}
	}
};

// Cubic Bezier curve (4 control points)
template <typename T>
struct BezierCubic {
	static_assert(std::is_floating_point_v<T>, "BezierCubic requires floating-point type");

	std::array<Vector3<T>, 4> points;

	BezierCubic() = default;

	BezierCubic(const Vector3<T> &p0, const Vector3<T> &p1,
			const Vector3<T> &p2, const Vector3<T> &p3) :
			points{ p0, p1, p2, p3 } {}

	// Evaluate at parameter t (0 to 1)
	[[nodiscard]] Vector3<T> evaluate(T t) const {
		T u = T(1) - t;
		T tt = t * t;
		T uu = u * u;
		T uuu = uu * u;
		T ttt = tt * t;

		return points[0] * uuu +
				points[1] * (T(3) * uu * t) +
				points[2] * (T(3) * u * tt) +
				points[3] * ttt;
	}

	// Evaluate tangent at parameter t
	[[nodiscard]] Vector3<T> tangent(T t) const {
		T u = T(1) - t;

		return (points[1] - points[0]) * (T(3) * u * u) +
				(points[2] - points[1]) * (T(6) * u * t) +
				(points[3] - points[2]) * (T(3) * t * t);
	}

	// Evaluate second derivative at parameter t
	[[nodiscard]] Vector3<T> second_derivative(T t) const {
		return (points[2] - points[1] * T(2) + points[0]) * (T(6) * (T(1) - t)) +
				(points[3] - points[2] * T(2) + points[1]) * (T(6) * t);
	}

	// Evaluate curvature at parameter t
	[[nodiscard]] T curvature(T t) const {
		Vector3<T> first = tangent(t);
		Vector3<T> second = second_derivative(t);

		Vector3<T> cross = first.cross(second);
		T first_len = first.length();

		if (Epsilon<T>::approx_zero(first_len)) {
			return T(0);
		}

		return cross.length() / (first_len * first_len * first_len);
	}

	// Subdivide curve at parameter t into two curves
	[[nodiscard]] std::pair<BezierCubic, BezierCubic> subdivide(T t) const {
		Vector3<T> p01 = points[0].lerp(points[1], t);
		Vector3<T> p12 = points[1].lerp(points[2], t);
		Vector3<T> p23 = points[2].lerp(points[3], t);
		Vector3<T> p012 = p01.lerp(p12, t);
		Vector3<T> p123 = p12.lerp(p23, t);
		Vector3<T> p0123 = p012.lerp(p123, t);

		return {
			BezierCubic(points[0], p01, p012, p0123),
			BezierCubic(p0123, p123, p23, points[3])
		};
	}

	// Calculate approximate length using n segments
	[[nodiscard]] T length(int segments = 20) const {
		T total_length = T(0);
		Vector3<T> prev = points[0];

		for (int i = 1; i <= segments; ++i) {
			T ti = T(i) / T(segments);
			Vector3<T> curr = evaluate(ti);
			total_length += (curr - prev).length();
			prev = curr;
		}

		return total_length;
	}

	// Find parameter t for arc length s using binary search
	[[nodiscard]] T reparameterize(T s, int iterations = 10) const {
		T total_length = length();
		if (Epsilon<T>::approx_zero(total_length)) {
			return T(0);
		}

		T target_length = s * total_length;
		T low = T(0), high = T(1);

		for (int i = 0; i < iterations; ++i) {
			T mid = (low + high) * T(0.5);
			T mid_length = BezierCubic(points[0],
					points[0].lerp(points[1], mid),
					points[0].lerp(points[1], mid).lerp(points[1].lerp(points[2], mid), mid),
					evaluate(mid))
								   .length();

			if (mid_length < target_length) {
				low = mid;
			} else {
				high = mid;
			}
		}

		return (low + high) * T(0.5);
	}

	// Evaluate normal at parameter t
	[[nodiscard]] Vector3<T> normal(T t) const {
		Vector3<T> tan = tangent(t);
		Vector3<T> d2 = second_derivative(t);

		Vector3<T> bin = tan.cross(d2);
		if (bin.length() < Epsilon<T>::value) {
			return Vector3<T>::up();
		}
		return bin.cross(tan).normalized();
	}

	// Evaluate frame (position, tangent, normal, binormal) at parameter t
	[[nodiscard]] CurveFrame<T> frame(T t) const {
		Vector3<T> pos = evaluate(t);
		Vector3<T> tan = tangent(t);
		Vector3<T> norm = normal(t);
		Vector3<T> bin = tan.cross(norm);

		return CurveFrame<T>(pos, tan, norm, bin);
	}

	// Get control point
	[[nodiscard]] const Vector3<T> &operator[](size_t i) const { return points[i]; }
	Vector3<T> &operator[](size_t i) { return points[i]; }

	// Flatten curve into line segments for rendering
	// Returns points along the curve with max error <= tolerance
	[[nodiscard]] std::vector<Vector3<T>> flatten(T tolerance = T(0.1)) const {
		std::vector<Vector3<T>> result;
		result.push_back(points[0]);
		flatten_recursive(points[0], points[1], points[2], points[3], tolerance, result);
		return result;
	}

private:
	void flatten_recursive(const Vector3<T> &p0, const Vector3<T> &p1, const Vector3<T> &p2, const Vector3<T> &p3, T tolerance, std::vector<Vector3<T>> &result) const {
		Vector3<T> mid01 = (p0 + p1) * T(0.5);
		Vector3<T> mid12 = (p1 + p2) * T(0.5);
		Vector3<T> mid23 = (p2 + p3) * T(0.5);
		Vector3<T> mid012 = (mid01 + mid12) * T(0.5);
		Vector3<T> mid123 = (mid12 + mid23) * T(0.5);
		Vector3<T> mid0123 = (mid012 + mid123) * T(0.5);

		Vector3<T> segment_mid = (p0 + p3) * T(0.5);
		Vector3<T> dist_vec = mid0123 - segment_mid;

		if (dist_vec.length() > tolerance) {
			flatten_recursive(p0, mid01, mid012, mid0123, tolerance, result);
			flatten_recursive(mid0123, mid123, mid23, p3, tolerance, result);
		} else {
			result.push_back(p3);
		}
	}
};

// ============================================================================
// Catmull-Rom Spline
// ============================================================================
// Catmull-Rom Spline
// ============================================================================

// C1 continuous spline that passes through all control points
template <typename T>
struct CatmullRomSpline {
	static_assert(std::is_floating_point_v<T>, "CatmullRomSpline requires floating-point type");

	std::vector<Vector3<T>> points;
	bool closed = false;

	CatmullRomSpline() = default;

	explicit CatmullRomSpline(const std::vector<Vector3<T>> &pts, bool is_closed = false) :
			points(pts), closed(is_closed) {}

	void add_point(const Vector3<T> &point) {
		points.push_back(point);
	}

	// Get segment index and local parameter from global t [0, 1]
	[[nodiscard]] std::pair<size_t, T> get_segment(T t) const {
		if (points.size() < 2) {
			return { 0, T(0) };
		}

		size_t segments = closed ? points.size() : points.size() - 1;
		if (segments == 0) {
			return { 0, T(0) };
		}

		t = Math<T>::clamp(t, T(0), T(1));
		T scaled_t = t * T(segments);
		size_t segment = static_cast<size_t>(scaled_t);
		segment = std::min(segment, segments - 1);

		T local_t = scaled_t - T(segment);
		return { segment, local_t };
	}

	// Evaluate at parameter t [0, 1]
	[[nodiscard]] Vector3<T> evaluate(T t) const {
		if (points.empty()) {
			return Vector3<T>::zero();
		}
		if (points.size() == 1) {
			return points[0];
		}

		auto [segment, local_t] = get_segment(t);
		return evaluate_segment(segment, local_t);
	}

	// Evaluate specific segment at local t [0, 1]
	[[nodiscard]] Vector3<T> evaluate_segment(size_t segment, T t) const {
		if (points.size() < 2) {
			return points.empty() ? Vector3<T>::zero() : points[0];
		}

		size_t n = points.size();

		// Get four control points for the segment
		Vector3<T> p0, p1, p2, p3;

		if (closed) {
			p1 = points[segment % n];
			p2 = points[(segment + 1) % n];
			p0 = points[(segment + n - 1) % n];
			p3 = points[(segment + 2) % n];
		} else {
			p1 = points[segment];
			p2 = points[std::min(segment + 1, n - 1)];
			p0 = (segment == 0) ? p1 - (p2 - p1) : points[segment - 1];
			p3 = (segment + 2 >= n) ? p2 + (p2 - p1) : points[segment + 2];
		}

		// Catmull-Rom basis matrix multiplication
		T tt = t * t;
		T ttt = tt * t;

		return p0 * (-T(0.5) * t + tt - T(0.5) * ttt) +
				p1 * (T(1) - T(2.5) * tt + T(1.5) * ttt) +
				p2 * (T(0.5) * t + T(2) * tt - T(1.5) * ttt) +
				p3 * (-T(0.5) * tt + T(0.5) * ttt);
	}

	// Evaluate tangent at parameter t
	[[nodiscard]] Vector3<T> tangent(T t) const {
		auto [segment, local_t] = get_segment(t);

		// Use central difference for smoother tangents
		T dt = T(0.001);
		Vector3<T> p_before = evaluate_segment(segment, Math<T>::clamp(local_t - dt, T(0), T(1)));
		Vector3<T> p_after = evaluate_segment(segment, Math<T>::clamp(local_t + dt, T(0), T(1)));

		return (p_after - p_before).normalized();
	}

	// Get total approximate length
	[[nodiscard]] T length(int samples_per_segment = 10) const {
		if (points.size() < 2) {
			return T(0);
		}

		size_t segments = closed ? points.size() : points.size() - 1;
		T total = T(0);

		for (size_t seg = 0; seg < segments; ++seg) {
			Vector3<T> prev = evaluate_segment(seg, T(0));
			for (int i = 1; i <= samples_per_segment; ++i) {
				T t = T(i) / T(samples_per_segment);
				Vector3<T> curr = evaluate_segment(seg, t);
				total += (curr - prev).length();
				prev = curr;
			}
		}

		return total;
	}

	// Sample the curve uniformly in arc length
	[[nodiscard]] std::vector<Vector3<T>> sample_uniform(size_t num_samples) const {
		std::vector<Vector3<T>> result;
		result.reserve(num_samples);

		for (size_t i = 0; i < num_samples; ++i) {
			T t = T(i) / T(num_samples - 1);
			result.push_back(evaluate(t));
		}

		return result;
	}

	// Evaluate normal at parameter t
	[[nodiscard]] Vector3<T> normal(T t) const {
		Vector3<T> tan = tangent(t);
		auto [segment, local_t] = get_segment(t);

		T dt = T(0.001);
		Vector3<T> p_before = evaluate_segment(segment, Math<T>::clamp(local_t - dt, T(0), T(1)));
		Vector3<T> p_after = evaluate_segment(segment, Math<T>::clamp(local_t + dt, T(0), T(1)));
		Vector3<T> d2 = (p_after - p_before * T(2) + p_before).normalized();

		Vector3<T> bin = tan.cross(d2);
		if (bin.length() < Epsilon<T>::value) {
			return Vector3<T>::up();
		}
		return bin.cross(tan).normalized();
	}

	// Evaluate frame at parameter t
	[[nodiscard]] CurveFrame<T> frame(T t) const {
		Vector3<T> pos = evaluate(t);
		Vector3<T> tan = tangent(t);
		Vector3<T> norm = normal(t);
		Vector3<T> bin = tan.cross(norm);

		return CurveFrame<T>(pos, tan, norm, bin);
	}
};

// ============================================================================
// Hermite Spline
// ============================================================================

// Piecewise cubic curve with position and tangent control at each point
template <typename T>
struct HermiteSpline {
	static_assert(std::is_floating_point_v<T>, "HermiteSpline requires floating-point type");

	struct ControlPoint {
		Vector3<T> position;
		Vector3<T> tangent;

		ControlPoint() = default;
		ControlPoint(const Vector3<T> &pos, const Vector3<T> &tan) :
				position(pos), tangent(tan) {}
	};

	std::vector<ControlPoint> points;

	HermiteSpline() = default;

	explicit HermiteSpline(const std::vector<ControlPoint> &pts) :
			points(pts) {}

	void add_point(const Vector3<T> &position, const Vector3<T> &tangent) {
		points.emplace_back(position, tangent);
	}

	// Get segment index and local parameter from global t [0, 1]
	[[nodiscard]] std::pair<size_t, T> get_segment(T t) const {
		if (points.size() < 2) {
			return { 0, T(0) };
		}

		size_t segments = points.size() - 1;
		t = Math<T>::clamp(t, T(0), T(1));
		T scaled_t = t * T(segments);
		size_t segment = static_cast<size_t>(scaled_t);
		segment = std::min(segment, segments - 1);

		T local_t = scaled_t - T(segment);
		return { segment, local_t };
	}

	// Evaluate at parameter t [0, 1]
	[[nodiscard]] Vector3<T> evaluate(T t) const {
		if (points.empty()) {
			return Vector3<T>::zero();
		}
		if (points.size() == 1) {
			return points[0].position;
		}

		auto [segment, local_t] = get_segment(t);
		return evaluate_segment(segment, local_t);
	}

	// Evaluate specific segment at local t [0, 1]
	[[nodiscard]] Vector3<T> evaluate_segment(size_t segment, T t) const {
		if (segment >= points.size() - 1) {
			return points.back().position;
		}

		const auto &p0 = points[segment];
		const auto &p1 = points[segment + 1];

		T tt = t * t;
		T ttt = tt * t;

		// Hermite basis functions
		T h00 = T(2) * ttt - T(3) * tt + T(1);
		T h10 = ttt - T(2) * tt + t;
		T h01 = -T(2) * ttt + T(3) * tt;
		T h11 = ttt - tt;

		return p0.position * h00 + p0.tangent * h10 +
				p1.position * h01 + p1.tangent * h11;
	}

	// Evaluate tangent at parameter t
	[[nodiscard]] Vector3<T> tangent(T t) const {
		auto [segment, local_t] = get_segment(t);

		if (segment >= points.size() - 1) {
			return points.back().tangent;
		}

		const auto &p0 = points[segment];
		const auto &p1 = points[segment + 1];

		T tt = local_t * local_t;

		// Derivatives of Hermite basis functions
		T dh00 = T(6) * tt - T(6) * local_t;
		T dh10 = T(3) * tt - T(4) * local_t + T(1);
		T dh01 = -T(6) * tt + T(6) * local_t;
		T dh11 = T(3) * tt - T(2) * local_t;

		return (p0.position * dh00 + p0.tangent * dh10 +
				p1.position * dh01 + p1.tangent * dh11)
				.normalized();
	}

	// Evaluate second derivative at parameter t
	[[nodiscard]] Vector3<T> second_derivative(T t) const {
		auto [segment, local_t] = get_segment(t);

		if (segment >= points.size() - 1) {
			return Vector3<T>::zero();
		}

		const auto &p0 = points[segment];
		const auto &p1 = points[segment + 1];

		T tt = local_t * local_t;

		T ddh00 = T(12) * local_t - T(6);
		T ddh10 = T(6) * local_t - T(4);
		T ddh01 = -T(12) * local_t + T(6);
		T ddh11 = T(6) * local_t - T(2);

		return p0.position * ddh00 + p0.tangent * ddh10 +
				p1.position * ddh01 + p1.tangent * ddh11;
	}

	// Evaluate normal at parameter t
	[[nodiscard]] Vector3<T> normal(T t) const {
		Vector3<T> tan = tangent(t);
		Vector3<T> d2 = second_derivative(t);

		Vector3<T> bin = tan.cross(d2);
		if (bin.length() < Epsilon<T>::value) {
			return Vector3<T>::up();
		}
		return bin.cross(tan).normalized();
	}

	// Evaluate frame at parameter t
	[[nodiscard]] CurveFrame<T> frame(T t) const {
		Vector3<T> pos = evaluate(t);
		Vector3<T> tan = tangent(t);
		Vector3<T> norm = normal(t);
		Vector3<T> bin = tan.cross(norm);

		return CurveFrame<T>(pos, tan, norm, bin);
	}

	// Create from Catmull-Rom spline (automatic tangent calculation)
	[[nodiscard]] static HermiteSpline from_catmull_rom(const std::vector<Vector3<T>> &points, T tension = T(0)) {
		HermiteSpline spline;
		if (points.size() < 2) {
			return spline;
		}

		for (size_t i = 0; i < points.size(); ++i) {
			Vector3<T> tangent;
			if (i == 0) {
				tangent = (points[1] - points[0]) * (T(1) - tension);
			} else if (i == points.size() - 1) {
				tangent = (points[i] - points[i - 1]) * (T(1) - tension);
			} else {
				tangent = (points[i + 1] - points[i - 1]) * (T(0.5) * (T(1) - tension));
			}
			spline.add_point(points[i], tangent);
		}

		return spline;
	}
};

// ============================================================================
// Type Aliases
// ============================================================================

using BezierQuadraticf = BezierQuadratic<float>;
using BezierQuadraticd = BezierQuadratic<double>;
using BezierCubicf = BezierCubic<float>;
using BezierCubicd = BezierCubic<double>;
using CatmullRomSplinef = CatmullRomSpline<float>;
using CatmullRomSplined = CatmullRomSpline<double>;
using HermiteSplinef = HermiteSpline<float>;
using HermiteSplined = HermiteSpline<double>;

} // namespace mml