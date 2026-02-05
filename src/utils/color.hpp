/**************************************************************************/
/*  color.hpp                                                             */
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

#include "../core/config.hpp"
#include "../core/constants.hpp"
#include "../core/epsilon.hpp"
#include <algorithm>
#include <cmath>
#include <type_traits>

namespace mml {

template <typename T>
struct Color {
	static_assert(std::is_floating_point_v<T>, "Color requires floating-point type");

	T r, g, b, a;

	Color() = default;

	explicit constexpr Color(T scalar) :
			r(scalar), g(scalar), b(scalar), a(scalar) {}

	explicit constexpr Color(T r_, T g_, T b_, T a_ = T(1)) :
			r(r_), g(g_), b(b_), a(a_) {}

	constexpr Color(const Color &other) = default;
	Color &operator=(const Color &other) = default;

	static constexpr Color black(T alpha = T(1)) {
		return Color(T(0), T(0), T(0), alpha);
	}

	static constexpr Color white(T alpha = T(1)) {
		return Color(T(1), T(1), T(1), alpha);
	}

	static constexpr Color red(T alpha = T(1)) {
		return Color(T(1), T(0), T(0), alpha);
	}

	static constexpr Color green(T alpha = T(1)) {
		return Color(T(0), T(1), T(0), alpha);
	}

	static constexpr Color blue(T alpha = T(1)) {
		return Color(T(0), T(0), T(1), alpha);
	}

	static constexpr Color yellow(T alpha = T(1)) {
		return Color(T(1), T(1), T(0), alpha);
	}

	static constexpr Color cyan(T alpha = T(1)) {
		return Color(T(0), T(1), T(1), alpha);
	}

	static constexpr Color magenta(T alpha = T(1)) {
		return Color(T(1), T(0), T(1), alpha);
	}

	static constexpr Color clear() {
		return Color(T(0), T(0), T(0), T(0));
	}

	static constexpr Color gray(T value, T alpha = T(1)) {
		return Color(value, value, value, alpha);
	}

	MML_FORCE_INLINE Color operator+(const Color &rhs) const {
		return Color(r + rhs.r, g + rhs.g, b + rhs.b, a + rhs.a);
	}

	MML_FORCE_INLINE Color &operator+=(const Color &rhs) {
		r += rhs.r;
		g += rhs.g;
		b += rhs.b;
		a += rhs.a;
		return *this;
	}

	MML_FORCE_INLINE Color operator-(const Color &rhs) const {
		return Color(r - rhs.r, g - rhs.g, b - rhs.b, a - rhs.a);
	}

	MML_FORCE_INLINE Color &operator-=(const Color &rhs) {
		r -= rhs.r;
		g -= rhs.g;
		b -= rhs.b;
		a -= rhs.a;
		return *this;
	}

	MML_FORCE_INLINE Color operator*(T scalar) const {
		return Color(r * scalar, g * scalar, b * scalar, a * scalar);
	}

	MML_FORCE_INLINE Color &operator*=(T scalar) {
		r *= scalar;
		g *= scalar;
		b *= scalar;
		a *= scalar;
		return *this;
	}

	MML_FORCE_INLINE Color operator*(const Color &rhs) const {
		return Color(r * rhs.r, g * rhs.g, b * rhs.b, a * rhs.a);
	}

	MML_FORCE_INLINE Color operator/(T scalar) const {
		T inv = T(1) / scalar;
		return Color(r * inv, g * inv, b * inv, a * inv);
	}

	MML_FORCE_INLINE Color &operator/=(T scalar) {
		T inv = T(1) / scalar;
		r *= inv;
		g *= inv;
		b *= inv;
		a *= inv;
		return *this;
	}

	MML_FORCE_INLINE Color operator-() const {
		return Color(-r, -g, -b, -a);
	}

	MML_FORCE_INLINE bool operator==(const Color &rhs) const {
		return Epsilon<T>::equal(r, rhs.r) &&
				Epsilon<T>::equal(g, rhs.g) &&
				Epsilon<T>::equal(b, rhs.b) &&
				Epsilon<T>::equal(a, rhs.a);
	}

	MML_FORCE_INLINE bool operator!=(const Color &rhs) const {
		return !(*this == rhs);
	}

	MML_FORCE_INLINE Color lerp(const Color &rhs, T t) const {
		return Color(
				r + (rhs.r - r) * t,
				g + (rhs.g - g) * t,
				b + (rhs.b - b) * t,
				a + (rhs.a - a) * t);
	}

	MML_FORCE_INLINE Color clamp(T min_val = T(0), T max_val = T(1)) const {
		T clamped_r = r < min_val ? min_val : (r > max_val ? max_val : r);
		T clamped_g = g < min_val ? min_val : (g > max_val ? max_val : g);
		T clamped_b = b < min_val ? min_val : (b > max_val ? max_val : b);
		T clamped_a = a < min_val ? min_val : (a > max_val ? max_val : a);
		return Color(clamped_r, clamped_g, clamped_b, clamped_a);
	}

	MML_FORCE_INLINE Color saturated() const {
		return clamp(T(0), T(1));
	}

	MML_FORCE_INLINE Color with_alpha(T new_alpha) const {
		return Color(r, g, b, new_alpha);
	}

	MML_FORCE_INLINE T luminance() const {
		return T(0.2126) * r + T(0.7152) * g + T(0.0722) * b;
	}

	MML_FORCE_INLINE T brightness() const {
		return (r + g + b) * T(1) / T(3);
	}

	static Color from_rgb(unsigned char r_, unsigned char g_, unsigned char b_, unsigned char a_ = 255) {
		return Color(
				static_cast<T>(r_) / T(255),
				static_cast<T>(g_) / T(255),
				static_cast<T>(b_) / T(255),
				static_cast<T>(a_) / T(255));
	}

	void to_rgb(unsigned char &out_r, unsigned char &out_g, unsigned char &out_b) const {
		to_rgb(out_r, out_g, out_b, 255);
	}

	void to_rgb(unsigned char &out_r, unsigned char &out_g, unsigned char &out_b, unsigned char out_a) const {
		out_r = static_cast<unsigned char>(clamp(r, T(0), T(1)) * T(255));
		out_g = static_cast<unsigned char>(clamp(g, T(0), T(1)) * T(255));
		out_b = static_cast<unsigned char>(clamp(b, T(0), T(1)) * T(255));
		out_a = static_cast<unsigned char>(clamp(a, T(0), T(1)) * T(255));
	}

	static Color from_hex(unsigned int hex) {
		T r_ = static_cast<T>((hex >> 24) & 0xFF) / T(255);
		T g_ = static_cast<T>((hex >> 16) & 0xFF) / T(255);
		T b_ = static_cast<T>((hex >> 8) & 0xFF) / T(255);
		T a_ = static_cast<T>(hex & 0xFF) / T(255);
		return Color(r_, g_, b_, a_);
	}

	unsigned int to_hex() const {
		unsigned char r_ = static_cast<unsigned char>(clamp(r, T(0), T(1)) * T(255));
		unsigned char g_ = static_cast<unsigned char>(clamp(g, T(0), T(1)) * T(255));
		unsigned char b_ = static_cast<unsigned char>(clamp(b, T(0), T(1)) * T(255));
		unsigned char a_ = static_cast<unsigned char>(clamp(a, T(0), T(1)) * T(255));
		return (r_ << 24) | (g_ << 16) | (b_ << 8) | a_;
	}

	static Color from_hsv(T h, T s, T v, T a = T(1)) {
		h = h - floor(h / T(360)) * T(360);
		T c = v * s;
		T x = c * (T(1) - std::abs(std::fmod(h / T(60), T(2)) - T(1)));
		T m = v - c;

		T r_, g_, b_;
		if (h < T(60)) {
			r_ = c;
			g_ = x;
			b_ = T(0);
		} else if (h < T(120)) {
			r_ = x;
			g_ = c;
			b_ = T(0);
		} else if (h < T(180)) {
			r_ = T(0);
			g_ = c;
			b_ = x;
		} else if (h < T(240)) {
			r_ = T(0);
			g_ = x;
			b_ = c;
		} else if (h < T(300)) {
			r_ = x;
			g_ = T(0);
			b_ = c;
		} else {
			r_ = c;
			g_ = T(0);
			b_ = x;
		}

		return Color(r_ + m, g_ + m, b_ + m, a);
	}

	void to_hsv(T &out_h, T &out_s, T &out_v, T &out_a) const {
		out_a = a;

		T max_val = r < g ? (g < b ? b : g) : (r < b ? b : r);
		T min_val = r < g ? (r < b ? r : b) : (g < b ? g : b);
		T delta = max_val - min_val;

		out_v = max_val;

		if (delta < T(0.0001)) {
			out_h = T(0);
			out_s = T(0);
			return;
		}

		out_s = delta / max_val;

		if (max_val == r) {
			out_h = T(60) * std::fmod((g - b) / delta, T(6));
		} else if (max_val == g) {
			out_h = T(60) * ((b - r) / delta + T(2));
		} else {
			out_h = T(60) * ((r - g) / delta + T(4));
		}

		if (out_h < T(0)) {
			out_h += T(360);
		}
	}

	static Color from_hsl(T h, T s, T l, T a = T(1)) {
		h = h - floor(h / T(360)) * T(360);
		T c = (T(1) - std::abs(T(2) * l - T(1))) * s;
		T x = c * (T(1) - std::abs(std::fmod(h / T(60), T(2)) - T(1)));
		T m = l - c / T(2);

		T r_, g_, b_;
		if (h < T(60)) {
			r_ = c;
			g_ = x;
			b_ = T(0);
		} else if (h < T(120)) {
			r_ = x;
			g_ = c;
			b_ = T(0);
		} else if (h < T(180)) {
			r_ = T(0);
			g_ = c;
			b_ = x;
		} else if (h < T(240)) {
			r_ = T(0);
			g_ = x;
			b_ = c;
		} else if (h < T(300)) {
			r_ = x;
			g_ = T(0);
			b_ = c;
		} else {
			r_ = c;
			g_ = T(0);
			b_ = x;
		}

		return Color(r_ + m, g_ + m, b_ + m, a);
	}

	void to_hsl(T &out_h, T &out_s, T &out_l, T &out_a) const {
		out_a = a;

		T max_val = r < g ? (g < b ? b : g) : (r < b ? b : r);
		T min_val = r < g ? (r < b ? r : b) : (g < b ? g : b);
		T delta = max_val - min_val;

		out_l = (max_val + min_val) / T(2);

		if (delta < T(0.0001)) {
			out_h = T(0);
			out_s = T(0);
			return;
		}

		out_s = delta / (T(1) - std::abs(T(2) * out_l - T(1)));

		if (max_val == r) {
			out_h = T(60) * std::fmod((g - b) / delta, T(6));
		} else if (max_val == g) {
			out_h = T(60) * ((b - r) / delta + T(2));
		} else {
			out_h = T(60) * ((r - g) / delta + T(4));
		}

		if (out_h < T(0)) {
			out_h += T(360);
		}
	}
};

using Colorf = Color<float>;
using Colord = Color<double>;

} //namespace mml