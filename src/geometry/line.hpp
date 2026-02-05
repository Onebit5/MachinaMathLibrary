/**************************************************************************/
/*  line.hpp                                                              */
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
#include "../core/epsilon.hpp"
#include "../utils/math.hpp"
#include <cmath>
#include <type_traits>

namespace mml {

template <typename T>
struct Line2D {
	static_assert(std::is_floating_point_v<T>, "Line2D requires floating-point type");

	Vector2<T> start;
	Vector2<T> end;

	Line2D() = default;

	explicit Line2D(const Vector2<T> &start_, const Vector2<T> &end_) :
			start(start_), end(end_) {}

	constexpr Line2D(const Line2D &other) = default;
	Line2D &operator=(const Line2D &other) = default;

	MML_FORCE_INLINE Vector2<T> direction() const {
		return (end - start).normalized();
	}

	MML_FORCE_INLINE T length() const {
		return start.distance(end);
	}

	MML_FORCE_INLINE T length_squared() const {
		return start.distance_squared(end);
	}

	MML_FORCE_INLINE Vector2<T> lerp(T t) const {
		return start.lerp(end, t);
	}

	MML_FORCE_INLINE Vector2<T> closest_point(const Vector2<T> &point) const {
		Vector2<T> ab = end - start;
		Vector2<T> ap = point - start;

		T t = ap.dot(ab) / ab.length_squared();
		t = t < T(0) ? T(0) : (t > T(1) ? T(1) : t);

		return start + ab * t;
	}

	MML_FORCE_INLINE T distance_squared(const Vector2<T> &point) const {
		Vector2<T> closest = closest_point(point);
		return (point - closest).length_squared();
	}

	MML_FORCE_INLINE T distance(const Vector2<T> &point) const {
		return Math<T>::safe_sqrt(distance_squared(point));
	}

	MML_FORCE_INLINE bool intersects(const Line2D &other) const {
		Vector2<T> a = start;
		Vector2<T> b = end;
		Vector2<T> c = other.start;
		Vector2<T> d = other.end;

		T denominator = (a.x - b.x) * (c.y - d.y) - (a.y - b.y) * (c.x - d.x);

		if (Epsilon<T>::approx_zero(denominator)) {
			return false;
		}

		T t = ((a.x - c.x) * (c.y - d.y) - (a.y - c.y) * (c.x - d.x)) / denominator;
		T u = -((a.x - b.x) * (a.y - c.y) - (a.y - b.y) * (a.x - c.x)) / denominator;

		return t >= T(0) && t <= T(1) && u >= T(0) && u <= T(1);
	}
};

template <typename T>
struct Line3D {
	static_assert(std::is_floating_point_v<T>, "Line3D requires floating-point type");

	Vector3<T> start;
	Vector3<T> end;

	Line3D() = default;

	explicit Line3D(const Vector3<T> &start_, const Vector3<T> &end_) :
			start(start_), end(end_) {}

	constexpr Line3D(const Line3D &other) = default;
	Line3D &operator=(const Line3D &other) = default;

	MML_FORCE_INLINE Vector3<T> direction() const {
		return (end - start).normalized();
	}

	MML_FORCE_INLINE T length() const {
		return start.distance(end);
	}

	MML_FORCE_INLINE T length_squared() const {
		return start.distance_squared(end);
	}

	MML_FORCE_INLINE Vector3<T> lerp(T t) const {
		return start.lerp(end, t);
	}

	MML_FORCE_INLINE Vector3<T> closest_point(const Vector3<T> &point) const {
		Vector3<T> ab = end - start;
		Vector3<T> ap = point - start;

		T t = ap.dot(ab) / ab.length_squared();
		t = t < T(0) ? T(0) : (t > T(1) ? T(1) : t);

		return start + ab * t;
	}

	MML_FORCE_INLINE T distance_squared(const Vector3<T> &point) const {
		Vector3<T> closest = closest_point(point);
		return (point - closest).length_squared();
	}

	MML_FORCE_INLINE T distance(const Vector3<T> &point) const {
		return Math<T>::safe_sqrt(distance_squared(point));
	}

	MML_FORCE_INLINE T distance_squared(const Line3D &other) const {
		Vector3<T> u = end - start;
		Vector3<T> v = other.end - other.start;
		Vector3<T> w = start - other.start;

		T a = u.dot(u);
		T b = u.dot(v);
		T c = v.dot(v);
		T d = u.dot(w);
		T e = v.dot(w);
		T D = a * c - b * b;
		T sc, sN, sD = D;
		T tc, tN, tD = D;

		if (Epsilon<T>::approx_zero(D)) {
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

		sc = Epsilon<T>::approx_zero(std::abs(sN)) ? T(0) : sN / sD;
		tc = Epsilon<T>::approx_zero(std::abs(tN)) ? T(0) : tN / tD;

		Vector3<T> dP = w + (u * sc) - (v * tc);
		return dP.length_squared();
	}

	MML_FORCE_INLINE T distance(const Line3D &other) const {
		return Math<T>::safe_sqrt(distance_squared(other));
	}
};

using Line2Df = Line2D<float>;
using Line2Dd = Line2D<double>;
using Line3Df = Line3D<float>;
using Line3Dd = Line3D<double>;

} //namespace mml