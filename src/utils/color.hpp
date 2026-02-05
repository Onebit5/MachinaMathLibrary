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
		return Color(T(0), T(0), T(0), alpha); // black color
	}

	static constexpr Color white(T alpha = T(1)) {
		return Color(T(1), T(1), T(1), alpha); // white color
	}

	static constexpr Color red(T alpha = T(1)) {
		return Color(T(1), T(0), T(0), alpha); // red color
	}

	static constexpr Color green(T alpha = T(1)) {
		return Color(T(0), T(1), T(0), alpha); // green color
	}

	static constexpr Color blue(T alpha = T(1)) {
		return Color(T(0), T(0), T(1), alpha); // blue color
	}

	static constexpr Color yellow(T alpha = T(1)) {
		return Color(T(1), T(1), T(0), alpha); // yellow color
	}

	static constexpr Color cyan(T alpha = T(1)) {
		return Color(T(0), T(1), T(1), alpha); // cyan color
	}

	static constexpr Color magenta(T alpha = T(1)) {
		return Color(T(1), T(0), T(1), alpha); // magenta color
	}

	static constexpr Color clear() {
		return Color(T(0), T(0), T(0), T(0)); // transparent color
	}

	static constexpr Color gray(T value, T alpha = T(1)) {
		return Color(value, value, value, alpha); // grayscale color
	}

	MML_FORCE_INLINE Color operator+(const Color &rhs) const {
		return Color(r + rhs.r, g + rhs.g, b + rhs.b, a + rhs.a); // add colors
	}

	MML_FORCE_INLINE Color &operator+=(const Color &rhs) {
		r += rhs.r;
		g += rhs.g;
		b += rhs.b;
		a += rhs.a;
		return *this;
	}

	MML_FORCE_INLINE Color operator-(const Color &rhs) const {
		return Color(r - rhs.r, g - rhs.g, b - rhs.b, a - rhs.a); // subtract colors
	}

	MML_FORCE_INLINE Color &operator-=(const Color &rhs) {
		r -= rhs.r;
		g -= rhs.g;
		b -= rhs.b;
		a -= rhs.a;
		return *this;
	}

	MML_FORCE_INLINE Color operator*(T scalar) const {
		return Color(r * scalar, g * scalar, b * scalar, a * scalar); // scale color
	}

	MML_FORCE_INLINE Color &operator*=(T scalar) {
		r *= scalar;
		g *= scalar;
		b *= scalar;
		a *= scalar;
		return *this;
	}

	MML_FORCE_INLINE Color operator*(const Color &rhs) const {
		return Color(r * rhs.r, g * rhs.g, b * rhs.b, a * rhs.a); // multiply colors
	}

	MML_FORCE_INLINE Color operator/(T scalar) const {
		T inv = T(1) / scalar; // reciprocal
		return Color(r * inv, g * inv, b * inv, a * inv); // divide color
	}

	MML_FORCE_INLINE Color &operator/=(T scalar) {
		T inv = T(1) / scalar; // reciprocal
		r *= inv;
		g *= inv;
		b *= inv;
		a *= inv;
		return *this;
	}

	MML_FORCE_INLINE Color operator-() const {
		return Color(-r, -g, -b, -a); // negate color
	}

	MML_FORCE_INLINE bool operator==(const Color &rhs) const {
		return Epsilon<T>::equal(r, rhs.r) &&
				Epsilon<T>::equal(g, rhs.g) &&
				Epsilon<T>::equal(b, rhs.b) &&
				Epsilon<T>::equal(a, rhs.a); // compare with epsilon
	}

	MML_FORCE_INLINE bool operator!=(const Color &rhs) const {
		return !(*this == rhs); // not equal
	}

	MML_FORCE_INLINE Color lerp(const Color &rhs, T t) const {
		return Color(
				r + (rhs.r - r) * t, // interpolate red
				g + (rhs.g - g) * t, // interpolate green
				b + (rhs.b - b) * t, // interpolate blue
				a + (rhs.a - a) * t); // interpolate alpha
	}

	MML_FORCE_INLINE Color clamp(T min_val = T(0), T max_val = T(1)) const {
		T clamped_r = r < min_val ? min_val : (r > max_val ? max_val : r); // clamp red
		T clamped_g = g < min_val ? min_val : (g > max_val ? max_val : g); // clamp green
		T clamped_b = b < min_val ? min_val : (b > max_val ? max_val : b); // clamp blue
		T clamped_a = a < min_val ? min_val : (a > max_val ? max_val : a); // clamp alpha
		return Color(clamped_r, clamped_g, clamped_b, clamped_a);
	}

	MML_FORCE_INLINE Color saturated() const {
		return clamp(T(0), T(1)); // clamp to [0,1]
	}

	MML_FORCE_INLINE Color with_alpha(T new_alpha) const {
		return Color(r, g, b, new_alpha); // change alpha
	}

	MML_FORCE_INLINE T luminance() const {
		return T(0.2126) * r + T(0.7152) * g + T(0.0722) * b; // Rec. 709 luma
	}

	MML_FORCE_INLINE T brightness() const {
		return (r + g + b) * T(1) / T(3); // average brightness
	}

	static Color from_rgb(unsigned char r_, unsigned char g_, unsigned char b_, unsigned char a_ = 255) {
		return Color(
				static_cast<T>(r_) / T(255), // normalize red
				static_cast<T>(g_) / T(255), // normalize green
				static_cast<T>(b_) / T(255), // normalize blue
				static_cast<T>(a_) / T(255)); // normalize alpha
	}

	void to_rgb(unsigned char &out_r, unsigned char &out_g, unsigned char &out_b) const {
		to_rgb(out_r, out_g, out_b, 255); // convert to 8-bit RGB
	}

	void to_rgb(unsigned char &out_r, unsigned char &out_g, unsigned char &out_b, unsigned char out_a) const {
		out_r = static_cast<unsigned char>(clamp(r, T(0), T(1)) * T(255)); // red to 8-bit
		out_g = static_cast<unsigned char>(clamp(g, T(0), T(1)) * T(255)); // green to 8-bit
		out_b = static_cast<unsigned char>(clamp(b, T(0), T(1)) * T(255)); // blue to 8-bit
		out_a = static_cast<unsigned char>(clamp(a, T(0), T(1)) * T(255)); // alpha to 8-bit
	}

	static Color from_hex(unsigned int hex) {
		T r_ = static_cast<T>((hex >> 24) & 0xFF) / T(255); // extract red
		T g_ = static_cast<T>((hex >> 16) & 0xFF) / T(255); // extract green
		T b_ = static_cast<T>((hex >> 8) & 0xFF) / T(255); // extract blue
		T a_ = static_cast<T>(hex & 0xFF) / T(255); // extract alpha
		return Color(r_, g_, b_, a_);
	}

	unsigned int to_hex() const {
		unsigned char r_ = static_cast<unsigned char>(clamp(r, T(0), T(1)) * T(255)); // red to 8-bit
		unsigned char g_ = static_cast<unsigned char>(clamp(g, T(0), T(1)) * T(255)); // green to 8-bit
		unsigned char b_ = static_cast<unsigned char>(clamp(b, T(0), T(1)) * T(255)); // blue to 8-bit
		unsigned char a_ = static_cast<unsigned char>(clamp(a, T(0), T(1)) * T(255)); // alpha to 8-bit
		return (r_ << 24) | (g_ << 16) | (b_ << 8) | a_; // pack into hex
	}

	static Color from_hsv(T h, T s, T v, T a = T(1)) {
		h = h - floor(h / T(360)) * T(360); // wrap hue
		T c = v * s; // chroma
		T x = c * (T(1) - std::abs(std::fmod(h / T(60), T(2)) - T(1))); // intermediate value
		T m = v - c; // match value

		T r_, g_, b_;
		if (h < T(60)) {
			r_ = c; // red dominant
			g_ = x;
			b_ = T(0);
		} else if (h < T(120)) {
			r_ = x;
			g_ = c; // green dominant
			b_ = T(0);
		} else if (h < T(180)) {
			r_ = T(0);
			g_ = c;
			b_ = x; // blue dominant
		} else if (h < T(240)) {
			r_ = T(0);
			g_ = x;
			b_ = c; // blue dominant
		} else if (h < T(300)) {
			r_ = x;
			g_ = T(0);
			b_ = c; // magenta dominant
		} else {
			r_ = c; // red dominant
			g_ = T(0);
			b_ = x;
		}

		return Color(r_ + m, g_ + m, b_ + m, a); // add match value
	}

	void to_hsv(T &out_h, T &out_s, T &out_v, T &out_a) const {
		out_a = a; // preserve alpha

		T max_val = r < g ? (g < b ? b : g) : (r < b ? b : r); // max component
		T min_val = r < g ? (r < b ? r : b) : (g < b ? g : b); // min component
		T delta = max_val - min_val; // color range

		out_v = max_val; // value = max component

		if (delta < T(0.0001)) {
			out_h = T(0); // achromatic
			out_s = T(0);
			return;
		}

		out_s = delta / max_val; // saturation = range / max

		if (max_val == r) {
			out_h = T(60) * std::fmod((g - b) / delta, T(6)); // hue from red
		} else if (max_val == g) {
			out_h = T(60) * ((b - r) / delta + T(2)); // hue from green
		} else {
			out_h = T(60) * ((r - g) / delta + T(4)); // hue from blue
		}

		if (out_h < T(0)) {
			out_h += T(360); // ensure positive hue
		}
	}

	static Color from_hsl(T h, T s, T l, T a = T(1)) {
		h = h - floor(h / T(360)) * T(360); // wrap hue
		T c = (T(1) - std::abs(T(2) * l - T(1))) * s; // chroma
		T x = c * (T(1) - std::abs(std::fmod(h / T(60), T(2)) - T(1))); // intermediate value
		T m = l - c / T(2); // match value

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

		return Color(r_ + m, g_ + m, b_ + m, a); // add match value
	}

	void to_hsl(T &out_h, T &out_s, T &out_l, T &out_a) const {
		out_a = a; // preserve alpha

		T max_val = r < g ? (g < b ? b : g) : (r < b ? b : r); // max component
		T min_val = r < g ? (r < b ? r : b) : (g < b ? g : b); // min component
		T delta = max_val - min_val; // color range

		out_l = (max_val + min_val) / T(2); // lightness = average of max and min

		if (delta < T(0.0001)) {
			out_h = T(0); // achromatic
			out_s = T(0);
			return;
		}

		out_s = delta / (T(1) - std::abs(T(2) * out_l - T(1))); // saturation from lightness

		if (max_val == r) {
			out_h = T(60) * std::fmod((g - b) / delta, T(6)); // hue from red
		} else if (max_val == g) {
			out_h = T(60) * ((b - r) / delta + T(2)); // hue from green
		} else {
			out_h = T(60) * ((r - g) / delta + T(4)); // hue from blue
		}

		if (out_h < T(0)) {
			out_h += T(360); // ensure positive hue
		}
	}
};

using Colorf = Color<float>;
using Colord = Color<double>;

} // namespace mml