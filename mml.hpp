/**************************************************************************/
/*  mml.hpp — Machina Math Library (single-header)                      */
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
/* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE      */
/* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                 */
/**************************************************************************/
#ifndef MML_HPP
#define MML_HPP

#include <cmath>
#include <cstddef>
#include <cstdint>
#include <type_traits>
#include <algorithm>
#include <array>
#include <memory>
#include <vector>
#include <limits>
#include <ostream>
#include <istream>
#include <functional>
#include <numeric>
#include <random>

namespace mml {


#ifndef MML_USE_SIMD
#define MML_USE_SIMD 0
#endif

#if MML_USE_SIMD
#if defined(__AVX2__)
#define MML_SIMD_AVX2 1
#define MML_SIMD_AVX 1
#define MML_SIMD_SSE4_1 1
#define MML_SIMD_SSE3 1
#define MML_SIMD_SSE2 1
#define MML_SIMD_SSE 1
#elif defined(__AVX__)
#define MML_SIMD_AVX 1
#define MML_SIMD_SSE4_1 1
#define MML_SIMD_SSE3 1
#define MML_SIMD_SSE2 1
#define MML_SIMD_SSE 1
#elif defined(__SSE4_1__)
#define MML_SIMD_SSE4_1 1
#define MML_SIMD_SSE3 1
#define MML_SIMD_SSE2 1
#define MML_SIMD_SSE 1
#elif defined(__SSE3__)
#define MML_SIMD_SSE3 1
#define MML_SIMD_SSE2 1
#define MML_SIMD_SSE 1
#elif defined(__SSE2__)
#define MML_SIMD_SSE2 1
#define MML_SIMD_SSE 1
#elif defined(_M_X64) || defined(__x86_64__)
#define MML_SIMD_SSE2 1
#define MML_SIMD_SSE 1
#elif defined(_M_IX86) || defined(__i386__)
#define MML_SIMD_SSE 1
#elif defined(__ARM_NEON) || defined(__aarch64__)
#define MML_SIMD_NEON 1
#endif
#endif

#ifndef MML_SIMD_SSE
#define MML_SIMD_SSE 0
#endif
#ifndef MML_SIMD_SSE2
#define MML_SIMD_SSE2 0
#endif
#ifndef MML_SIMD_SSE3
#define MML_SIMD_SSE3 0
#endif
#ifndef MML_SIMD_SSE4_1
#define MML_SIMD_SSE4_1 0
#endif
#ifndef MML_SIMD_AVX
#define MML_SIMD_AVX 0
#endif
#ifndef MML_SIMD_AVX2
#define MML_SIMD_AVX2 0
#endif
#ifndef MML_SIMD_NEON
#define MML_SIMD_NEON 0
#endif

#if defined(_MSC_VER)
#define MML_FORCE_INLINE __forceinline
#else
#define MML_FORCE_INLINE inline __attribute__((always_inline))
#endif

#if defined(_MSC_VER) && _MSC_VER >= 1910
#pragma warning(disable : 4514 4179)
#pragma warning(disable : 4457)
#endif

#ifndef MML_EPSILON
#define MML_EPSILON 1e-6f
#endif




template <typename T>
struct Constants {
	static constexpr T pi =
			T(3.14159265358979323846264338327950288L);

	static constexpr T two_pi = T(2) * pi;
	static constexpr T half_pi = pi / T(2);
	static constexpr T quarter_pi = pi / T(4);
	static constexpr T inv_pi = T(1) / pi;

	static constexpr T e =
			T(2.71828182845904523536028747135266250L);

	static constexpr T sqrt_2 =
			T(1.41421356237309504880168872420969808L);

	static constexpr T sqrt_3 =
			T(1.73205080756887729352744634150587236L);

	static constexpr T inv_sqrt_2 = T(1) / sqrt_2;
	static constexpr T inv_sqrt_3 = T(1) / sqrt_3;

	static constexpr T golden_ratio = T(1.61803398874989484820458683436563811L);

	static constexpr T inv_golden_ratio = T(0.61803398874989484820458683436563811L);

	static constexpr T ln_2 = T(0.69314718055994530941723212145817656L);

	static constexpr T ln_10 = T(2.30258509299404568401799145468436L);

	static constexpr T log10_2 = T(0.30102999566398119521373889472449302L);

	static constexpr T log2_e = T(1.44269504088896340735992468100189213L);

	static constexpr T rad_to_deg = T(180) / pi;

	static constexpr T deg_to_rad = pi / T(180);

	static constexpr T infinity =
			std::numeric_limits<T>::infinity();
};

using Constantsf = Constants<float>;
using Constantsd = Constants<double>;






template <typename T>
struct Epsilon {
	static_assert(std::is_floating_point_v<T>, "Epsilon only supports floating-point types");

	static constexpr T value = T(MML_EPSILON);

	static constexpr T high = std::is_same_v<T, float> ? T(1e-6f) : T(1e-12);

	static constexpr T medium = std::is_same_v<T, float> ? T(1e-4f) : T(1e-8);

	static constexpr T low = std::is_same_v<T, float> ? T(1e-2f) : T(1e-4);

	static constexpr T ultra = std::is_same_v<T, float> ? T(1e-7f) : T(1e-14);

	static constexpr T machine = std::numeric_limits<T>::epsilon();

	MML_FORCE_INLINE static bool equal(T a, T b, T eps = value) {
		return std::abs(a - b) <= eps;
	}

	MML_FORCE_INLINE static bool equal_high(T a, T b) {
		return equal(a, b, high);
	}

	MML_FORCE_INLINE static bool equal_medium(T a, T b) {
		return equal(a, b, medium);
	}

	MML_FORCE_INLINE static bool equal_low(T a, T b) {
		return equal(a, b, low);
	}

	MML_FORCE_INLINE static bool is_zero(T a, T eps = value) {
		return std::abs(a) <= eps;
	}

	MML_FORCE_INLINE static bool is_zero_high(T a) {
		return is_zero(a, high);
	}

	MML_FORCE_INLINE static bool is_zero_medium(T a) {
		return is_zero(a, medium);
	}

	MML_FORCE_INLINE static bool is_zero_low(T a) {
		return is_zero(a, low);
	}

	MML_FORCE_INLINE static bool less(T a, T b, T eps = value) {
		return (b - a) > eps;
	}

	MML_FORCE_INLINE static bool greater(T a, T b, T eps = value) {
		return (a - b) > eps;
	}

	MML_FORCE_INLINE static bool less_equal(T a, T b, T eps = value) {
		return !greater(a, b, eps);
	}

	MML_FORCE_INLINE static bool greater_equal(T a, T b, T eps = value) {
		return !less(a, b, eps);
	}

	MML_FORCE_INLINE static bool nearly_equal(T a, T b, T relative_eps = value) {
		T diff = std::abs(a - b);
		T abs_a = std::abs(a);
		T abs_b = std::abs(b);
		T max_abs = (abs_a > abs_b) ? abs_a : abs_b;
		T scale = (T(1) > max_abs) ? T(1) : max_abs;
		return diff <= scale * relative_eps;
	}

	MML_FORCE_INLINE static bool nearly_equal_abs(T a, T b, T absolute_eps = value) {
		return std::abs(a - b) <= absolute_eps;
	}

	MML_FORCE_INLINE static bool is_within(T val, T target, T tolerance = value) {
		return std::abs(val - target) <= tolerance;
	}

	MML_FORCE_INLINE static bool approx_equal(T a, T b, T relative_eps = value) {
		return nearly_equal(a, b, relative_eps);
	}

	MML_FORCE_INLINE static bool approx_zero(T a, T relative_eps = value) {
		T abs_a = std::abs(a);
		T scale = (T(1) > abs_a) ? T(1) : abs_a;
		return abs_a <= scale * relative_eps;
	}

	MML_FORCE_INLINE static bool approx_less_equal(T a, T b, T relative_eps = value) {
		return (a - b) <= relative_eps * (std::abs(a) > T(1) ? std::abs(a) : T(1));
	}

	MML_FORCE_INLINE static bool approx_greater_equal(T a, T b, T relative_eps = value) {
		return (b - a) <= relative_eps * (std::abs(b) > T(1) ? std::abs(b) : T(1));
	}
};

using Epsilonf = Epsilon<float>;
using Epsilond = Epsilon<double>;






template <typename T>
struct Math {
	static_assert(std::is_arithmetic_v<T>, "Math requires arithmetic type");

	MML_FORCE_INLINE static constexpr T abs(T value) {
		return value < T(0) ? -value : value;
	}

	MML_FORCE_INLINE static constexpr T min(T a, T b) {
		return a < b ? a : b;
	}

	MML_FORCE_INLINE static constexpr T max(T a, T b) {
		return a > b ? a : b;
	}

	MML_FORCE_INLINE static constexpr T clamp(T value, T min_val, T max_val) {
		return value < min_val ? min_val : (value > max_val ? max_val : value);
	}

	MML_FORCE_INLINE static constexpr T lerp(T a, T b, T t) {
		return a + (b - a) * t;
	}

	MML_FORCE_INLINE static constexpr T inverse_lerp(T a, T b, T value) {
		if (a == b) {
			return T(0);
		}
		return (value - a) / (b - a);
	}

	MML_FORCE_INLINE static constexpr T remap(T value, T in_min, T in_max, T out_min, T out_max) {
		T t = inverse_lerp(in_min, in_max, value);
		return lerp(out_min, out_max, t);
	}

	MML_FORCE_INLINE static T sqrt(T value) {
		return std::sqrt(value);
	}

	MML_FORCE_INLINE static T safe_sqrt(T value) {
		if (value < T(0)) {
			return T(0);
		}
		return std::sqrt(value);
	}

	MML_FORCE_INLINE static T sqrt_fast(T value) {
		return std::sqrt(value);
	}

	MML_FORCE_INLINE static T inv_sqrt(T value) {
		return T(1) / std::sqrt(value);
	}

	MML_FORCE_INLINE static T pow(T base, T exp) {
		return std::pow(base, exp);
	}

	MML_FORCE_INLINE static T exp(T value) {
		return std::exp(value);
	}

	MML_FORCE_INLINE static T log(T value) {
		return std::log(value);
	}

	MML_FORCE_INLINE static T log2(T value) {
		return std::log2(value);
	}

	MML_FORCE_INLINE static T log10(T value) {
		return std::log10(value);
	}

	MML_FORCE_INLINE static T sin(T radians) {
		return std::sin(radians);
	}

	MML_FORCE_INLINE static T cos(T radians) {
		return std::cos(radians);
	}

	MML_FORCE_INLINE static T tan(T radians) {
		return std::tan(radians);
	}

	MML_FORCE_INLINE static T asin(T value) {
		return std::asin(value);
	}

	MML_FORCE_INLINE static T acos(T value) {
		return std::acos(value);
	}

	MML_FORCE_INLINE static T safe_acos(T value) {
		T clamped = value > T(1) ? T(1) : (value < T(-1) ? T(-1) : value);
		return std::acos(clamped);
	}

	MML_FORCE_INLINE static T safe_asin(T value) {
		T clamped = value > T(1) ? T(1) : (value < T(-1) ? T(-1) : value);
		return std::asin(clamped);
	}

	MML_FORCE_INLINE static T atan(T value) {
		return std::atan(value);
	}

	MML_FORCE_INLINE static T atan2(T y, T x) {
		return std::atan2(y, x);
	}

	MML_FORCE_INLINE static T sinh(T value) {
		return std::sinh(value);
	}

	MML_FORCE_INLINE static T cosh(T value) {
		return std::cosh(value);
	}

	MML_FORCE_INLINE static T tanh(T value) {
		return std::tanh(value);
	}

	MML_FORCE_INLINE static T floor(T value) {
		return std::floor(value);
	}

	MML_FORCE_INLINE static T ceil(T value) {
		return std::ceil(value);
	}

	MML_FORCE_INLINE static T round(T value) {
		return std::round(value);
	}

	MML_FORCE_INLINE static T trunc(T value) {
		return std::trunc(value);
	}

	MML_FORCE_INLINE static T frac(T value) {
		return value - std::floor(value);
	}

	MML_FORCE_INLINE static T fmod(T x, T y) {
		return std::fmod(x, y);
	}

	MML_FORCE_INLINE static T sign(T value) {
		return value > T(0) ? T(1) : (value < T(0) ? T(-1) : T(0));
	}

	MML_FORCE_INLINE static T step(T edge, T value) {
		return value < edge ? T(0) : T(1);
	}

	MML_FORCE_INLINE static T smoothstep(T edge0, T edge1, T x) {
		T t = clamp((x - edge0) / (edge1 - edge0), T(0), T(1));
		return t * t * (T(3) - T(2) * t);
	}

	MML_FORCE_INLINE static T mix(T x, T y, T a) {
		return x * (T(1) - a) + y * a;
	}

	MML_FORCE_INLINE static constexpr T degrees_to_radians(T degrees) {
		return degrees * Constants<T>::pi / T(180);
	}

	MML_FORCE_INLINE static constexpr T radians_to_degrees(T radians) {
		return radians * T(180) / Constants<T>::pi;
	}

	MML_FORCE_INLINE static T normalize_angle(T radians) {
		T result = fmod(radians, Constants<T>::two_pi);
		if (result < T(0)) {
			result += Constants<T>::two_pi;
		}
		return result;
	}

	MML_FORCE_INLINE static T shortest_angle_distance(T from, T to) {
		T diff = normalize_angle(to - from);
		if (diff > Constants<T>::pi) {
			diff -= Constants<T>::two_pi;
		}
		return diff;
	}

	MML_FORCE_INLINE static T lerp_angle(T from, T to, T t) {
		return normalize_angle(from + shortest_angle_distance(from, to) * t);
	}

	MML_FORCE_INLINE static constexpr bool is_even(T value) {
		return (static_cast<long long>(value) & 1LL) == 0;
	}

	MML_FORCE_INLINE static constexpr bool is_odd(T value) {
		return !is_even(value);
	}

	MML_FORCE_INLINE static constexpr T repeat(T value, T length) {
		return value - floor(value / length) * length;
	}

	MML_FORCE_INLINE static constexpr T ping_pong(T value, T length) {
		T t = repeat(value, length * T(2));
		return length - abs(t - length);
	}

	MML_FORCE_INLINE static T damp(T current, T target, T smoothing, T dt) {
		return lerp(current, target, T(1) - pow(smoothing, dt));
	}

	MML_FORCE_INLINE static T damp_angle(T current, T target, T smoothing, T dt) {
		return lerp_angle(current, target, T(1) - pow(smoothing, dt));
	}

	MML_FORCE_INLINE static T map_range(T value, T in_min, T in_max, T out_min, T out_max) {
		return out_min + (out_max - out_min) * ((value - in_min) / (in_max - in_min));
	}

	MML_FORCE_INLINE static T approach(T current, T target, T max_delta) {
		T diff = target - current;
		return (abs(diff) <= max_delta) ? target : (current + sign(diff) * max_delta);
	}

	MML_FORCE_INLINE static T move_towards(T current, T target, T max_delta) {
		T diff = target - current;
		if (abs(diff) <= max_delta) {
			return target;
		}
		return current + sign(diff) * max_delta;
	}

	MML_FORCE_INLINE static T wrap(T value, T min_val, T max_val) {
		T range = max_val - min_val;
		if (range == T(0)) {
			return min_val;
		}
		return min_val + fmod(value - min_val, range + range);
	}

	MML_FORCE_INLINE static bool approximately(T a, T b, T epsilon = T(0.0001)) {
		return abs(a - b) < epsilon;
	}

	MML_FORCE_INLINE static bool is_nan(T value) {
		if constexpr (std::is_floating_point_v<T>) {
			return value != value;
		}
		return false;
	}

	MML_FORCE_INLINE static bool is_inf(T value) {
		if constexpr (std::is_floating_point_v<T>) {
			return value > Constants<T>::infinity || value < -Constants<T>::infinity;
		}
		return false;
	}

	MML_FORCE_INLINE static bool is_finite(T value) {
		if constexpr (std::is_floating_point_v<T>) {
			return !is_nan(value) && !is_inf(value);
		}
		return true;
	}
};

using Mathf = Math<float>;
using Mathd = Math<double>;
using Mathi = Math<int>;






template <typename T>
struct Vector3 {
	static_assert(std::is_floating_point_v<T>, "Vector3 requires floating-point type");

	T x, y, z;

	Vector3() = default;

	explicit constexpr Vector3(T scalar) :
			x(scalar), y(scalar), z(scalar) {}

	explicit constexpr Vector3(T x_, T y_, T z_) :
			x(x_), y(y_), z(z_) {}

	constexpr Vector3(const Vector3 &other) = default;
	Vector3 &operator=(const Vector3 &other) = default;

	MML_FORCE_INLINE T &operator[](size_t i) { return (&x)[i]; }
	MML_FORCE_INLINE const T &operator[](size_t i) const { return (&x)[i]; }

	[[nodiscard]] constexpr MML_FORCE_INLINE Vector3 operator+(const Vector3 &rhs) const {
		return Vector3(x + rhs.x, y + rhs.y, z + rhs.z);
	}

	constexpr MML_FORCE_INLINE Vector3 &operator+=(const Vector3 &rhs) {
		x += rhs.x;
		y += rhs.y;
		z += rhs.z;
		return *this;
	}

	[[nodiscard]] constexpr MML_FORCE_INLINE Vector3 operator-(const Vector3 &rhs) const {
		return Vector3(x - rhs.x, y - rhs.y, z - rhs.z);
	}

	constexpr MML_FORCE_INLINE Vector3 &operator-=(const Vector3 &rhs) {
		x -= rhs.x;
		y -= rhs.y;
		z -= rhs.z;
		return *this;
	}

	[[nodiscard]] constexpr MML_FORCE_INLINE Vector3 operator*(T scalar) const {
		return Vector3(x * scalar, y * scalar, z * scalar);
	}

	constexpr MML_FORCE_INLINE Vector3 &operator*=(T scalar) {
		x *= scalar;
		y *= scalar;
		z *= scalar;
		return *this;
	}

	[[nodiscard]] constexpr MML_FORCE_INLINE Vector3 operator/(T scalar) const {
		T inv = T(1) / scalar;
		return Vector3(x * inv, y * inv, z * inv);
	}

	constexpr MML_FORCE_INLINE Vector3 &operator/=(T scalar) {
		T inv = T(1) / scalar;
		x *= inv;
		y *= inv;
		z *= inv;
		return *this;
	}

	[[nodiscard]] constexpr MML_FORCE_INLINE Vector3 operator-() const {
		return Vector3(-x, -y, -z);
	}

	[[nodiscard]] constexpr MML_FORCE_INLINE bool operator==(const Vector3 &rhs) const {
		return Epsilon<T>::equal(x, rhs.x) &&
				Epsilon<T>::equal(y, rhs.y) &&
				Epsilon<T>::equal(z, rhs.z);
	}

	[[nodiscard]] constexpr MML_FORCE_INLINE bool operator!=(const Vector3 &rhs) const {
		return !(*this == rhs);
	}

	[[nodiscard]] constexpr MML_FORCE_INLINE T dot(const Vector3 &rhs) const {
		return x * rhs.x + y * rhs.y + z * rhs.z;
	}

	[[nodiscard]] constexpr MML_FORCE_INLINE Vector3 cross(const Vector3 &rhs) const {
		return Vector3(
				y * rhs.z - z * rhs.y,
				z * rhs.x - x * rhs.z,
				x * rhs.y - y * rhs.x);
	}

	[[nodiscard]] constexpr MML_FORCE_INLINE T length_squared() const {
		return x * x + y * y + z * z;
	}

	[[nodiscard]] MML_FORCE_INLINE T length() const {
		return std::sqrt(length_squared());
	}

	[[nodiscard]] MML_FORCE_INLINE Vector3 normalized() const {
		T len = length();
		if (Epsilon<T>::approx_zero(len)) {
			return Vector3(T(0), T(0), T(0));
		}
		return *this / len;
	}

	MML_FORCE_INLINE Vector3 &normalize() {
		T len = length();
		if (!Epsilon<T>::approx_zero(len)) {
			*this /= len;
		}
		return *this;
	}

	[[nodiscard]] MML_FORCE_INLINE bool is_zero() const {
		return Epsilon<T>::approx_zero(length_squared());
	}

	[[nodiscard]] MML_FORCE_INLINE bool is_unit() const {
		return Epsilon<T>::equal(length_squared(), T(1));
	}

	[[nodiscard]] MML_FORCE_INLINE bool is_finite() const {
		return std::isfinite(x) && std::isfinite(y) && std::isfinite(z);
	}

	[[nodiscard]] MML_FORCE_INLINE T distance(const Vector3 &rhs) const {
		return (*this - rhs).length();
	}

	[[nodiscard]] constexpr MML_FORCE_INLINE T distance_squared(const Vector3 &rhs) const {
		return (*this - rhs).length_squared();
	}

	[[nodiscard]] constexpr MML_FORCE_INLINE Vector3 reflect(const Vector3 &normal) const {
		return *this - normal * (T(2) * this->dot(normal));
	}

	[[nodiscard]] MML_FORCE_INLINE Vector3 refract(const Vector3 &normal, T eta) const {
		T dot_n = this->dot(normal);
		T k = T(1) - eta * eta * (T(1) - dot_n * dot_n);
		if (k < T(0)) {
			return Vector3(T(0), T(0), T(0));
		}
		return *this * eta - normal * (eta * dot_n + std::sqrt(k));
	}

	[[nodiscard]] constexpr MML_FORCE_INLINE Vector3 project(const Vector3 &onto) const {
		T onto_len_sq = onto.length_squared();
		if (Epsilon<T>::approx_zero(onto_len_sq)) {
			return Vector3(T(0), T(0), T(0));
		}
		return onto * (this->dot(onto) / onto_len_sq);
	}

	[[nodiscard]] constexpr MML_FORCE_INLINE Vector3 hadamard(const Vector3 &rhs) const {
		return Vector3(x * rhs.x, y * rhs.y, z * rhs.z);
	}

	[[nodiscard]] constexpr MML_FORCE_INLINE Vector3 hadamard_div(const Vector3 &rhs) const {
		return Vector3(x / rhs.x, y / rhs.y, z / rhs.z);
	}

	[[nodiscard]] MML_FORCE_INLINE T angle(const Vector3 &rhs) const {
		T len_sq = length_squared() * rhs.length_squared();
		if (Epsilon<T>::approx_zero(len_sq)) {
			return T(0);
		}
		T dot_val = dot(rhs);
		T value = dot_val / Math<T>::safe_sqrt(len_sq);
		return Math<T>::safe_acos(value);
	}

	[[nodiscard]] constexpr MML_FORCE_INLINE Vector3 lerp(const Vector3 &rhs, T t) const {
		return Vector3(
				x + (rhs.x - x) * t,
				y + (rhs.y - y) * t,
				z + (rhs.z - z) * t);
	}

	[[nodiscard]] static constexpr MML_FORCE_INLINE T triple_scalar(const Vector3 &a, const Vector3 &b, const Vector3 &c) {
		return a.dot(b.cross(c));
	}

	[[nodiscard]] static constexpr MML_FORCE_INLINE Vector3 triple_vector(const Vector3 &a, const Vector3 &b, const Vector3 &c) {
		return b * a.dot(c) - c * a.dot(b);
	}

	[[nodiscard]] constexpr MML_FORCE_INLINE Vector3 min(const Vector3 &rhs) const {
		return Vector3(x < rhs.x ? x : rhs.x, y < rhs.y ? y : rhs.y, z < rhs.z ? z : rhs.z);
	}

	[[nodiscard]] constexpr MML_FORCE_INLINE Vector3 max(const Vector3 &rhs) const {
		return Vector3(x > rhs.x ? x : rhs.x, y > rhs.y ? y : rhs.y, z > rhs.z ? z : rhs.z);
	}

	[[nodiscard]] constexpr MML_FORCE_INLINE Vector3 clamp(T min_val, T max_val) const {
		T clamped_x = x < min_val ? min_val : (x > max_val ? max_val : x);
		T clamped_y = y < min_val ? min_val : (y > max_val ? max_val : y);
		T clamped_z = z < min_val ? min_val : (z > max_val ? max_val : z);
		return Vector3(clamped_x, clamped_y, clamped_z);
	}

	[[nodiscard]] constexpr MML_FORCE_INLINE Vector3 clamp(const Vector3 &min_vec, const Vector3 &max_vec) const {
		T clamped_x = x < min_vec.x ? min_vec.x : (x > max_vec.x ? max_vec.x : x);
		T clamped_y = y < min_vec.y ? min_vec.y : (y > max_vec.y ? max_vec.y : y);
		T clamped_z = z < min_vec.z ? min_vec.z : (z > max_vec.z ? max_vec.z : z);
		return Vector3(clamped_x, clamped_y, clamped_z);
	}

	[[nodiscard]] constexpr MML_FORCE_INLINE Vector3 abs() const {
		return Vector3(x < T(0) ? -x : x, y < T(0) ? -y : y, z < T(0) ? -z : z);
	}

	[[nodiscard]] constexpr MML_FORCE_INLINE Vector3 sign() const {
		return Vector3(
				x > T(0) ? T(1) : (x < T(0) ? T(-1) : T(0)),
				y > T(0) ? T(1) : (y < T(0) ? T(-1) : T(0)),
				z > T(0) ? T(1) : (z < T(0) ? T(-1) : T(0)));
	}

	[[nodiscard]] MML_FORCE_INLINE Vector3 floor() const {
		return Vector3(std::floor(x), std::floor(y), std::floor(z));
	}

	[[nodiscard]] MML_FORCE_INLINE Vector3 ceil() const {
		return Vector3(std::ceil(x), std::ceil(y), std::ceil(z));
	}

	[[nodiscard]] MML_FORCE_INLINE Vector3 round() const {
		return Vector3(std::round(x), std::round(y), std::round(z));
	}

	[[nodiscard]] static MML_FORCE_INLINE Vector3 cross_normalized(const Vector3 &a, const Vector3 &b) {
		return a.cross(b).normalized();
	}

	[[nodiscard]] static MML_FORCE_INLINE Vector3 slerp(const Vector3 &a, const Vector3 &b, T t) {
		T theta = a.angle(b);
		if (Epsilon<T>::approx_zero(theta)) {
			return a.lerp(b, t);
		}
		T sin_theta = std::sin(theta);
		T wa = std::sin((T(1) - t) * theta) / sin_theta;
		T wb = std::sin(t * theta) / sin_theta;
		return a * wa + b * wb;
	}

	[[nodiscard]] static constexpr Vector3 zero() {
		return Vector3(T(0), T(0), T(0));
	}

	[[nodiscard]] static constexpr Vector3 unit_x() {
		return Vector3(T(1), T(0), T(0));
	}

	[[nodiscard]] static constexpr Vector3 unit_y() {
		return Vector3(T(0), T(1), T(0));
	}

	[[nodiscard]] static constexpr Vector3 unit_z() {
		return Vector3(T(0), T(0), T(1));
	}

	[[nodiscard]] static constexpr Vector3 up() {
		return Vector3(T(0), T(1), T(0));
	}

	[[nodiscard]] static constexpr Vector3 down() {
		return Vector3(T(0), T(-1), T(0));
	}

	[[nodiscard]] static constexpr Vector3 forward() {
		return Vector3(T(0), T(0), T(1));
	}

	[[nodiscard]] static constexpr Vector3 back() {
		return Vector3(T(0), T(0), T(-1));
	}

	[[nodiscard]] static constexpr Vector3 right() {
		return Vector3(T(1), T(0), T(0));
	}

	[[nodiscard]] static constexpr Vector3 left() {
		return Vector3(T(-1), T(0), T(0));
	}

	[[nodiscard]] static constexpr Vector3 one() {
		return Vector3(T(1), T(1), T(1));
	}

	[[nodiscard]] static MML_FORCE_INLINE T distance(const Vector3 &a, const Vector3 &b) {
		return a.distance(b);
	}

	[[nodiscard]] static constexpr MML_FORCE_INLINE Vector3 cross(const Vector3 &a, const Vector3 &b) {
		return a.cross(b);
	}

	[[nodiscard]] static constexpr MML_FORCE_INLINE T dot(const Vector3 &a, const Vector3 &b) {
		return a.dot(b);
	}

	[[nodiscard]] static MML_FORCE_INLINE T angle_between(const Vector3 &a, const Vector3 &b) {
		return a.angle(b);
	}

	[[nodiscard]] static constexpr MML_FORCE_INLINE Vector3 lerp(const Vector3 &a, const Vector3 &b, T t) {
		return a.lerp(b, t);
	}

	[[nodiscard]] static constexpr MML_FORCE_INLINE Vector3 min(const Vector3 &a, const Vector3 &b) {
		return a.min(b);
	}

	[[nodiscard]] static constexpr MML_FORCE_INLINE Vector3 max(const Vector3 &a, const Vector3 &b) {
		return a.max(b);
	}
};

template <typename T>
[[nodiscard]] MML_FORCE_INLINE Vector3<T> operator*(T scalar, const Vector3<T> &vec) {
	return vec * scalar;
}

using Vector3f = Vector3<float>;
using Vector3d = Vector3<double>;






template <typename T>
struct Matrix3 {
	static_assert(std::is_floating_point_v<T>, "Matrix3 requires floating-point type");

	Vector3<T> cols[3];

	constexpr Matrix3() = default;

	[[nodiscard]] static constexpr Matrix3 identity() {
		Matrix3 m;
		m.cols[0] = Vector3<T>(T(1), T(0), T(0));
		m.cols[1] = Vector3<T>(T(0), T(1), T(0));
		m.cols[2] = Vector3<T>(T(0), T(0), T(1));
		return m;
	}

	[[nodiscard]] static constexpr Matrix3 zero() {
		Matrix3 m;
		m.cols[0] = Vector3<T>(T(0), T(0), T(0));
		m.cols[1] = Vector3<T>(T(0), T(0), T(0));
		m.cols[2] = Vector3<T>(T(0), T(0), T(0));
		return m;
	}

	explicit constexpr Matrix3(const Vector3<T> &c0, const Vector3<T> &c1, const Vector3<T> &c2) {
		cols[0] = c0;
		cols[1] = c1;
		cols[2] = c2;
	}

	explicit constexpr Matrix3(
			T m00, T m10, T m20,
			T m01, T m11, T m21,
			T m02, T m12, T m22) {
		cols[0] = Vector3<T>(m00, m10, m20);
		cols[1] = Vector3<T>(m01, m11, m21);
		cols[2] = Vector3<T>(m02, m12, m22);
	}

	constexpr Matrix3(const Matrix3 &other) = default;
	Matrix3 &operator=(const Matrix3 &other) = default;

	constexpr MML_FORCE_INLINE Vector3<T> &operator[](size_t i) { return cols[i]; }
	constexpr MML_FORCE_INLINE const Vector3<T> &operator[](size_t i) const { return cols[i]; }

	[[nodiscard]] constexpr MML_FORCE_INLINE Matrix3 operator*(const Matrix3 &rhs) const {
		Matrix3 result;
		for (size_t col = 0; col < 3; ++col) {
			for (size_t row = 0; row < 3; ++row) {
				result[col][row] =
						cols[0][row] * rhs[col][0] +
						cols[1][row] * rhs[col][1] +
						cols[2][row] * rhs[col][2];
			}
		}
		return result;
	}

	[[nodiscard]] constexpr MML_FORCE_INLINE Vector3<T> operator*(const Vector3<T> &v) const {
		return Vector3<T>(
				cols[0].x * v.x + cols[1].x * v.y + cols[2].x * v.z,
				cols[0].y * v.x + cols[1].y * v.y + cols[2].y * v.z,
				cols[0].z * v.x + cols[1].z * v.y + cols[2].z * v.z);
	}

	[[nodiscard]] constexpr MML_FORCE_INLINE Matrix3 operator+(const Matrix3 &rhs) const {
		return Matrix3(
				cols[0] + rhs.cols[0],
				cols[1] + rhs.cols[1],
				cols[2] + rhs.cols[2]);
	}

	[[nodiscard]] constexpr MML_FORCE_INLINE Matrix3 operator-(const Matrix3 &rhs) const {
		return Matrix3(
				cols[0] - rhs.cols[0],
				cols[1] - rhs.cols[1],
				cols[2] - rhs.cols[2]);
	}

	[[nodiscard]] constexpr MML_FORCE_INLINE Matrix3 operator*(T scalar) const {
		return Matrix3(
				cols[0] * scalar,
				cols[1] * scalar,
				cols[2] * scalar);
	}

	[[nodiscard]] constexpr MML_FORCE_INLINE Matrix3 transposed() const {
		return Matrix3(
				cols[0].x, cols[1].x, cols[2].x,
				cols[0].y, cols[1].y, cols[2].y,
				cols[0].z, cols[1].z, cols[2].z);
	}

	[[nodiscard]] constexpr MML_FORCE_INLINE T determinant() const {
		return cols[0].x * (cols[1].y * cols[2].z - cols[2].y * cols[1].z) -
				cols[1].x * (cols[0].y * cols[2].z - cols[2].y * cols[0].z) +
				cols[2].x * (cols[0].y * cols[1].z - cols[1].y * cols[0].z);
	}

	[[nodiscard]] MML_FORCE_INLINE Matrix3 inverse() const {
		T det = determinant();
		if (Epsilon<T>::approx_zero(det)) {
			return identity();
		}

		T inv_det = T(1) / det;

		Matrix3 result;
		result[0][0] = (cols[1].y * cols[2].z - cols[2].y * cols[1].z) * inv_det;
		result[0][1] = (cols[2].y * cols[0].z - cols[0].y * cols[2].z) * inv_det;
		result[0][2] = (cols[0].y * cols[1].z - cols[1].y * cols[0].z) * inv_det;
		result[1][0] = (cols[2].x * cols[1].z - cols[1].x * cols[2].z) * inv_det;
		result[1][1] = (cols[0].x * cols[2].z - cols[2].x * cols[0].z) * inv_det;
		result[1][2] = (cols[1].x * cols[0].z - cols[0].x * cols[1].z) * inv_det;
		result[2][0] = (cols[1].x * cols[2].y - cols[2].x * cols[1].y) * inv_det;
		result[2][1] = (cols[2].x * cols[0].y - cols[0].x * cols[2].y) * inv_det;
		result[2][2] = (cols[0].x * cols[1].y - cols[1].x * cols[0].y) * inv_det;

		return result;
	}

	[[nodiscard]] static Matrix3 rotation_x(T angle) {
		T c = std::cos(angle);
		T s = std::sin(angle);
		return Matrix3(
				T(1), T(0), T(0),
				T(0), c, s,
				T(0), -s, c);
	}

	[[nodiscard]] static Matrix3 rotation_y(T angle) {
		T c = std::cos(angle);
		T s = std::sin(angle);
		return Matrix3(
				c, T(0), -s,
				T(0), T(1), T(0),
				s, T(0), c);
	}

	[[nodiscard]] static Matrix3 rotation_z(T angle) {
		T c = std::cos(angle);
		T s = std::sin(angle);
		return Matrix3(
				c, s, T(0),
				-s, c, T(0),
				T(0), T(0), T(1));
	}

	[[nodiscard]] static constexpr Matrix3 scale(T x, T y, T z) {
		Matrix3 m = zero();
		m[0][0] = x;
		m[1][1] = y;
		m[2][2] = z;
		return m;
	}

	[[nodiscard]] static constexpr Matrix3 scale(T scalar) {
		return scale(scalar, scalar, scalar);
	}

	[[nodiscard]] static constexpr Matrix3 scale(const Vector3<T> &v) {
		return scale(v.x, v.y, v.z);
	}

	[[nodiscard]] static Matrix3 rotation_axis(const Vector3<T> &axis, T angle) {
		T c = std::cos(angle);
		T s = std::sin(angle);
		T t = T(1) - c;
		Vector3<T> a = axis.normalized();
		T x = a.x, y = a.y, z = a.z;

		return Matrix3(
				t * x * x + c, t * x * y + s * z, t * x * z - s * y,
				t * x * y - s * z, t * y * y + c, t * y * z + s * x,
				t * x * z + s * y, t * y * z - s * x, t * z * z + c);
	}

	[[nodiscard]] static Matrix3 rotation_euler(T pitch, T yaw, T roll) {
		T cy = std::cos(yaw);
		T sy = std::sin(yaw);
		T cp = std::cos(pitch);
		T sp = std::sin(pitch);
		T cr = std::cos(roll);
		T sr = std::sin(roll);

		return Matrix3(
				cy * cr, sy * sp * cr - cp * sr, sy * cp * cr + sp * sr,
				cy * sr, sy * sp * sr + cp * cr, sy * cp * sr - sp * cr,
				-sy, cy * sp, cy * cp);
	}

	[[nodiscard]] static Matrix3 shear(T xy, T xz, T yx, T yz, T zx, T zy) {
		return Matrix3(
				T(1), yx, zx,
				xy, T(1), zy,
				xz, yz, T(1));
	}

	MML_FORCE_INLINE Matrix3 &operator*=(const Matrix3 &rhs) {
		*this = *this * rhs;
		return *this;
	}

	[[nodiscard]] MML_FORCE_INLINE Matrix3 operator/(T scalar) const {
		T inv = T(1) / scalar;
		return *this * inv;
	}

	MML_FORCE_INLINE Matrix3 &operator/=(T scalar) {
		*this = *this / scalar;
		return *this;
	}

	[[nodiscard]] MML_FORCE_INLINE Matrix3 operator-() const {
		return Matrix3(-cols[0], -cols[1], -cols[2]);
	}

	[[nodiscard]] MML_FORCE_INLINE Vector3<T> diagonal() const {
		return Vector3<T>(cols[0].x, cols[1].y, cols[2].z);
	}

	[[nodiscard]] MML_FORCE_INLINE T trace() const {
		return cols[0].x + cols[1].y + cols[2].z;
	}

	[[nodiscard]] MML_FORCE_INLINE bool is_symmetric() const {
		return Epsilon<T>::equal(cols[0].y, cols[1].x) &&
				Epsilon<T>::equal(cols[0].z, cols[2].x) &&
				Epsilon<T>::equal(cols[1].z, cols[2].y);
	}

	[[nodiscard]] MML_FORCE_INLINE bool is_orthogonal() const {
		Matrix3 transposed = this->transposed();
		Matrix3 product = *this * transposed;
		Matrix3 identity = Matrix3::identity();
		return Epsilon<T>::equal(product[0][0], identity[0][0]) &&
				Epsilon<T>::equal(product[1][1], identity[1][1]) &&
				Epsilon<T>::equal(product[2][2], identity[2][2]);
	}

	[[nodiscard]] MML_FORCE_INLINE Vector3<T> transform_point(const Vector3<T> &p) const {
		return *this * p;
	}

	[[nodiscard]] MML_FORCE_INLINE Vector3<T> transform_direction(const Vector3<T> &d) const {
		return *this * d;
	}

	[[nodiscard]] MML_FORCE_INLINE Matrix3 rotation_matrix() const {
		return *this;
	}

	[[nodiscard]] MML_FORCE_INLINE Vector3<T> scale_vector() const {
		return Vector3<T>(cols[0].length(), cols[1].length(), cols[2].length());
	}

	[[nodiscard]] MML_FORCE_INLINE Vector3<T> right_vector() const {
		return Vector3<T>(cols[0].x, cols[0].y, cols[0].z).normalized();
	}

	[[nodiscard]] MML_FORCE_INLINE Vector3<T> up_vector() const {
		return Vector3<T>(cols[1].x, cols[1].y, cols[1].z).normalized();
	}

	[[nodiscard]] MML_FORCE_INLINE Vector3<T> forward_vector() const {
		return Vector3<T>(cols[2].x, cols[2].y, cols[2].z).normalized();
	}

	[[nodiscard]] static Matrix3 from_forward_up(const Vector3<T> &forward, const Vector3<T> &up) {
		Vector3<T> f = forward.normalized();
		Vector3<T> r = up.cross(f).normalized();
		Vector3<T> u = f.cross(r);
		return Matrix3(r, u, f);
	}

	[[nodiscard]] static Matrix3 look_at(const Vector3<T> &eye, const Vector3<T> &target, const Vector3<T> &up) {
		Vector3<T> f = (target - eye).normalized();
		Vector3<T> r = up.cross(f).normalized();
		Vector3<T> u = f.cross(r);
		return Matrix3(r, u, f);
	}

	[[nodiscard]] MML_FORCE_INLINE T frobenius_norm() const {
		return std::sqrt(
				cols[0].length_squared() +
				cols[1].length_squared() +
				cols[2].length_squared());
	}

	[[nodiscard]] static Matrix3 from_euler(const Vector3<T> &angles) {
		return rotation_euler(angles.x, angles.y, angles.z);
	}

	[[nodiscard]] static Matrix3 from_basis(const Vector3<T> &right, const Vector3<T> &up, const Vector3<T> &forward) {
		return Matrix3(right, up, forward);
	}

	[[nodiscard]] Vector3<T> to_euler() const {
		T sy = cols[2].x;
		T cy = std::sqrt(T(1) - sy * sy);
		T cp, sp, cr, sr;

		if (cy > Epsilon<T>::value) {
			sp = cols[2].y;
			cp = cols[2].z;
			sr = cols[1].x;
			cr = cols[0].x;
		} else {
			sp = -cols[1].z;
			cp = cols[0].y;
			sr = cols[0].z;
			cr = cols[1].y;
		}

		T pitch = std::atan2(sp, cp);
		T yaw = std::atan2(-sy, cp);
		T roll = std::atan2(sr, cr);

		return Vector3<T>(pitch, yaw, roll);
	}
};

template <typename T>
[[nodiscard]] MML_FORCE_INLINE Matrix3<T> operator*(T scalar, const Matrix3<T> &mat) {
	return mat * scalar;
}

using Matrix3f = Matrix3<float>;
using Matrix3d = Matrix3<double>;






template <typename T>
struct is_floating_point : std::is_floating_point<T> {};

template <typename T>
inline constexpr bool is_floating_point_v = is_floating_point<T>::value;

template <typename T>
struct is_scalar : std::is_arithmetic<T> {};

template <typename T>
inline constexpr bool is_scalar_v = is_scalar<T>::value;

struct SIMDTraits {
	static constexpr bool has_sse = (MML_SIMD_SSE != 0);
	static constexpr bool has_sse2 = (MML_SIMD_SSE2 != 0);
	static constexpr bool has_sse3 = (MML_SIMD_SSE3 != 0);
	static constexpr bool has_sse4_1 = (MML_SIMD_SSE4_1 != 0);
	static constexpr bool has_avx = (MML_SIMD_AVX != 0);
	static constexpr bool has_avx2 = (MML_SIMD_AVX2 != 0);
	static constexpr bool has_neon = (MML_SIMD_NEON != 0);

	static constexpr bool enabled = (MML_USE_SIMD != 0) && (has_sse2 || has_avx || has_neon);

	static constexpr int vector_width_float = has_avx ? 8 : (has_sse2 ? 4 : 1);
	static constexpr int vector_width_double = has_avx ? 4 : (has_sse2 ? 2 : 1);
};

template <bool UseSimd>
struct Implementation {
	static constexpr bool use_simd = UseSimd && SIMDTraits::enabled;
};





namespace simd {

#if MML_SIMD_SSE2 && defined(__SSE2__)

#if MML_SIMD_SSE4_1
#endif

struct Float4 {
	__m128 v;

	Float4() = default;
	Float4(__m128 v_) :
			v(v_) {}

	explicit Float4(float x, float y, float z, float w) {
		alignas(16) float data[4] = { x, y, z, w };
		v = _mm_load_ps(data);
	}

	explicit Float4(float scalar) :
			v(_mm_set1_ps(scalar)) {}
	static Float4 zero() { return Float4(_mm_setzero_ps()); }
	static Float4 load(const float *p) { return Float4(_mm_load_ps(p)); }
	static Float4 load_aligned(const float *p) { return Float4(_mm_load_ps(p)); }
	static Float4 load_unaligned(const float *p) { return Float4(_mm_loadu_ps(p)); }

	void store(float *p) const { _mm_store_ps(p, v); }
	void store_aligned(float *p) const { _mm_store_ps(p, v); }
	void store_unaligned(float *p) const { _mm_storeu_ps(p, v); }

	Float4 operator+(const Float4 &rhs) const { return Float4(_mm_add_ps(v, rhs.v)); }
	Float4 operator-(const Float4 &rhs) const { return Float4(_mm_sub_ps(v, rhs.v)); }
	Float4 operator*(const Float4 &rhs) const { return Float4(_mm_mul_ps(v, rhs.v)); }
	Float4 operator/(const Float4 &rhs) const { return Float4(_mm_div_ps(v, rhs.v)); }

	Float4 operator*(float scalar) const { return Float4(_mm_mul_ps(v, _mm_set1_ps(scalar))); }
	Float4 operator/(float scalar) const { return Float4(_mm_div_ps(v, _mm_set1_ps(scalar))); }

	float dot3(const Float4 &rhs) const {
		__m128 mul = _mm_mul_ps(v, rhs.v);
		__m128 shuf = _mm_movehdup_ps(mul);
		return _mm_cvtss_f32(_mm_add_ss(mul, shuf));
	}

	float dot4(const Float4 &rhs) const {
#if MML_SIMD_SSE4_1
		return _mm_cvtss_f32(_mm_dp_ps(v, rhs.v, 0xF1));
#else
		__m128 mul = _mm_mul_ps(v, rhs.v);
		__m128 shuf = _mm_movehdup_ps(mul);
		__m128 sum = _mm_add_ss(mul, shuf);
		shuf = _mm_movehl_ps(sum, sum);
		sum = _mm_add_ss(sum, shuf);
		return _mm_cvtss_f32(sum);
#endif
	}

	Float4 cross3(const Float4 &rhs) const {
		__m128 tmp0 = _mm_shuffle_ps(v, v, _MM_SHUFFLE(3, 0, 2, 1));
		__m128 tmp1 = _mm_shuffle_ps(rhs.v, rhs.v, _MM_SHUFFLE(3, 1, 0, 2));
		__m128 tmp2 = _mm_shuffle_ps(v, v, _MM_SHUFFLE(3, 1, 0, 2));
		__m128 tmp3 = _mm_shuffle_ps(rhs.v, rhs.v, _MM_SHUFFLE(3, 0, 2, 1));
		return Float4(_mm_sub_ps(_mm_mul_ps(tmp0, tmp1), _mm_mul_ps(tmp2, tmp3)));
	}

	Float4 min(const Float4 &rhs) const { return Float4(_mm_min_ps(v, rhs.v)); }
	Float4 max(const Float4 &rhs) const { return Float4(_mm_max_ps(v, rhs.v)); }
	Float4 abs() const {
		__m128 mask = _mm_castsi128_ps(_mm_set1_epi32(0x7FFFFFFF));
		return Float4(_mm_and_ps(v, mask));
	}
};

struct Double2 {
	__m128d v;

	Double2() = default;
	Double2(__m128d v_) :
			v(v_) {}

	explicit Double2(double x, double y) {
		alignas(16) double data[2] = { x, y };
		v = _mm_load_pd(data);
	}

	explicit Double2(double scalar) :
			v(_mm_set1_pd(scalar)) {}
	static Double2 zero() { return Double2(_mm_setzero_pd()); }

	double dot(const Double2 &rhs) const {
		__m128d mul = _mm_mul_pd(v, rhs.v);
		__m128d shuf = _mm_shuffle_pd(mul, mul, 1);
		__m128d sum = _mm_add_pd(mul, shuf);
		return _mm_cvtsd_f64(sum);
	}
};

#if MML_SIMD_AVX

struct Float8 {
	__m256 v;

	Float8() = default;
	Float8(__m256 v_) :
			v(v_) {}

	explicit Float8(float scalar) :
			v(_mm256_set1_ps(scalar)) {}
	static Float8 zero() { return Float8(_mm256_setzero_ps()); }

	Float8 operator+(const Float8 &rhs) const { return Float8(_mm256_add_ps(v, rhs.v)); }
	Float8 operator-(const Float8 &rhs) const { return Float8(_mm256_sub_ps(v, rhs.v)); }
	Float8 operator*(const Float8 &rhs) const { return Float8(_mm256_mul_ps(v, rhs.v)); }
	Float8 operator*(float scalar) const { return Float8(_mm256_mul_ps(v, _mm_set1_ps(scalar))); }

	struct Double4 {
		__m256d v;

		Double4() = default;
		Double4(__m256d v_) :
				v(v_) {}

		explicit Double4(double scalar) :
				v(_mm256_set1_pd(scalar)) {}
		static Double4 zero() { return Double4(_mm256_setzero_pd()); }

		Double4 operator+(const Double4 &rhs) const { return Double4(_mm256_add_pd(v, rhs.v)); }
		Double4 operator-(const Double4 &rhs) const { return Double4(_mm256_sub_pd(v, rhs.v)); }
		Double4 operator*(const Double4 &rhs) const { return Double4(_mm256_mul_pd(v, rhs.v)); }
	};
#endif

#else

struct Float4 {
	float x, y, z, w;

	Float4() = default;
	Float4(float x_, float y_, float z_, float w_) :
			x(x_), y(y_), z(z_), w(w_) {}

	explicit Float4(float scalar) :
			x(scalar), y(scalar), z(scalar), w(scalar) {}
	static Float4 zero() { return Float4(0, 0, 0, 0); }

	Float4 operator+(const Float4 &rhs) const { return Float4(x + rhs.x, y + rhs.y, z + rhs.z, w + rhs.w); }
	Float4 operator-(const Float4 &rhs) const { return Float4(x - rhs.x, y - rhs.y, z - rhs.z, w - rhs.w); }
	Float4 operator*(const Float4 &rhs) const { return Float4(x * rhs.x, y * rhs.y, z * rhs.z, w * rhs.w); }
	Float4 operator*(float scalar) const { return Float4(x * scalar, y * scalar, z * scalar, w * scalar); }

	float dot4(const Float4 &rhs) const { return x * rhs.x + y * rhs.y + z * rhs.z + w * rhs.w; }
	float dot3(const Float4 &rhs) const { return x * rhs.x + y * rhs.y + z * rhs.z; }
};

struct Double2 {
	double x, y;

	Double2() = default;
	Double2(double x_, double y_) :
			x(x_), y(y_) {}

	double dot(const Double2 &rhs) const { return x * rhs.x + y * rhs.y; }
};
#endif

} 





template <typename T>
struct Vector2 {
	static_assert(std::is_floating_point_v<T>, "Vector2 requires floating-point type");

	T x, y;

	Vector2() = default;

	explicit constexpr Vector2(T scalar) :
			x(scalar), y(scalar) {}

	explicit constexpr Vector2(T x_, T y_) :
			x(x_), y(y_) {}

	constexpr Vector2(const Vector2 &other) = default;
	Vector2 &operator=(const Vector2 &other) = default;

	MML_FORCE_INLINE T &operator[](size_t i) { return (&x)[i]; }
	MML_FORCE_INLINE const T &operator[](size_t i) const { return (&x)[i]; }

	[[nodiscard]] constexpr MML_FORCE_INLINE Vector2 operator+(const Vector2 &rhs) const {
		return Vector2(x + rhs.x, y + rhs.y);
	}

	constexpr MML_FORCE_INLINE Vector2 &operator+=(const Vector2 &rhs) {
		x += rhs.x;
		y += rhs.y;
		return *this;
	}

	[[nodiscard]] constexpr MML_FORCE_INLINE Vector2 operator-(const Vector2 &rhs) const {
		return Vector2(x - rhs.x, y - rhs.y);
	}

	constexpr MML_FORCE_INLINE Vector2 &operator-=(const Vector2 &rhs) {
		x -= rhs.x;
		y -= rhs.y;
		return *this;
	}

	[[nodiscard]] constexpr MML_FORCE_INLINE Vector2 operator*(T scalar) const {
		return Vector2(x * scalar, y * scalar);
	}

	constexpr MML_FORCE_INLINE Vector2 &operator*=(T scalar) {
		x *= scalar;
		y *= scalar;
		return *this;
	}

	[[nodiscard]] constexpr MML_FORCE_INLINE Vector2 operator/(T scalar) const {
		T inv = T(1) / scalar;
		return Vector2(x * inv, y * inv);
	}

	constexpr MML_FORCE_INLINE Vector2 &operator/=(T scalar) {
		T inv = T(1) / scalar;
		x *= inv;
		y *= inv;
		return *this;
	}

	[[nodiscard]] constexpr MML_FORCE_INLINE Vector2 operator-() const {
		return Vector2(-x, -y);
	}

	[[nodiscard]] constexpr MML_FORCE_INLINE bool operator==(const Vector2 &rhs) const {
		return Epsilon<T>::equal(x, rhs.x) && Epsilon<T>::equal(y, rhs.y);
	}

	[[nodiscard]] constexpr MML_FORCE_INLINE bool operator!=(const Vector2 &rhs) const {
		return !(*this == rhs);
	}

	[[nodiscard]] constexpr MML_FORCE_INLINE T dot(const Vector2 &rhs) const {
		return x * rhs.x + y * rhs.y;
	}

	[[nodiscard]] constexpr MML_FORCE_INLINE T length_squared() const {
		return x * x + y * y;
	}

	[[nodiscard]] MML_FORCE_INLINE T length() const {
		return std::sqrt(length_squared());
	}

	[[nodiscard]] MML_FORCE_INLINE Vector2 normalized() const {
		T len = length();
		if (Epsilon<T>::approx_zero(len)) {
			return Vector2(T(0), T(0));
		}
		return *this / len;
	}

	MML_FORCE_INLINE Vector2 &normalize() {
		T len = length();
		if (!Epsilon<T>::approx_zero(len)) {
			*this /= len;
		}
		return *this;
	}

	[[nodiscard]] MML_FORCE_INLINE bool is_zero() const {
		return Epsilon<T>::approx_zero(length_squared());
	}

	[[nodiscard]] MML_FORCE_INLINE bool is_unit() const {
		return Epsilon<T>::equal(length_squared(), T(1));
	}

	[[nodiscard]] MML_FORCE_INLINE bool is_finite() const {
		return std::isfinite(x) && std::isfinite(y);
	}

	[[nodiscard]] MML_FORCE_INLINE T distance(const Vector2 &rhs) const {
		return (*this - rhs).length();
	}

	[[nodiscard]] MML_FORCE_INLINE T distance_squared(const Vector2 &rhs) const {
		return (*this - rhs).length_squared();
	}

	[[nodiscard]] MML_FORCE_INLINE T angle(const Vector2 &rhs) const {
		T len_sq = length_squared() * rhs.length_squared();
		if (Epsilon<T>::approx_zero(len_sq)) {
			return T(0);
		}
		T dot_val = dot(rhs);
		T value = dot_val / Math<T>::safe_sqrt(len_sq);
		return Math<T>::safe_acos(value);
	}

	[[nodiscard]] constexpr MML_FORCE_INLINE Vector2 lerp(const Vector2 &rhs, T t) const {
		return Vector2(
				x + (rhs.x - x) * t,
				y + (rhs.y - y) * t);
	}

	[[nodiscard]] constexpr MML_FORCE_INLINE Vector2 perpendicular() const {
		return Vector2(-y, x);
	}

	[[nodiscard]] constexpr MML_FORCE_INLINE Vector2 perpendicular_cw() const {
		return Vector2(y, -x);
	}

	[[nodiscard]] constexpr MML_FORCE_INLINE Vector2 reflect(const Vector2 &normal) const {
		return *this - normal * (T(2) * this->dot(normal));
	}

	[[nodiscard]] MML_FORCE_INLINE Vector2 refract(const Vector2 &normal, T eta) const {
		T dot_n = this->dot(normal);
		T k = T(1) - eta * eta * (T(1) - dot_n * dot_n);
		if (k < T(0)) {
			return Vector2(T(0), T(0));
		}
		return *this * eta - normal * (eta * dot_n + std::sqrt(k));
	}

	[[nodiscard]] constexpr MML_FORCE_INLINE Vector2 project(const Vector2 &onto) const {
		T onto_len_sq = onto.length_squared();
		if (Epsilon<T>::approx_zero(onto_len_sq)) {
			return Vector2(T(0), T(0));
		}
		return onto * (this->dot(onto) / onto_len_sq);
	}

	[[nodiscard]] constexpr MML_FORCE_INLINE Vector2 hadamard(const Vector2 &rhs) const {
		return Vector2(x * rhs.x, y * rhs.y);
	}

	[[nodiscard]] constexpr MML_FORCE_INLINE Vector2 hadamard_div(const Vector2 &rhs) const {
		return Vector2(x / rhs.x, y / rhs.y);
	}

	[[nodiscard]] constexpr MML_FORCE_INLINE T cross(const Vector2 &rhs) const {
		return x * rhs.y - y * rhs.x;
	}

	[[nodiscard]] constexpr MML_FORCE_INLINE Vector2 min(const Vector2 &rhs) const {
		return Vector2(x < rhs.x ? x : rhs.x, y < rhs.y ? y : rhs.y);
	}

	[[nodiscard]] constexpr MML_FORCE_INLINE Vector2 max(const Vector2 &rhs) const {
		return Vector2(x > rhs.x ? x : rhs.x, y > rhs.y ? y : rhs.y);
	}

	[[nodiscard]] constexpr MML_FORCE_INLINE Vector2 clamp(T min_val, T max_val) const {
		T clamped_x = x < min_val ? min_val : (x > max_val ? max_val : x);
		T clamped_y = y < min_val ? min_val : (y > max_val ? max_val : y);
		return Vector2(clamped_x, clamped_y);
	}

	[[nodiscard]] constexpr MML_FORCE_INLINE Vector2 clamp(const Vector2 &min_vec, const Vector2 &max_vec) const {
		T clamped_x = x < min_vec.x ? min_vec.x : (x > max_vec.x ? max_vec.x : x);
		T clamped_y = y < min_vec.y ? min_vec.y : (y > max_vec.y ? max_vec.y : y);
		return Vector2(clamped_x, clamped_y);
	}

	[[nodiscard]] constexpr MML_FORCE_INLINE Vector2 abs() const {
		return Vector2(x < T(0) ? -x : x, y < T(0) ? -y : y);
	}

	[[nodiscard]] constexpr MML_FORCE_INLINE Vector2 sign() const {
		return Vector2(
				x > T(0) ? T(1) : (x < T(0) ? T(-1) : T(0)),
				y > T(0) ? T(1) : (y < T(0) ? T(-1) : T(0)));
	}

	[[nodiscard]] MML_FORCE_INLINE Vector2 floor() const {
		return Vector2(std::floor(x), std::floor(y));
	}

	[[nodiscard]] MML_FORCE_INLINE Vector2 ceil() const {
		return Vector2(std::ceil(x), std::ceil(y));
	}

	[[nodiscard]] MML_FORCE_INLINE Vector2 round() const {
		return Vector2(std::round(x), std::round(y));
	}

	[[nodiscard]] static MML_FORCE_INLINE Vector2 slerp(const Vector2 &a, const Vector2 &b, T t) {
		T theta = a.angle(b);
		if (Epsilon<T>::approx_zero(theta)) {
			return a.lerp(b, t);
		}
		T sin_theta = std::sin(theta);
		T wa = std::sin((T(1) - t) * theta) / sin_theta;
		T wb = std::sin(t * theta) / sin_theta;
		return a * wa + b * wb;
	}

	[[nodiscard]] static constexpr Vector2 zero() {
		return Vector2(T(0), T(0));
	}

	[[nodiscard]] static constexpr Vector2 unit_x() {
		return Vector2(T(1), T(0));
	}

	[[nodiscard]] static constexpr Vector2 unit_y() {
		return Vector2(T(0), T(1));
	}

	[[nodiscard]] static constexpr Vector2 one() {
		return Vector2(T(1), T(1));
	}

	[[nodiscard]] static constexpr MML_FORCE_INLINE T distance(const Vector2 &a, const Vector2 &b) {
		return a.distance(b);
	}

	[[nodiscard]] static constexpr MML_FORCE_INLINE T dot(const Vector2 &a, const Vector2 &b) {
		return a.dot(b);
	}

	[[nodiscard]] static MML_FORCE_INLINE T angle_between(const Vector2 &a, const Vector2 &b) {
		return a.angle(b);
	}

	[[nodiscard]] static constexpr MML_FORCE_INLINE Vector2 lerp(const Vector2 &a, const Vector2 &b, T t) {
		return a.lerp(b, t);
	}

	[[nodiscard]] static constexpr MML_FORCE_INLINE Vector2 min(const Vector2 &a, const Vector2 &b) {
		return a.min(b);
	}

	[[nodiscard]] static constexpr MML_FORCE_INLINE Vector2 max(const Vector2 &a, const Vector2 &b) {
		return a.max(b);
	}
};

template <typename T>
[[nodiscard]] MML_FORCE_INLINE Vector2<T> operator*(T scalar, const Vector2<T> &vec) {
	return vec * scalar;
}

using Vector2f = Vector2<float>;
using Vector2d = Vector2<double>;






template <typename T>
struct Vector4 {
	static_assert(std::is_floating_point_v<T>, "Vector4 requires floating-point type");

	T x, y, z, w;

	Vector4() = default;

	explicit constexpr Vector4(T scalar) :
			x(scalar), y(scalar), z(scalar), w(scalar) {}

	explicit constexpr Vector4(T x_, T y_, T z_, T w_) :
			x(x_), y(y_), z(z_), w(w_) {}

	explicit constexpr Vector4(const Vector3<T> &v, T w_) :
			x(v.x), y(v.y), z(v.z), w(w_) {}

	constexpr Vector4(const Vector4 &other) = default;
	Vector4 &operator=(const Vector4 &other) = default;

	MML_FORCE_INLINE T &operator[](size_t i) { return (&x)[i]; }
	constexpr MML_FORCE_INLINE const T &operator[](size_t i) const { return (&x)[i]; }

	[[nodiscard]] constexpr MML_FORCE_INLINE Vector4 operator+(const Vector4 &rhs) const {
		return Vector4(x + rhs.x, y + rhs.y, z + rhs.z, w + rhs.w);
	}

	constexpr MML_FORCE_INLINE Vector4 &operator+=(const Vector4 &rhs) {
		x += rhs.x;
		y += rhs.y;
		z += rhs.z;
		w += rhs.w;
		return *this;
	}

	[[nodiscard]] constexpr MML_FORCE_INLINE Vector4 operator-(const Vector4 &rhs) const {
		return Vector4(x - rhs.x, y - rhs.y, z - rhs.z, w - rhs.w);
	}

	constexpr MML_FORCE_INLINE Vector4 &operator-=(const Vector4 &rhs) {
		x -= rhs.x;
		y -= rhs.y;
		z -= rhs.z;
		w -= rhs.w;
		return *this;
	}

	[[nodiscard]] constexpr MML_FORCE_INLINE Vector4 operator*(T scalar) const {
		return Vector4(x * scalar, y * scalar, z * scalar, w * scalar);
	}

	constexpr MML_FORCE_INLINE Vector4 &operator*=(T scalar) {
		x *= scalar;
		y *= scalar;
		z *= scalar;
		w *= scalar;
		return *this;
	}

	[[nodiscard]] constexpr MML_FORCE_INLINE Vector4 operator/(T scalar) const {
		T inv = T(1) / scalar;
		return Vector4(x * inv, y * inv, z * inv, w * inv);
	}

	constexpr MML_FORCE_INLINE Vector4 &operator/=(T scalar) {
		T inv = T(1) / scalar;
		x *= inv;
		y *= inv;
		z *= inv;
		w *= inv;
		return *this;
	}

	[[nodiscard]] constexpr MML_FORCE_INLINE Vector4 operator-() const {
		return Vector4(-x, -y, -z, -w);
	}

	[[nodiscard]] constexpr MML_FORCE_INLINE bool operator==(const Vector4 &rhs) const {
		return Epsilon<T>::equal(x, rhs.x) &&
				Epsilon<T>::equal(y, rhs.y) &&
				Epsilon<T>::equal(z, rhs.z) &&
				Epsilon<T>::equal(w, rhs.w);
	}

	[[nodiscard]] constexpr MML_FORCE_INLINE bool operator!=(const Vector4 &rhs) const {
		return !(*this == rhs);
	}

	[[nodiscard]] constexpr MML_FORCE_INLINE T dot(const Vector4 &rhs) const {
		return x * rhs.x + y * rhs.y + z * rhs.z + w * rhs.w;
	}

	[[nodiscard]] constexpr MML_FORCE_INLINE T length_squared() const {
		return x * x + y * y + z * z + w * w;
	}

	[[nodiscard]] MML_FORCE_INLINE T length() const {
		return std::sqrt(length_squared());
	}

	[[nodiscard]] MML_FORCE_INLINE Vector4 normalized() const {
		T len = length();
		if (Epsilon<T>::approx_zero(len)) {
			return Vector4(T(0), T(0), T(0), T(0));
		}
		return *this / len;
	}

	MML_FORCE_INLINE Vector4 &normalize() {
		T len = length();
		if (!Epsilon<T>::approx_zero(len)) {
			*this /= len;
		}
		return *this;
	}

	[[nodiscard]] MML_FORCE_INLINE bool is_zero() const {
		return Epsilon<T>::approx_zero(length_squared());
	}

	[[nodiscard]] MML_FORCE_INLINE bool is_unit() const {
		return Epsilon<T>::equal(length_squared(), T(1));
	}

	[[nodiscard]] MML_FORCE_INLINE bool is_finite() const {
		return std::isfinite(x) && std::isfinite(y) && std::isfinite(z) && std::isfinite(w);
	}

	[[nodiscard]] MML_FORCE_INLINE Vector3<T> xyz() const {
		return Vector3<T>(x, y, z);
	}

	[[nodiscard]] MML_FORCE_INLINE Vector3<T> xyw() const {
		return Vector3<T>(x, y, w);
	}

	[[nodiscard]] MML_FORCE_INLINE Vector2<T> xy() const {
		return Vector2<T>(x, y);
	}

	[[nodiscard]] MML_FORCE_INLINE T angle(const Vector4 &rhs) const {
		T len_sq = length_squared() * rhs.length_squared();
		if (Epsilon<T>::approx_zero(len_sq)) {
			return T(0);
		}
		T dot_val = dot(rhs);
		T value = dot_val / Math<T>::safe_sqrt(len_sq);
		return Math<T>::safe_acos(value);
	}

	[[nodiscard]] constexpr MML_FORCE_INLINE Vector4 lerp(const Vector4 &rhs, T t) const {
		return Vector4(
				x + (rhs.x - x) * t,
				y + (rhs.y - y) * t,
				z + (rhs.z - z) * t,
				w + (rhs.w - w) * t);
	}

	[[nodiscard]] constexpr MML_FORCE_INLINE Vector4 min(const Vector4 &rhs) const {
		return Vector4(x < rhs.x ? x : rhs.x, y < rhs.y ? y : rhs.y, z < rhs.z ? z : rhs.z, w < rhs.w ? w : rhs.w);
	}

	[[nodiscard]] constexpr MML_FORCE_INLINE Vector4 max(const Vector4 &rhs) const {
		return Vector4(x > rhs.x ? x : rhs.x, y > rhs.y ? y : rhs.y, z > rhs.z ? z : rhs.z, w > rhs.w ? w : rhs.w);
	}

	[[nodiscard]] constexpr MML_FORCE_INLINE Vector4 hadamard(const Vector4 &rhs) const {
		return Vector4(x * rhs.x, y * rhs.y, z * rhs.z, w * rhs.w);
	}

	[[nodiscard]] constexpr MML_FORCE_INLINE Vector4 hadamard_div(const Vector4 &rhs) const {
		return Vector4(x / rhs.x, y / rhs.y, z / rhs.z, w / rhs.w);
	}

	[[nodiscard]] constexpr MML_FORCE_INLINE Vector4 clamp(T min_val, T max_val) const {
		T clamped_x = x < min_val ? min_val : (x > max_val ? max_val : x);
		T clamped_y = y < min_val ? min_val : (y > max_val ? max_val : y);
		T clamped_z = z < min_val ? min_val : (z > max_val ? max_val : z);
		T clamped_w = w < min_val ? min_val : (w > max_val ? max_val : w);
		return Vector4(clamped_x, clamped_y, clamped_z, clamped_w);
	}

	[[nodiscard]] constexpr MML_FORCE_INLINE Vector4 clamp(const Vector4 &min_vec, const Vector4 &max_vec) const {
		T clamped_x = x < min_vec.x ? min_vec.x : (x > max_vec.x ? max_vec.x : x);
		T clamped_y = y < min_vec.y ? min_vec.y : (y > max_vec.y ? max_vec.y : y);
		T clamped_z = z < min_vec.z ? min_vec.z : (z > max_vec.z ? max_vec.z : z);
		T clamped_w = w < min_vec.w ? min_vec.w : (w > max_vec.w ? max_vec.w : w);
		return Vector4(clamped_x, clamped_y, clamped_z, clamped_w);
	}

	[[nodiscard]] constexpr MML_FORCE_INLINE Vector4 abs() const {
		return Vector4(x < T(0) ? -x : x, y < T(0) ? -y : y, z < T(0) ? -z : z, w < T(0) ? -w : w);
	}

	[[nodiscard]] constexpr MML_FORCE_INLINE Vector4 sign() const {
		return Vector4(
				x > T(0) ? T(1) : (x < T(0) ? T(-1) : T(0)),
				y > T(0) ? T(1) : (y < T(0) ? T(-1) : T(0)),
				z > T(0) ? T(1) : (z < T(0) ? T(-1) : T(0)),
				w > T(0) ? T(1) : (w < T(0) ? T(-1) : T(0)));
	}

	[[nodiscard]] MML_FORCE_INLINE Vector4 floor() const {
		return Vector4(std::floor(x), std::floor(y), std::floor(z), std::floor(w));
	}

	[[nodiscard]] MML_FORCE_INLINE Vector4 ceil() const {
		return Vector4(std::ceil(x), std::ceil(y), std::ceil(z), std::ceil(w));
	}

	[[nodiscard]] MML_FORCE_INLINE Vector4 round() const {
		return Vector4(std::round(x), std::round(y), std::round(z), std::round(w));
	}

	[[nodiscard]] static MML_FORCE_INLINE Vector4 slerp(const Vector4 &a, const Vector4 &b, T t) {
		T theta = a.angle(b);
		if (Epsilon<T>::approx_zero(theta)) {
			return a.lerp(b, t);
		}
		T sin_theta = std::sin(theta);
		T wa = std::sin((T(1) - t) * theta) / sin_theta;
		T wb = std::sin(t * theta) / sin_theta;
		return a * wa + b * wb;
	}

	[[nodiscard]] static constexpr Vector4 zero() {
		return Vector4(T(0), T(0), T(0), T(0));
	}

	[[nodiscard]] static constexpr Vector4 unit_x() {
		return Vector4(T(1), T(0), T(0), T(0));
	}

	[[nodiscard]] static constexpr Vector4 unit_y() {
		return Vector4(T(0), T(1), T(0), T(0));
	}

	[[nodiscard]] static constexpr Vector4 unit_z() {
		return Vector4(T(0), T(0), T(1), T(0));
	}

	[[nodiscard]] static constexpr Vector4 unit_w() {
		return Vector4(T(0), T(0), T(0), T(1));
	}

	[[nodiscard]] static constexpr Vector4 one() {
		return Vector4(T(1), T(1), T(1), T(1));
	}

	[[nodiscard]] static constexpr Vector4 from_vector3(const Vector3<T> &v, T w_) {
		return Vector4(v.x, v.y, v.z, w_);
	}

	[[nodiscard]] static MML_FORCE_INLINE T distance(const Vector4 &a, const Vector4 &b) {
		return a.distance(b);
	}

	[[nodiscard]] static constexpr MML_FORCE_INLINE T dot(const Vector4 &a, const Vector4 &b) {
		return a.dot(b);
	}

	[[nodiscard]] static MML_FORCE_INLINE T angle_between(const Vector4 &a, const Vector4 &b) {
		return a.angle(b);
	}

	[[nodiscard]] static constexpr MML_FORCE_INLINE Vector4 lerp(const Vector4 &a, const Vector4 &b, T t) {
		return a.lerp(b, t);
	}

	[[nodiscard]] static constexpr MML_FORCE_INLINE Vector4 min(const Vector4 &a, const Vector4 &b) {
		return a.min(b);
	}

	[[nodiscard]] static constexpr MML_FORCE_INLINE Vector4 max(const Vector4 &a, const Vector4 &b) {
		return a.max(b);
	}
};

template <typename T>
MML_FORCE_INLINE Vector4<T> operator*(T scalar, const Vector4<T> &vec) {
	return vec * scalar;
}

using Vector4f = Vector4<float>;
using Vector4d = Vector4<double>;

#if MML_USE_SIMD && MML_SIMD_SSE2 && defined(__SSE2__)

template <>
struct Vector4<float> {
	__m128 v;

	Vector4() = default;
	explicit constexpr Vector4(float scalar) :
			v(_mm_set1_ps(scalar)) {}
	explicit constexpr Vector4(float x_, float y_, float z_, float w_) :
			v(_mm_setr_ps(x_, y_, z_, w_)) {}
	explicit constexpr Vector4(const Vector3<float> &vec, float w_) :
			v(_mm_setr_ps(vec.x, vec.y, vec.z, w_)) {}
	constexpr Vector4(const Vector4 &other) = default;
	Vector4 &operator=(const Vector4 &other) = default;

	MML_FORCE_INLINE float &operator[](size_t i) { return ((float *)&v)[i]; }
	constexpr MML_FORCE_INLINE const float &operator[](size_t i) const { return ((float *)&v)[i]; }

	constexpr MML_FORCE_INLINE Vector4 operator+(const Vector4 &rhs) const {
		return Vector4(_mm_add_ps(v, rhs.v));
	}

	constexpr MML_FORCE_INLINE Vector4 operator-(const Vector4 &rhs) const {
		return Vector4(_mm_sub_ps(v, rhs.v));
	}

	constexpr MML_FORCE_INLINE Vector4 &operator-=(const Vector4 &rhs) {
		v = _mm_sub_ps(v, rhs.v);
		return *this;
	}

	constexpr MML_FORCE_INLINE Vector4 operator*(float scalar) const {
		return Vector4(_mm_mul_ps(v, _mm_set1_ps(scalar)));
	}

	constexpr MML_FORCE_INLINE Vector4 &operator*=(float scalar) {
		v = _mm_mul_ps(v, _mm_set1_ps(scalar));
		return *this;
	}

	constexpr MML_FORCE_INLINE Vector4 operator/(float scalar) const {
		return Vector4(_mm_div_ps(v, _mm_set1_ps(scalar)));
	}

	constexpr MML_FORCE_INLINE Vector4 &operator/=(float scalar) {
		v = _mm_div_ps(v, _mm_set1_ps(scalar));
		return *this;
	}

	constexpr MML_FORCE_INLINE Vector4 operator-() const {
		return Vector4(_mm_xor_ps(v, _mm_set1_ps(-0.0f)));
	}

	constexpr MML_FORCE_INLINE bool operator==(const Vector4 &rhs) const {
		__m128 cmp = _mm_cmpeq_ps(v, rhs.v);
		return _mm_movemask_ps(cmp) == 0xF;
	}

	constexpr MML_FORCE_INLINE bool operator!=(const Vector4 &rhs) const {
		return !(*this == rhs);
	}

	constexpr MML_FORCE_INLINE float dot(const Vector4 &rhs) const {
#if MML_SIMD_SSE4_1
		return _mm_cvtss_f32(_mm_dp_ps(v, rhs.v, 0xF1));
#else
		__m128 mul = _mm_mul_ps(v, rhs.v);
		__m128 shuf = _mm_movehdup_ps(mul);
		__m128 sum = _mm_add_ss(mul, shuf);
		shuf = _mm_movehl_ps(sum, sum);
		sum = _mm_add_ss(sum, shuf);
		return _mm_cvtss_f32(_mm_add_ss(sum, _mm_movehdup_ps(sum, sum)));
#endif
	}

	constexpr MML_FORCE_INLINE float dot3(const Vector4 &rhs) const {
#if MML_SIMD_SSE4_1
		return _mm_cvtss_f32(_mm_dp_ps(v, rhs.v, 0x71));
#else
		__m128 mul = _mm_mul_ps(v, rhs.v);
		__m128 shuf = _mm_movehdup_ps(mul);
		return _mm_cvtss_f32(_mm_add_ss(mul, shuf));
#endif
	}

	constexpr MML_FORCE_INLINE float length_squared() const {
		return dot(*this);
	}

	constexpr MML_FORCE_INLINE float length_squared3() const {
		return dot3(*this);
	}

	MML_FORCE_INLINE float length() const {
		return _mm_cvtss_f32(_mm_sqrt_ss(_mm_dp_ps(v, v, 0xF1)));
	}

	MML_FORCE_INLINE float length3() const {
#if MML_SIMD_SSE4_1
		return _mm_cvtss_f32(_mm_sqrt_ss(_mm_dp_ps(v, v, 0x71)));
#else
		__m128 dp = _mm_mul_ps(v, v);
		__m128 shuf = _mm_movehdup_ps(dp);
		return _mm_cvtss_f32(_mm_sqrt_ss(_mm_add_ss(dp, shuf)));
#endif
	}

	MML_FORCE_INLINE Vector4 normalized() const {
		float len = length();
		if (Epsilon<float>::approx_zero(len)) {
			return Vector4(0.0f, 0.0f, 0.0f, 0.0f);
		}
		return *this / len;
	}

	MML_FORCE_INLINE Vector4 &normalize() {
		float len = length();
		if (!Epsilon<float>::approx_zero(len)) {
			*this /= len;
		}
		return *this;
	}

	MML_FORCE_INLINE Vector4 normalized3() const {
		float len = length3();
		if (Epsilon<float>::approx_zero(len)) {
			return Vector4(0.0f, 0.0f, 0.0f, 0.0f);
		}
		float inv_len = 1.0f / len;
		__m128 len_vec = _mm_setr_ps(inv_len, inv_len, inv_len, 0.0f);
		return Vector4(_mm_mul_ps(v, len_vec));
	}

	MML_FORCE_INLINE Vector3<float> xyz() const {
		return Vector3<float>(((float *)&v)[0], ((float *)&v)[1], ((float *)&v)[2]);
	}

	MML_FORCE_INLINE Vector3<float> xyw() const {
		return Vector3<float>(((float *)&v)[0], ((float *)&v)[1], ((float *)&v)[3]);
	}

	MML_FORCE_INLINE Vector2<float> xy() const {
		return Vector2<float>(((float *)&v)[0], ((float *)&v)[1]);
	}

	MML_FORCE_INLINE float angle(const Vector4 &rhs) const {
		float len_sq = length_squared() * rhs.length_squared();
		if (Epsilon<float>::approx_zero(len_sq)) {
			return 0.0f;
		}
		float dot_val = dot(rhs);
		float value = dot_val / Math<float>::safe_sqrt(len_sq);
		return Math<float>::safe_acos(value);
	}

	constexpr MML_FORCE_INLINE Vector4 lerp(const Vector4 &rhs, float t) const {
		__m128 t_vec = _mm_set1_ps(t);
		__m128 diff = _mm_sub_ps(rhs.v, v);
		return Vector4(_mm_add_ps(v, _mm_mul_ps(diff, t_vec)));
	}

	constexpr MML_FORCE_INLINE Vector4 min(const Vector4 &rhs) const {
		return Vector4(_mm_min_ps(v, rhs.v));
	}

	constexpr MML_FORCE_INLINE Vector4 max(const Vector4 &rhs) const {
		return Vector4(_mm_max_ps(v, rhs.v));
	}

	constexpr MML_FORCE_INLINE Vector4 clamp(float min_val, float max_val) const {
		__m128 min_vec = _mm_set1_ps(min_val);
		__m128 max_vec = _mm_set1_ps(max_val);
		return Vector4(_mm_min_ps(_mm_max_ps(v, min_vec), max_vec));
	}

	constexpr MML_FORCE_INLINE Vector4 clamp(const Vector4 &min_vec, const Vector4 &max_vec) const {
		return Vector4(_mm_min_ps(_mm_max_ps(v, min_vec.v), max_vec.v));
	}

	constexpr MML_FORCE_INLINE Vector4 abs() const {
		__m128 mask = _mm_castsi128_ps(_mm_set1_epi32(0x7FFFFFFF));
		return Vector4(_mm_and_ps(v, mask));
	}

	constexpr MML_FORCE_INLINE Vector4 sign() const {
		__m128 zero = _mm_setzero_ps();
		__m128 one = _mm_set1_ps(1.0f);
		__m128 neg_one = _mm_set1_ps(-1.0f);
		__m128 pos_mask = _mm_and_ps(_mm_cmpgt_ps(v, zero), one);
		__m128 neg_mask = _mm_and_ps(_mm_cmplt_ps(v, zero), neg_one);
		return Vector4(_mm_or_ps(pos_mask, neg_mask));
	}

	MML_FORCE_INLINE Vector4 floor() const {
#if MML_SIMD_SSE4_1
		return Vector4(_mm_floor_ps(v));
#else
		return Vector4(std::floor(((float *)&v)[0]), std::floor(((float *)&v)[1]), std::floor(((float *)&v)[2]), std::floor(((float *)&v)[3]));
#endif
	}

	MML_FORCE_INLINE Vector4 ceil() const {
#if MML_SIMD_SSE4_1
		return Vector4(_mm_ceil_ps(v));
#else
		return Vector4(std::ceil(((float *)&v)[0]), std::ceil(((float *)&v)[1]), std::ceil(((float *)&v)[2]), std::ceil(((float *)&v)[3]));
#endif
	}

	MML_FORCE_INLINE Vector4 round() const {
#if MML_SIMD_SSE4_1
		return Vector4(_mm_round_ps(v, _MM_FROUND_TO_NEAREST_INT | _MM_FROUND_NO_EXC));
#else
		return Vector4(std::round(((float *)&v)[0]), std::round(((float *)&v)[1]), std::round(((float *)&v)[2]), std::round(((float *)&v)[3]));
#endif
	}

	static MML_FORCE_INLINE Vector4 slerp(const Vector4 &a, const Vector4 &b, float t) {
		float theta = a.angle(b);
		if (Epsilon<float>::approx_zero(theta)) {
			return a.lerp(b, t);
		}
		float sin_theta = std::sin(theta);
		float wa = std::sin((1.0f - t) * theta) / sin_theta;
		float wb = std::sin(t * theta) / sin_theta;
		return a * wa + b * wb;
	}

	static constexpr Vector4 zero() {
		return Vector4(_mm_setzero_ps());
	}

	static constexpr Vector4 unit_x() {
		return Vector4(_mm_setr_ps(1.0f, 0.0f, 0.0f, 0.0f));
	}

	static constexpr Vector4 unit_y() {
		return Vector4(_mm_setr_ps(0.0f, 1.0f, 0.0f, 0.0f));
	}

	static constexpr Vector4 unit_z() {
		return Vector4(_mm_setr_ps(0.0f, 0.0f, 1.0f, 0.0f));
	}

	static constexpr Vector4 unit_w() {
		return Vector4(_mm_setr_ps(0.0f, 0.0f, 0.0f, 1.0f));
	}

	static constexpr Vector4 one() {
		return Vector4(_mm_set1_ps(1.0f));
	}

	static constexpr Vector4 from_vector3(const Vector3<float> &vec, float w_) {
		return Vector4(vec, w_);
	}

	static MML_FORCE_INLINE float distance(const Vector4 &a, const Vector4 &b) {
		return a.distance(b);
	}

	static constexpr MML_FORCE_INLINE float dot(const Vector4 &a, const Vector4 &b) {
		return a.dot(b);
	}

	static MML_FORCE_INLINE float angle_between(const Vector4 &a, const Vector4 &b) {
		return a.angle(b);
	}

	static constexpr MML_FORCE_INLINE Vector4 lerp(const Vector4 &a, const Vector4 &b, float t) {
		return a.lerp(b, t);
	}

	static constexpr MML_FORCE_INLINE Vector4 min(const Vector4 &a, const Vector4 &b) {
		return a.min(b);
	}

	static constexpr MML_FORCE_INLINE Vector4 max(const Vector4 &a, const Vector4 &b) {
		return a.max(b);
	}

private:
	float distance(const Vector4 &rhs) const {
		Vector4 diff = *this - rhs;
		return diff.length();
	}
};

#endif






template <typename T>
struct [[nodiscard]] Matrix4 {
	static_assert(std::is_floating_point_v<T>, "Matrix4 requires floating-point type");

	Vector4<T> cols[4];

	constexpr Matrix4() = default;

	[[nodiscard]] static constexpr Matrix4 identity() {
		Matrix4 m;
		m.cols[0] = Vector4<T>(T(1), T(0), T(0), T(0));
		m.cols[1] = Vector4<T>(T(0), T(1), T(0), T(0));
		m.cols[2] = Vector4<T>(T(0), T(0), T(1), T(0));
		m.cols[3] = Vector4<T>(T(0), T(0), T(0), T(1));
		return m;
	}

	[[nodiscard]] static constexpr Matrix4 zero() {
		Matrix4 m;
		m.cols[0] = Vector4<T>(T(0), T(0), T(0), T(0));
		m.cols[1] = Vector4<T>(T(0), T(0), T(0), T(0));
		m.cols[2] = Vector4<T>(T(0), T(0), T(0), T(0));
		m.cols[3] = Vector4<T>(T(0), T(0), T(0), T(0));
		return m;
	}

	explicit constexpr Matrix4(const Vector4<T> &c0, const Vector4<T> &c1, const Vector4<T> &c2, const Vector4<T> &c3) {
		cols[0] = c0;
		cols[1] = c1;
		cols[2] = c2;
		cols[3] = c3;
	}

	explicit constexpr Matrix4(
			T m00, T m10, T m20, T m30,
			T m01, T m11, T m21, T m31,
			T m02, T m12, T m22, T m32,
			T m03, T m13, T m23, T m33) {
		cols[0] = Vector4<T>(m00, m10, m20, m30);
		cols[1] = Vector4<T>(m01, m11, m21, m31);
		cols[2] = Vector4<T>(m02, m12, m22, m32);
		cols[3] = Vector4<T>(m03, m13, m23, m33);
	}

	constexpr Matrix4(const Matrix4 &other) = default;
	Matrix4 &operator=(const Matrix4 &other) = default;

	[[nodiscard]] constexpr MML_FORCE_INLINE Vector4<T> &operator[](size_t i) { return cols[i]; }
	[[nodiscard]] constexpr MML_FORCE_INLINE const Vector4<T> &operator[](size_t i) const { return cols[i]; }

	[[nodiscard]] constexpr MML_FORCE_INLINE Matrix4 operator*(const Matrix4 &rhs) const {
		Matrix4 result;
#if MML_SIMD_SSE2 && defined(__SSE2__)
		if constexpr (std::is_same_v<T, float>) {
			for (size_t col = 0; col < 4; ++col) {
				__m128 c0 = _mm_loadu_ps(&cols[0].x);
				__m128 c1 = _mm_loadu_ps(&cols[1].x);
				__m128 c2 = _mm_loadu_ps(&cols[2].x);
				__m128 c3 = _mm_loadu_ps(&cols[3].x);

				__m128 x = _mm_set1_ps(rhs[col][0]);
				__m128 y = _mm_set1_ps(rhs[col][1]);
				__m128 z = _mm_set1_ps(rhs[col][2]);
				__m128 w = _mm_set1_ps(rhs[col][3]);

				__m128 res = _mm_add_ps(
						_mm_add_ps(_mm_mul_ps(c0, x), _mm_mul_ps(c1, y)),
						_mm_add_ps(_mm_mul_ps(c2, z), _mm_mul_ps(c3, w)));
				_mm_storeu_ps(&result[col].x, res);
			}
			return result;
		}
#endif
		for (size_t col = 0; col < 4; ++col) {
			for (size_t row = 0; row < 4; ++row) {
				result[col][row] =
						cols[0][row] * rhs[col][0] +
						cols[1][row] * rhs[col][1] +
						cols[2][row] * rhs[col][2] +
						cols[3][row] * rhs[col][3];
			}
		}
		return result;
	}

	[[nodiscard]] constexpr MML_FORCE_INLINE Vector4<T> operator*(const Vector4<T> &v) const {
#if MML_SIMD_SSE2 && defined(__SSE2__)
		if constexpr (std::is_same_v<T, float>) {
			__m128 c0 = _mm_loadu_ps(&cols[0].x);
			__m128 c1 = _mm_loadu_ps(&cols[1].x);
			__m128 c2 = _mm_loadu_ps(&cols[2].x);
			__m128 c3 = _mm_loadu_ps(&cols[3].x);

			__m128 vx = _mm_set1_ps(v.x);
			__m128 vy = _mm_set1_ps(v.y);
			__m128 vz = _mm_set1_ps(v.z);
			__m128 vw = _mm_set1_ps(v.w);

			__m128 res = _mm_add_ps(
					_mm_add_ps(_mm_mul_ps(c0, vx), _mm_mul_ps(c1, vy)),
					_mm_add_ps(_mm_mul_ps(c2, vz), _mm_mul_ps(c3, vw)));

			Vector4<float> result;
			_mm_storeu_ps(&result.x, res);
			return result;
		}
#endif
		return Vector4<T>(
				cols[0].x * v.x + cols[1].x * v.y + cols[2].x * v.z + cols[3].x * v.w,
				cols[0].y * v.x + cols[1].y * v.y + cols[2].y * v.z + cols[3].y * v.w,
				cols[0].z * v.x + cols[1].z * v.y + cols[2].z * v.z + cols[3].z * v.w,
				cols[0].w * v.x + cols[1].w * v.y + cols[2].w * v.z + cols[3].w * v.w);
	}

	[[nodiscard]] constexpr MML_FORCE_INLINE Matrix4 operator+(const Matrix4 &rhs) const {
		return Matrix4(
				cols[0] + rhs.cols[0],
				cols[1] + rhs.cols[1],
				cols[2] + rhs.cols[2],
				cols[3] + rhs.cols[3]);
	}

	[[nodiscard]] constexpr MML_FORCE_INLINE Matrix4 operator-(const Matrix4 &rhs) const {
		return Matrix4(
				cols[0] - rhs.cols[0],
				cols[1] - rhs.cols[1],
				cols[2] - rhs.cols[2],
				cols[3] - rhs.cols[3]);
	}

	[[nodiscard]] constexpr MML_FORCE_INLINE Matrix4 operator*(T scalar) const {
		return Matrix4(
				cols[0] * scalar,
				cols[1] * scalar,
				cols[2] * scalar,
				cols[3] * scalar);
	}

	[[nodiscard]] constexpr MML_FORCE_INLINE Matrix4 transposed() const {
		return Matrix4(
				cols[0].x, cols[0].y, cols[0].z, cols[0].w,
				cols[1].x, cols[1].y, cols[1].z, cols[1].w,
				cols[2].x, cols[2].y, cols[2].z, cols[2].w,
				cols[3].x, cols[3].y, cols[3].z, cols[3].w);
	}

	[[nodiscard]] MML_FORCE_INLINE Vector4<T> diagonal() const {
		return Vector4<T>(cols[0].x, cols[1].y, cols[2].z, cols[3].w);
	}

	[[nodiscard]] MML_FORCE_INLINE T trace() const {
		return cols[0].x + cols[1].y + cols[2].z + cols[3].w;
	}

	[[nodiscard]] MML_FORCE_INLINE Vector3<T> translation() const {
		return Vector3<T>(cols[3].x, cols[3].y, cols[3].z);
	}

	[[nodiscard]] MML_FORCE_INLINE Matrix3<T> to_matrix3() const {
		return Matrix3<T>(
				Vector3<T>(cols[0].x, cols[0].y, cols[0].z),
				Vector3<T>(cols[1].x, cols[1].y, cols[1].z),
				Vector3<T>(cols[2].x, cols[2].y, cols[2].z));
	}

	[[nodiscard]] MML_FORCE_INLINE Vector3<T> transform_point(const Vector3<T> &p) const {
		Vector4<T> result = (*this) * Vector4<T>(p, T(1));
		return Vector3<T>(result.x, result.y, result.z);
	}

	[[nodiscard]] MML_FORCE_INLINE Vector3<T> transform_direction(const Vector3<T> &d) const {
		return Vector3<T>(
				cols[0].x * d.x + cols[1].x * d.y + cols[2].x * d.z,
				cols[0].y * d.x + cols[1].y * d.y + cols[2].y * d.z,
				cols[0].z * d.x + cols[1].z * d.y + cols[2].z * d.z);
	}

	[[nodiscard]] static Matrix4 translation(T x, T y, T z) {
		Matrix4 m = identity();
		m[3][0] = x;
		m[3][1] = y;
		m[3][2] = z;
		return m;
	}

	[[nodiscard]] static Matrix4 translation(const Vector3<T> &v) {
		return translation(v.x, v.y, v.z);
	}

	[[nodiscard]] static constexpr Matrix4 scale(T x, T y, T z) {
		Matrix4 m = zero();
		m[0][0] = x;
		m[1][1] = y;
		m[2][2] = z;
		m[3][3] = T(1);
		return m;
	}

	[[nodiscard]] static constexpr Matrix4 scale(const Vector3<T> &v) {
		return scale(v.x, v.y, v.z);
	}

	[[nodiscard]] static Matrix4 rotation_x(T angle) {
		T c = std::cos(angle);
		T s = std::sin(angle);
		Matrix4 m = identity();
		m[1][1] = c;
		m[1][2] = s;
		m[2][1] = -s;
		m[2][2] = c;
		return m;
	}

	[[nodiscard]] static Matrix4 rotation_y(T angle) {
		T c = std::cos(angle);
		T s = std::sin(angle);
		Matrix4 m = identity();
		m[0][0] = c;
		m[0][2] = -s;
		m[2][0] = s;
		m[2][2] = c;
		return m;
	}

	[[nodiscard]] static Matrix4 rotation_z(T angle) {
		T c = std::cos(angle);
		T s = std::sin(angle);
		Matrix4 m = identity();
		m[0][0] = c;
		m[0][1] = s;
		m[1][0] = -s;
		m[1][1] = c;
		return m;
	}

	[[nodiscard]] static Matrix4 rotation_axis(const Vector3<T> &axis, T angle) {
		T c = std::cos(angle);
		T s = std::sin(angle);
		T t = T(1) - c;
		Vector3<T> a = axis.normalized();
		T x = a.x, y = a.y, z = a.z;

		Matrix4 m = identity();
		m[0][0] = t * x * x + c;
		m[0][1] = t * x * y + s * z;
		m[0][2] = t * x * z - s * y;
		m[1][0] = t * x * y - s * z;
		m[1][1] = t * y * y + c;
		m[1][2] = t * y * z + s * x;
		m[2][0] = t * x * z + s * y;
		m[2][1] = t * y * z - s * x;
		m[2][2] = t * z * z + c;
		return m;
	}

	[[nodiscard]] static Matrix4 rotation_euler(T pitch, T yaw, T roll) {
		T cy = std::cos(yaw);
		T sy = std::sin(yaw);
		T cp = std::cos(pitch);
		T sp = std::sin(pitch);
		T cr = std::cos(roll);
		T sr = std::sin(roll);

		Matrix4 m = identity();
		m[0][0] = cy * cr;
		m[0][1] = sy * sp * cr - cp * sr;
		m[0][2] = sy * cp * cr + sp * sr;
		m[1][0] = cy * sr;
		m[1][1] = sy * sp * sr + cp * cr;
		m[1][2] = sy * cp * sr - sp * cr;
		m[2][0] = -sy;
		m[2][1] = cy * sp;
		m[2][2] = cy * cp;
		return m;
	}

	[[nodiscard]] static Matrix4 look_at(const Vector3<T> &eye, const Vector3<T> &target, const Vector3<T> &up) {
		Vector3<T> f = (target - eye).normalized();
		Vector3<T> s = f.cross(up).normalized();
		Vector3<T> u = s.cross(f);

		Matrix4 m = identity();
		m[0][0] = s.x;
		m[1][0] = s.y;
		m[2][0] = s.z;
		m[0][1] = u.x;
		m[1][1] = u.y;
		m[2][1] = u.z;
		m[0][2] = -f.x;
		m[1][2] = -f.y;
		m[2][2] = -f.z;
		m[3][0] = -s.dot(eye);
		m[3][1] = -u.dot(eye);
		m[3][2] = f.dot(eye);
		return m;
	}

	[[nodiscard]] static Matrix4 orthographic(T left, T right, T bottom, T top, T near_plane, T far_plane) {
		T tx = -(right + left) / (right - left);
		T ty = -(top + bottom) / (top - bottom);
		T tz = -(far_plane + near_plane) / (far_plane - near_plane);

		Matrix4 m = zero();
		m[0][0] = T(2) / (right - left);
		m[1][1] = T(2) / (top - bottom);
		m[2][2] = -T(2) / (far_plane - near_plane);
		m[3][0] = tx;
		m[3][1] = ty;
		m[3][2] = tz;
		m[3][3] = T(1);
		return m;
	}

	[[nodiscard]] static Matrix4 perspective(T fov_y, T aspect, T near_plane, T far_plane) {
		T tan_half_fov = std::tan(fov_y * T(0.5));

		Matrix4 m = zero();
		m[0][0] = T(1) / (aspect * tan_half_fov);
		m[1][1] = T(1) / tan_half_fov;
		m[2][2] = -(far_plane + near_plane) / (far_plane - near_plane);
		m[2][3] = -T(1);
		m[3][2] = -(T(2) * far_plane * near_plane) / (far_plane - near_plane);
		return m;
	}

	[[nodiscard]] MML_FORCE_INLINE Vector3<T> right_vector() const {
		return Vector3<T>(cols[0].x, cols[0].y, cols[0].z).normalized();
	}

	[[nodiscard]] MML_FORCE_INLINE Vector3<T> up_vector() const {
		return Vector3<T>(cols[1].x, cols[1].y, cols[1].z).normalized();
	}

	[[nodiscard]] MML_FORCE_INLINE Vector3<T> forward_vector() const {
		return Vector3<T>(cols[2].x, cols[2].y, cols[2].z).normalized();
	}

	void decompose(Vector3<T> &out_translation, Matrix3<T> &out_rotation, Vector3<T> &out_scale) const {
		out_translation = translation();
		out_scale = Vector3<T>(
				Vector3<T>(cols[0].x, cols[0].y, cols[0].z).length(),
				Vector3<T>(cols[1].x, cols[1].y, cols[1].z).length(),
				Vector3<T>(cols[2].x, cols[2].y, cols[2].z).length());

		Matrix3<T> rot(
				Vector3<T>(cols[0].x, cols[0].y, cols[0].z) / out_scale.x,
				Vector3<T>(cols[1].x, cols[1].y, cols[1].z) / out_scale.y,
				Vector3<T>(cols[2].x, cols[2].y, cols[2].z) / out_scale.z);
		out_rotation = rot;
	}

	[[nodiscard]] MML_FORCE_INLINE T frobenius_norm() const {
		T sum = T(0);
		for (size_t i = 0; i < 4; ++i) {
			sum += cols[i].length_squared();
		}
		return std::sqrt(sum);
	}

	[[nodiscard]] static Matrix4 from_euler(const Vector3<T> &angles) {
		return rotation_euler(angles.x, angles.y, angles.z);
	}

	[[nodiscard]] static Matrix4 from_basis(const Vector3<T> &right, const Vector3<T> &up, const Vector3<T> &forward) {
		Matrix4 m = identity();
		m[0][0] = right.x;
		m[1][0] = right.y;
		m[2][0] = right.z;
		m[0][1] = up.x;
		m[1][1] = up.y;
		m[2][1] = up.z;
		m[0][2] = forward.x;
		m[1][2] = forward.y;
		m[2][2] = forward.z;
		return m;
	}

	[[nodiscard]] MML_FORCE_INLINE Vector3<T> right_vector_raw() const {
		return Vector3<T>(cols[0].x, cols[0].y, cols[0].z);
	}

	[[nodiscard]] MML_FORCE_INLINE Vector3<T> up_vector_raw() const {
		return Vector3<T>(cols[1].x, cols[1].y, cols[1].z);
	}

	[[nodiscard]] MML_FORCE_INLINE Vector3<T> forward_vector_raw() const {
		return Vector3<T>(cols[2].x, cols[2].y, cols[2].z);
	}
};

using Matrix4f = Matrix4<float>;
using Matrix4d = Matrix4<double>;






template <typename T>
struct Quaternion {
	static_assert(std::is_floating_point_v<T>, "Quaternion requires floating-point type");

	T x, y, z, w;

	Quaternion() = default;

	[[nodiscard]] static constexpr Quaternion identity() {
		return Quaternion(T(0), T(0), T(0), T(1));
	}

	explicit constexpr Quaternion(T x_, T y_, T z_, T w_) :
			x(x_), y(y_), z(z_), w(w_) {}

	[[nodiscard]] static Quaternion from_axis_angle(const Vector3<T> &axis, T angle) {
		T half_angle = angle * T(0.5);
		T s = std::sin(half_angle);
		Vector3<T> normalized_axis = axis.normalized();
		return Quaternion(
				normalized_axis.x * s,
				normalized_axis.y * s,
				normalized_axis.z * s,
				std::cos(half_angle));
	}

	[[nodiscard]] static Quaternion from_euler(T pitch, T yaw, T roll) {
		T cy = std::cos(yaw * T(0.5));
		T sy = std::sin(yaw * T(0.5));
		T cp = std::cos(pitch * T(0.5));
		T sp = std::sin(pitch * T(0.5));
		T cr = std::cos(roll * T(0.5));
		T sr = std::sin(roll * T(0.5));

		return Quaternion(
				sr * cp * cy - cr * sp * sy,
				cr * sp * cy + sr * cp * sy,
				cr * cp * sy - sr * sp * cy,
				cr * cp * cy + sr * sp * sy);
	}

	[[nodiscard]] static Quaternion from_euler(const Vector3<T> &angles) {
		return from_euler(angles.x, angles.y, angles.z);
	}

	[[nodiscard]] static Quaternion from_to_rotation(const Vector3<T> &from, const Vector3<T> &to) {
		Vector3<T> from_norm = from.normalized();
		Vector3<T> to_norm = to.normalized();

		T dot_val = from_norm.dot(to_norm);

		if (dot_val >= T(1) - Epsilon<T>::value) {
			return identity();
		}

		if (dot_val <= T(-1) + Epsilon<T>::value) {
			Vector3<T> orth = std::abs(from_norm.x) < std::abs(from_norm.y) ? Vector3<T>::unit_x() : Vector3<T>::unit_y();
			Vector3<T> axis = from_norm.cross(orth).normalized();
			return from_axis_angle(axis, Constants<T>::pi);
		}

		Vector3<T> axis = from_norm.cross(to_norm);
		T w = std::sqrt((T(1) + dot_val) * T(2));
		T inv_w = T(1) / w;

		return Quaternion(axis.x * inv_w, axis.y * inv_w, axis.z * inv_w, w * T(0.5));
	}

	[[nodiscard]] static Quaternion rotation_between(const Vector3<T> &from, const Vector3<T> &to) {
		return from_to_rotation(from, to);
	}

	[[nodiscard]] void to_axis_angle(Vector3<T> &out_axis, T &out_angle) const {
		out_angle = angle();
		out_axis = axis();
	}

	[[nodiscard]] Quaternion rotation_to(const Quaternion &to) const {
		return conjugate() * to;
	}

	[[nodiscard]] T angle_to(const Quaternion &to) const {
		return rotation_to(to).angle();
	}

	[[nodiscard]] static Quaternion look_rotation(const Vector3<T> &forward, const Vector3<T> &up = Vector3<T>::up()) {
		Vector3<T> f = forward.normalized();
		Vector3<T> r = up.cross(f).normalized();
		Vector3<T> u = f.cross(r);

		Matrix3<T> m(r, u, f);
		return from_matrix(m);
	}

	[[nodiscard]] static Quaternion from_matrix(const Matrix3<T> &m) {
		T trace = m[0][0] + m[1][1] + m[2][2];

		if (trace > T(0)) {
			T s = Math<T>::safe_sqrt(trace + T(1)) * T(2);
			return Quaternion(
					(m[1][2] - m[2][1]) / s,
					(m[2][0] - m[0][2]) / s,
					(m[0][1] - m[1][0]) / s,
					s * T(0.25));
		} else if (m[0][0] > m[1][1] && m[0][0] > m[2][2]) {
			T s = Math<T>::safe_sqrt(T(1) + m[0][0] - m[1][1] - m[2][2]) * T(2);
			return Quaternion(
					s * T(0.25),
					(m[1][0] + m[0][1]) / s,
					(m[2][0] + m[0][2]) / s,
					(m[1][2] - m[2][1]) / s);
		} else if (m[1][1] > m[2][2]) {
			T s = Math<T>::safe_sqrt(T(1) + m[1][1] - m[0][0] - m[2][2]) * T(2);
			return Quaternion(
					(m[1][0] + m[0][1]) / s,
					s * T(0.25),
					(m[2][1] + m[1][2]) / s,
					(m[2][0] - m[0][2]) / s);
		} else {
			T s = Math<T>::safe_sqrt(T(1) + m[2][2] - m[0][0] - m[1][1]) * T(2);
			return Quaternion(
					(m[2][0] + m[0][2]) / s,
					(m[2][1] + m[1][2]) / s,
					s * T(0.25),
					(m[0][1] - m[1][0]) / s);
		}
	}

	constexpr Quaternion(const Quaternion &other) = default;
	Quaternion &operator=(const Quaternion &other) = default;

	[[nodiscard]] MML_FORCE_INLINE Quaternion operator+(const Quaternion &rhs) const {
		return Quaternion(x + rhs.x, y + rhs.y, z + rhs.z, w + rhs.w);
	}

	[[nodiscard]] MML_FORCE_INLINE Quaternion operator-(const Quaternion &rhs) const {
		return Quaternion(x - rhs.x, y - rhs.y, z - rhs.z, w - rhs.w);
	}

	[[nodiscard]] MML_FORCE_INLINE Quaternion operator*(const Quaternion &rhs) const {
		return Quaternion(
				w * rhs.x + x * rhs.w + y * rhs.z - z * rhs.y,
				w * rhs.y - x * rhs.z + y * rhs.w + z * rhs.x,
				w * rhs.z + x * rhs.y - y * rhs.x + z * rhs.w,
				w * rhs.w - x * rhs.x - y * rhs.y - z * rhs.z);
	}

	[[nodiscard]] MML_FORCE_INLINE Quaternion operator*(T scalar) const {
		return Quaternion(x * scalar, y * scalar, z * scalar, w * scalar);
	}

	[[nodiscard]] MML_FORCE_INLINE Vector3<T> rotate(const Vector3<T> &v) const {
		Vector3<T> qv(x, y, z);
		Vector3<T> t = qv.cross(v) * T(2);
		return v + t * w + qv.cross(t);
	}

	[[nodiscard]] MML_FORCE_INLINE T dot(const Quaternion &rhs) const {
		return x * rhs.x + y * rhs.y + z * rhs.z + w * rhs.w;
	}

	[[nodiscard]] MML_FORCE_INLINE T length_squared() const {
		return x * x + y * y + z * z + w * w;
	}

	[[nodiscard]] MML_FORCE_INLINE T length() const {
		return Math<T>::safe_sqrt(length_squared());
	}

	[[nodiscard]] MML_FORCE_INLINE bool is_zero() const {
		return Epsilon<T>::approx_zero(length_squared());
	}

	[[nodiscard]] MML_FORCE_INLINE bool is_identity() const {
		return Epsilon<T>::equal(x, T(0)) && Epsilon<T>::equal(y, T(0)) &&
				Epsilon<T>::equal(z, T(0)) && Epsilon<T>::equal(w, T(1));
	}

	[[nodiscard]] MML_FORCE_INLINE bool is_finite() const {
		return std::isfinite(x) && std::isfinite(y) && std::isfinite(z) && std::isfinite(w);
	}

	[[nodiscard]] MML_FORCE_INLINE bool is_normalized() const {
		return Epsilon<T>::equal(length_squared(), T(1));
	}

	[[nodiscard]] MML_FORCE_INLINE Quaternion normalized() const {
		T len = length();
		if (Epsilon<T>::approx_zero(len)) {
			return identity();
		}
		T inv_len = T(1) / len;
		return Quaternion(x * inv_len, y * inv_len, z * inv_len, w * inv_len);
	}

	MML_FORCE_INLINE Quaternion &normalize() {
		T len = length();
		if (!Epsilon<T>::approx_zero(len)) {
			T inv_len = T(1) / len;
			x *= inv_len;
			y *= inv_len;
			z *= inv_len;
			w *= inv_len;
		}
		return *this;
	}

	[[nodiscard]] MML_FORCE_INLINE Quaternion conjugate() const {
		return Quaternion(-x, -y, -z, w);
	}

	[[nodiscard]] MML_FORCE_INLINE Quaternion inverse() const {
		T len_sq = length_squared();
		if (Epsilon<T>::approx_zero(len_sq)) {
			return identity();
		}
		T inv_len_sq = T(1) / len_sq;
		return Quaternion(-x * inv_len_sq, -y * inv_len_sq, -z * inv_len_sq, w * inv_len_sq);
	}

	[[nodiscard]] Matrix3<T> to_matrix3() const {
		T xx = x * x, yy = y * y, zz = z * z;
		T xy = x * y, xz = x * z, yz = y * z;
		T wx = w * x, wy = w * y, wz = w * z;

		return Matrix3<T>(
				T(1) - T(2) * (yy + zz), T(2) * (xy + wz), T(2) * (xz - wy),
				T(2) * (xy - wz), T(1) - T(2) * (xx + zz), T(2) * (yz + wx),
				T(2) * (xz + wy), T(2) * (yz - wx), T(1) - T(2) * (xx + yy));
	}

	[[nodiscard]] Matrix4<T> to_matrix4() const {
		Matrix3<T> m3 = to_matrix3();
		return Matrix4<T>(
				Vector4<T>(m3[0], T(0)),
				Vector4<T>(m3[1], T(0)),
				Vector4<T>(m3[2], T(0)),
				Vector4<T>(T(0), T(0), T(0), T(1)));
	}

	[[nodiscard]] static Quaternion slerp(const Quaternion &a, const Quaternion &b, T t) {
		T cos_theta = a.dot(b);

		Quaternion b_adjusted = b;
		if (cos_theta < T(0)) {
			b_adjusted = Quaternion(-b.x, -b.y, -b.z, -b.w);
			cos_theta = -cos_theta;
		}

		if (cos_theta > T(1) - Epsilon<T>::value) {
			return Quaternion(
					a.x + t * (b_adjusted.x - a.x),
					a.y + t * (b_adjusted.y - a.y),
					a.z + t * (b_adjusted.z - a.z),
					a.w + t * (b_adjusted.w - a.w))
					.normalized();
		}

		T theta = Math<T>::safe_acos(cos_theta);
		T sin_theta = std::sin(theta);
		T wa = std::sin((T(1) - t) * theta) / sin_theta;
		T wb = std::sin(t * theta) / sin_theta;

		return Quaternion(
				a.x * wa + b_adjusted.x * wb,
				a.y * wa + b_adjusted.y * wb,
				a.z * wa + b_adjusted.z * wb,
				a.w * wa + b_adjusted.w * wb);
	}

	[[nodiscard]] static Quaternion squad(const Quaternion &q0, const Quaternion &q1, const Quaternion &q2, const Quaternion &q3, T t) {
		Quaternion s0 = slerp(q0, q3, t);
		Quaternion s1 = slerp(q1, q2, t);
		return slerp(s0, s1, 2.0 * t * (1.0 - t));
	}

	[[nodiscard]] static Quaternion squad_intermediate(const Quaternion &q_prev, const Quaternion &q_curr, const Quaternion &q_next) {
		Quaternion q_prev_inv = q_prev.conjugate();
		Quaternion q_next_inv = q_next.conjugate();
		Quaternion term1 = q_prev_inv * q_curr;
		Quaternion term2 = q_next_inv * q_curr;
		return q_curr * (term1 + term2).normalized() * T(-0.25);
	}

	[[nodiscard]] static Quaternion lerp_unnormalized(const Quaternion &a, const Quaternion &b, T t) {
		return Quaternion(
				a.x + t * (b.x - a.x),
				a.y + t * (b.y - a.y),
				a.z + t * (b.z - a.z),
				a.w + t * (b.w - a.w));
	}

	[[nodiscard]] static Quaternion nlerp(const Quaternion &a, const Quaternion &b, T t) {
		return lerp_unnormalized(a, b, t).normalized();
	}

	[[nodiscard]] Quaternion exp() const {
		Vector3<T> v(x, y, z);
		T v_len = v.length();

		if (Epsilon<T>::approx_zero(v_len)) {
			return identity();
		}

		T s = std::sin(v_len) / v_len;
		T e = std::exp(w);

		return Quaternion(
				e * s * v.x,
				e * s * v.y,
				e * s * v.z,
				e * std::cos(v_len));
	}

	[[nodiscard]] Quaternion log() const {
		T len = length();

		if (Epsilon<T>::approx_zero(len)) {
			return Quaternion(T(0), T(0), T(0), std::numeric_limits<T>::lowest());
		}

		T v_len = Math<T>::safe_sqrt(x * x + y * y + z * z);

		if (Epsilon<T>::approx_zero(v_len)) {
			return Quaternion(T(0), T(0), T(0), std::log(len));
		}

		T coeff = Math<T>::safe_acos(w / len) / v_len;
		return Quaternion(
				coeff * x,
				coeff * y,
				coeff * z,
				std::log(len));
	}

	[[nodiscard]] Quaternion pow(T exponent) const {
		T len = length();
		if (Epsilon<T>::approx_zero(len)) {
			return identity();
		}

		T alpha = Math<T>::safe_acos(w / len);
		T new_alpha = alpha * exponent;
		T s = std::sin(new_alpha) / std::sin(alpha);

		return Quaternion(
				s * x,
				s * y,
				s * z,
				std::cos(new_alpha));
	}

	[[nodiscard]] Quaternion negate() const {
		return Quaternion(-x, -y, -z, -w);
	}

	[[nodiscard]] T angle() const {
		return T(2) * Math<T>::safe_acos(std::abs(w) / length());
	}

	[[nodiscard]] Vector3<T> axis() const {
		T s = Math<T>::safe_sqrt(T(1) - w * w);
		if (Epsilon<T>::approx_zero(s)) {
			return Vector3<T>::unit_x();
		}
		return Vector3<T>(x, y, z) / s;
	}

	[[nodiscard]] Vector3<T> to_euler() const {
		T sinr_cosp = T(2) * (w * x + y * z);
		T cosr_cosp = T(1) - T(2) * (x * x + y * y);
		T roll = std::atan2(sinr_cosp, cosr_cosp);

		T sinp = T(2) * (w * y - z * x);
		T pitch;
		if (std::abs(sinp) >= T(1)) {
			pitch = std::copysign(Constants<T>::half_pi, sinp);
		} else {
			pitch = Math<T>::safe_asin(sinp);
		}

		T siny_cosp = T(2) * (w * z + x * y);
		T cosy_cosp = T(1) - T(2) * (y * y + z * z);
		T yaw = std::atan2(siny_cosp, cosy_cosp);

		return Vector3<T>(pitch, yaw, roll);
	}

	[[nodiscard]] Vector3<T> forward() const {
		return rotate(Vector3<T>::forward());
	}

	[[nodiscard]] Vector3<T> up() const {
		return rotate(Vector3<T>::up());
	}

	[[nodiscard]] Vector3<T> right() const {
		return rotate(Vector3<T>::right());
	}

	[[nodiscard]] static T angle_between(const Quaternion &a, const Quaternion &b) {
		T dot_val = a.dot(b);
		return T(2) * Math<T>::safe_acos(std::abs(dot_val));
	}

	[[nodiscard]] static T angular_distance(const Quaternion &a, const Quaternion &b) {
		Quaternion diff = b.inverse() * a;
		return diff.angle();
	}

	[[nodiscard]] static Quaternion shortest_path(const Quaternion &a, const Quaternion &b) {
		if (a.dot(b) < T(0)) {
			return Quaternion(-b.x, -b.y, -b.z, -b.w);
		}
		return b;
	}

	[[nodiscard]] bool is_valid() const {
		return Epsilon<T>::equal(length(), T(1));
	}

	void swing_twist(const Vector3<T> &twist_axis, Quaternion &out_swing, Quaternion &out_twist) const {
		Vector3<T> r(x, y, z);
		T p = r.dot(twist_axis);
		out_twist = Quaternion(twist_axis * p, w).normalized();
		out_swing = *this * out_twist.conjugate();
	}

	[[nodiscard]] Quaternion roll_quat() const {
		return Quaternion(x, T(0), T(0), w).normalized();
	}

	[[nodiscard]] Quaternion pitch_quat() const {
		return Quaternion(T(0), y, T(0), w).normalized();
	}

	[[nodiscard]] Quaternion yaw_quat() const {
		return Quaternion(T(0), T(0), z, w).normalized();
	}

	struct DualQuaternion {
		Quaternion real;
		Quaternion dual;

		static DualQuaternion from_rotation_translation(const Quaternion &rot, const Vector3<T> &trans) {
			DualQuaternion dq;
			dq.real = rot;
			dq.dual = Quaternion(T(0.5) * trans.x, T(0.5) * trans.y, T(0.5) * trans.z, T(0)) * rot;
			return dq;
		}

		Vector3<T> transform_point(const Vector3<T> &p) const {
			Quaternion point(T(0), p.x, p.y, p.z);
			Quaternion result = real * point * real.conjugate() + T(2) * (dual * real.conjugate());
			return Vector3<T>(result.x, result.y, result.z);
		}
	};

	[[nodiscard]] Quaternion scale(T scale_factor) const {
		T new_len = length() * scale_factor;
		T factor = new_len / length();
		return Quaternion(x * factor, y * factor, z * factor, w * factor);
	}

	[[nodiscard]] T distance_to(const Quaternion &rhs) const {
		return angle_between(*this, rhs);
	}

	[[nodiscard]] constexpr MML_FORCE_INLINE bool operator==(const Quaternion &rhs) const {
		return Epsilon<T>::equal(x, rhs.x) &&
				Epsilon<T>::equal(y, rhs.y) &&
				Epsilon<T>::equal(z, rhs.z) &&
				Epsilon<T>::equal(w, rhs.w);
	}

	[[nodiscard]] constexpr MML_FORCE_INLINE bool operator!=(const Quaternion &rhs) const {
		return !(*this == rhs);
	}
};

template <typename T>
[[nodiscard]] MML_FORCE_INLINE Quaternion<T> operator*(T scalar, const Quaternion<T> &q) {
	return q * scalar;
}

using Quaternionf = Quaternion<float>;
using Quaterniond = Quaternion<double>;






template <typename T>
struct DualQuaternion {
	static_assert(std::is_floating_point_v<T>, "DualQuaternion requires floating-point type");

	Quaternion<T> real;
	Quaternion<T> dual;

	DualQuaternion() = default;

	DualQuaternion(const Quaternion<T> &r, const Quaternion<T> &d) :
			real(r), dual(d) {}

	static DualQuaternion from_rotation_translation(const Quaternion<T> &rotation, const Vector3<T> &translation) {
		DualQuaternion dq;
		dq.real = rotation.normalized();
		dq.dual = Quaternion<T>(
						  translation.x * T(0.5),
						  translation.y * T(0.5),
						  translation.z * T(0.5),
						  T(0)) *
				dq.real;
		return dq;
	}

	static DualQuaternion from_transform(const Matrix4<T> &transform) {
		Vector3<T> translation = transform.translation();
		Matrix3<T> rotation_matrix = transform.to_matrix3();
		Quaternion<T> rotation = Quaternion<T>::from_matrix(rotation_matrix);
		return from_rotation_translation(rotation, translation);
	}

	[[nodiscard]] Matrix4<T> to_matrix4() const {
		DualQuaternion n = normalized();
		Quaternion<T> r = n.real;
		Quaternion<T> d = n.dual;

		T xx = r.x * r.x, xy = r.x * r.y, xz = r.x * r.z;
		T yy = r.y * r.y, yz = r.y * r.z, zz = r.z * r.z;
		T wx = r.w * r.x, wy = r.w * r.y, wz = r.w * r.z;

		Quaternion<T> t_quat = d * r.conjugate();
		T tx = T(2) * t_quat.x;
		T ty = T(2) * t_quat.y;
		T tz = T(2) * t_quat.z;

		Matrix4<T> result;
		result[0][0] = T(1) - T(2) * (yy + zz);
		result[0][1] = T(2) * (xy + wz);
		result[0][2] = T(2) * (xz - wy);
		result[0][3] = tx;

		result[1][0] = T(2) * (xy - wz);
		result[1][1] = T(1) - T(2) * (xx + zz);
		result[1][2] = T(2) * (yz + wx);
		result[1][3] = ty;

		result[2][0] = T(2) * (xz + wy);
		result[2][1] = T(2) * (yz - wx);
		result[2][2] = T(1) - T(2) * (xx + yy);
		result[2][3] = tz;

		result[3][0] = T(0);
		result[3][1] = T(0);
		result[3][2] = T(0);
		result[3][3] = T(1);

		return result;
	}

	[[nodiscard]] DualQuaternion normalized() const {
		T len = std::sqrt(real.x * real.x + real.y * real.y + real.z * real.z + real.w * real.w);
		if (len < Epsilon<T>::value) {
			return DualQuaternion();
		}
		return DualQuaternion(
				Quaternion<T>(real.x / len, real.y / len, real.z / len, real.w / len),
				Quaternion<T>(dual.x / len, dual.y / len, dual.z / len, dual.w / len));
	}

	DualQuaternion &normalize() {
		T len = std::sqrt(real.x * real.x + real.y * real.y + real.z * real.z + real.w * real.w);
		if (len >= Epsilon<T>::value) {
			real.x /= len;
			real.y /= len;
			real.z /= len;
			real.w /= len;
			dual.x /= len;
			dual.y /= len;
			dual.z /= len;
			dual.w /= len;
		}
		return *this;
	}

	[[nodiscard]] Vector3<T> translation() const {
		Quaternion<T> t_quat = dual * real.conjugate();
		return Vector3<T>(T(2) * t_quat.x, T(2) * t_quat.y, T(2) * t_quat.z);
	}

	[[nodiscard]] DualQuaternion operator+(const DualQuaternion &rhs) const {
		return DualQuaternion(real + rhs.real, dual + rhs.dual);
	}

	DualQuaternion &operator+=(const DualQuaternion &rhs) {
		real += rhs.real;
		dual += rhs.dual;
		return *this;
	}

	[[nodiscard]] DualQuaternion operator-(const DualQuaternion &rhs) const {
		return DualQuaternion(real - rhs.real, dual - rhs.dual);
	}

	DualQuaternion &operator-=(const DualQuaternion &rhs) {
		real -= rhs.real;
		dual -= rhs.dual;
		return *this;
	}

	[[nodiscard]] DualQuaternion operator*(const DualQuaternion &rhs) const {
		return DualQuaternion(
				real * rhs.real,
				real * rhs.dual + dual * rhs.real);
	}

	DualQuaternion &operator*=(const DualQuaternion &rhs) {
		Quaternion<T> new_real = real * rhs.real;
		dual = real * rhs.dual + dual * rhs.real;
		real = new_real;
		return *this;
	}

	[[nodiscard]] DualQuaternion operator*(T scalar) const {
		return DualQuaternion(real * scalar, dual * scalar);
	}

	DualQuaternion &operator*=(T scalar) {
		real *= scalar;
		dual *= scalar;
		return *this;
	}

	[[nodiscard]] DualQuaternion conjugate() const {
		return DualQuaternion(real.conjugate(), dual.conjugate());
	}

	[[nodiscard]] DualQuaternion inverse() const {
		DualQuaternion conj = conjugate();
		T len_sq = real.x * real.x + real.y * real.y + real.z * real.z + real.w * real.w;
		if (len_sq < Epsilon<T>::value) {
			return DualQuaternion();
		}
		T inv_len_sq = T(1) / len_sq;
		return DualQuaternion(
				Quaternion<T>(conj.real.x * inv_len_sq, conj.real.y * inv_len_sq, conj.real.z * inv_len_sq, conj.real.w * inv_len_sq),
				Quaternion<T>(conj.dual.x * inv_len_sq, conj.dual.y * inv_len_sq, conj.dual.z * inv_len_sq, conj.dual.w * inv_len_sq));
	}

	[[nodiscard]] bool operator==(const DualQuaternion &rhs) const {
		return real == rhs.real && dual == rhs.dual;
	}

	[[nodiscard]] bool operator!=(const DualQuaternion &rhs) const {
		return !(*this == rhs);
	}

	[[nodiscard]] T dot(const DualQuaternion &rhs) const {
		return real.dot(rhs.real);
	}

	[[nodiscard]] T length_squared() const {
		return dot(*this);
	}

	[[nodiscard]] T length() const {
		return std::sqrt(length_squared());
	}

	[[nodiscard]] static DualQuaternion slerp(const DualQuaternion &a, const DualQuaternion &b, T t) {
		T cos_theta = a.real.dot(b.real);
		cos_theta = std::max(T(-1), std::min(T(1), cos_theta));
		T theta = std::acos(cos_theta);

		if (theta < Epsilon<T>::value) {
			return a * (T(1) - t) + b * t;
		}

		T sin_theta = std::sin(theta);
		T wa = std::sin((T(1) - t) * theta) / sin_theta;
		T wb = std::sin(t * theta) / sin_theta;

		return DualQuaternion(
				Quaternion<T>(
						wa * a.real.x + wb * b.real.x,
						wa * a.real.y + wb * b.real.y,
						wa * a.real.z + wb * b.real.z,
						wa * a.real.w + wb * b.real.w),
				Quaternion<T>(
						wa * a.dual.x + wb * b.dual.x,
						wa * a.dual.y + wb * b.dual.y,
						wa * a.dual.z + wb * b.dual.z,
						wa * a.dual.w + wb * b.dual.w));
	}

	[[nodiscard]] static DualQuaternion identity() {
		return DualQuaternion(Quaternion<T>::identity(), Quaternion<T>(T(0), T(0), T(0), T(0)));
	}
};

using DualQuaternionf = DualQuaternion<float>;
using DualQuaterniond = DualQuaternion<double>;





namespace simd {

#if MML_SIMD_SSE2 && defined(__SSE2__)

inline Vector4f add(const Vector4f &a, const Vector4f &b) {
	__m128 av = _mm_setr_ps(a.x, a.y, a.z, a.w);
	__m128 bv = _mm_setr_ps(b.x, b.y, b.z, b.w);
	__m128 result = _mm_add_ps(av, bv);
	alignas(16) float data[4];
	_mm_store_ps(data, result);
	return Vector4f(data[0], data[1], data[2], data[3]);
}

inline Vector4f sub(const Vector4f &a, const Vector4f &b) {
	__m128 av = _mm_setr_ps(a.x, a.y, a.z, a.w);
	__m128 bv = _mm_setr_ps(b.x, b.y, b.z, b.w);
	__m128 result = _mm_sub_ps(av, bv);
	alignas(16) float data[4];
	_mm_store_ps(data, result);
	return Vector4f(data[0], data[1], data[2], data[3]);
}

inline Vector4f mul(const Vector4f &a, const Vector4f &b) {
	__m128 av = _mm_setr_ps(a.x, a.y, a.z, a.w);
	__m128 bv = _mm_setr_ps(b.x, b.y, b.z, b.w);
	__m128 result = _mm_mul_ps(av, bv);
	alignas(16) float data[4];
	_mm_store_ps(data, result);
	return Vector4f(data[0], data[1], data[2], data[3]);
}

inline Vector4f mul_scalar(const Vector4f &v, float scalar) {
	__m128 av = _mm_setr_ps(v.x, v.y, v.z, v.w);
	__m128 result = _mm_mul_ps(av, _mm_set1_ps(scalar));
	alignas(16) float data[4];
	_mm_store_ps(data, result);
	return Vector4f(data[0], data[1], data[2], data[3]);
}

inline float dot4(const Vector4f &a, const Vector4f &b) {
#if MML_SIMD_SSE4_1
	__m128 av = _mm_setr_ps(a.x, a.y, a.z, a.w);
	__m128 bv = _mm_setr_ps(b.x, b.y, b.z, b.w);
	return _mm_cvtss_f32(_mm_dp_ps(av, bv, 0xF1));
#else
	__m128 av = _mm_setr_ps(a.x, a.y, a.z, a.w);
	__m128 bv = _mm_setr_ps(b.x, b.y, b.z, b.w);
	__m128 mul = _mm_mul_ps(av, bv);
	__m128 shuf = _mm_shuffle_ps(mul, mul, _MM_SHUFFLE(1, 0, 1, 0));
	__m128 sum = _mm_add_ss(mul, shuf);
	shuf = _mm_shuffle_ps(sum, sum, _MM_SHUFFLE(3, 2, 3, 2));
	sum = _mm_add_ss(sum, shuf);
	return _mm_cvtss_f32(sum);
#endif
}

inline float dot3(const Vector4f &a, const Vector4f &b) {
#if MML_SIMD_SSE4_1
	__m128 av = _mm_setr_ps(a.x, a.y, a.z, a.w);
	__m128 bv = _mm_setr_ps(b.x, b.y, b.z, b.w);
	return _mm_cvtss_f32(_mm_dp_ps(av, bv, 0x71));
#else
	__m128 av = _mm_setr_ps(a.x, a.y, a.z, a.w);
	__m128 bv = _mm_setr_ps(b.x, b.y, b.z, b.w);
	__m128 mul = _mm_mul_ps(av, bv);
	__m128 shuf = _mm_shuffle_ps(mul, mul, _MM_SHUFFLE(1, 0, 1, 0));
	return _mm_cvtss_f32(_mm_add_ss(mul, shuf));
#endif
}

inline float length_squared4(const Vector4f &v) {
	return dot4(v, v);
}

inline float length_squared3(const Vector4f &v) {
	return dot3(v, v);
}

inline Vector4f min(const Vector4f &a, const Vector4f &b) {
	__m128 av = _mm_setr_ps(a.x, a.y, a.z, a.w);
	__m128 bv = _mm_setr_ps(b.x, b.y, b.z, b.w);
	__m128 result = _mm_min_ps(av, bv);
	alignas(16) float data[4];
	_mm_store_ps(data, result);
	return Vector4f(data[0], data[1], data[2], data[3]);
}

inline Vector4f max(const Vector4f &a, const Vector4f &b) {
	__m128 av = _mm_setr_ps(a.x, a.y, a.z, a.w);
	__m128 bv = _mm_setr_ps(b.x, b.y, b.z, b.w);
	__m128 result = _mm_max_ps(av, bv);
	alignas(16) float data[4];
	_mm_store_ps(data, result);
	return Vector4f(data[0], data[1], data[2], data[3]);
}

inline Vector4f abs(const Vector4f &v) {
	__m128 av = _mm_setr_ps(v.x, v.y, v.z, v.w);
	__m128 mask = _mm_castsi128_ps(_mm_set1_epi32(0x7FFFFFFF));
	__m128 result = _mm_and_ps(av, mask);
	alignas(16) float data[4];
	_mm_store_ps(data, result);
	return Vector4f(data[0], data[1], data[2], data[3]);
}

inline Vector4f lerp(const Vector4f &a, const Vector4f &b, float t) {
	__m128 av = _mm_setr_ps(a.x, a.y, a.z, a.w);
	__m128 bv = _mm_setr_ps(b.x, b.y, b.z, b.w);
	__m128 diff = _mm_sub_ps(bv, av);
	__m128 result = _mm_add_ps(av, _mm_mul_ps(diff, _mm_set1_ps(t)));
	alignas(16) float data[4];
	_mm_store_ps(data, result);
	return Vector4f(data[0], data[1], data[2], data[3]);
}

namespace batch {

inline void add_arrays(Vector4f *result, const Vector4f *a, const Vector4f *b, size_t count) {
	for (size_t i = 0; i < count; ++i) {
		__m128 av = _mm_loadu_ps(&a[i].x);
		__m128 bv = _mm_loadu_ps(&b[i].x);
		__m128 rv = _mm_add_ps(av, bv);
		_mm_storeu_ps(&result[i].x, rv);
	}
}

inline void sub_arrays(Vector4f *result, const Vector4f *a, const Vector4f *b, size_t count) {
	for (size_t i = 0; i < count; ++i) {
		__m128 av = _mm_loadu_ps(&a[i].x);
		__m128 bv = _mm_loadu_ps(&b[i].x);
		__m128 rv = _mm_sub_ps(av, bv);
		_mm_storeu_ps(&result[i].x, rv);
	}
}

inline void mul_scalar_arrays(Vector4f *result, const Vector4f *v, float scalar, size_t count) {
	__m128 sv = _mm_set1_ps(scalar);
	for (size_t i = 0; i < count; ++i) {
		__m128 av = _mm_loadu_ps(&v[i].x);
		__m128 rv = _mm_mul_ps(av, sv);
		_mm_storeu_ps(&result[i].x, rv);
	}
}

inline void dot4_arrays(float *result, const Vector4f *a, const Vector4f *b, size_t count) {
	for (size_t i = 0; i < count; ++i) {
		__m128 av = _mm_loadu_ps(&a[i].x);
		__m128 bv = _mm_loadu_ps(&b[i].x);
#if MML_SIMD_SSE4_1
		result[i] = _mm_cvtss_f32(_mm_dp_ps(av, bv, 0xF1));
#else
		__m128 mul = _mm_mul_ps(av, bv);
		__m128 shuf = _mm_shuffle_ps(mul, mul, _MM_SHUFFLE(1, 0, 1, 0));
		__m128 sum = _mm_add_ss(mul, shuf);
		shuf = _mm_shuffle_ps(sum, sum, _MM_SHUFFLE(3, 2, 3, 2));
		result[i] = _mm_cvtss_f32(_mm_add_ss(sum, shuf));
#endif
	}
}

inline void dot3_arrays(float *result, const Vector4f *a, const Vector4f *b, size_t count) {
	for (size_t i = 0; i < count; ++i) {
		__m128 av = _mm_loadu_ps(&a[i].x);
		__m128 bv = _mm_loadu_ps(&b[i].x);
#if MML_SIMD_SSE4_1
		result[i] = _mm_cvtss_f32(_mm_dp_ps(av, bv, 0x71));
#else
		__m128 mul = _mm_mul_ps(av, bv);
		__m128 shuf = _mm_shuffle_ps(mul, mul, _MM_SHUFFLE(1, 0, 1, 0));
		result[i] = _mm_cvtss_f32(_mm_add_ss(mul, shuf));
#endif
	}
}

inline void length_squared4_arrays(float *result, const Vector4f *v, size_t count) {
	for (size_t i = 0; i < count; ++i) {
		__m128 av = _mm_loadu_ps(&v[i].x);
#if MML_SIMD_SSE4_1
		result[i] = _mm_cvtss_f32(_mm_dp_ps(av, av, 0xF1));
#else
		__m128 mul = _mm_mul_ps(av, av);
		__m128 shuf = _mm_shuffle_ps(mul, mul, _MM_SHUFFLE(1, 0, 1, 0));
		__m128 sum = _mm_add_ss(mul, shuf);
		shuf = _mm_shuffle_ps(sum, sum, _MM_SHUFFLE(3, 2, 3, 2));
		result[i] = _mm_cvtss_f32(_mm_add_ss(sum, shuf));
#endif
	}
}

inline void length_squared3_arrays(float *result, const Vector4f *v, size_t count) {
	for (size_t i = 0; i < count; ++i) {
		__m128 av = _mm_loadu_ps(&v[i].x);
#if MML_SIMD_SSE4_1
		result[i] = _mm_cvtss_f32(_mm_dp_ps(av, av, 0x71));
#else
		__m128 mul = _mm_mul_ps(av, av);
		__m128 shuf = _mm_shuffle_ps(mul, mul, _MM_SHUFFLE(1, 0, 1, 0));
		result[i] = _mm_cvtss_f32(_mm_add_ss(mul, shuf));
#endif
	}
}

inline void min_arrays(Vector4f *result, const Vector4f *a, const Vector4f *b, size_t count) {
	for (size_t i = 0; i < count; ++i) {
		__m128 av = _mm_loadu_ps(&a[i].x);
		__m128 bv = _mm_loadu_ps(&b[i].x);
		__m128 rv = _mm_min_ps(av, bv);
		_mm_storeu_ps(&result[i].x, rv);
	}
}

inline void max_arrays(Vector4f *result, const Vector4f *a, const Vector4f *b, size_t count) {
	for (size_t i = 0; i < count; ++i) {
		__m128 av = _mm_loadu_ps(&a[i].x);
		__m128 bv = _mm_loadu_ps(&b[i].x);
		__m128 rv = _mm_max_ps(av, bv);
		_mm_storeu_ps(&result[i].x, rv);
	}
}

inline void abs_arrays(Vector4f *result, const Vector4f *v, size_t count) {
	__m128 mask = _mm_castsi128_ps(_mm_set1_epi32(0x7FFFFFFF));
	for (size_t i = 0; i < count; ++i) {
		__m128 av = _mm_loadu_ps(&v[i].x);
		__m128 rv = _mm_and_ps(av, mask);
		_mm_storeu_ps(&result[i].x, rv);
	}
}

inline void lerp_arrays(Vector4f *result, const Vector4f *a, const Vector4f *b, float t, size_t count) {
	__m128 tv = _mm_set1_ps(t);
	for (size_t i = 0; i < count; ++i) {
		__m128 av = _mm_loadu_ps(&a[i].x);
		__m128 bv = _mm_loadu_ps(&b[i].x);
		__m128 diff = _mm_sub_ps(bv, av);
		__m128 rv = _mm_add_ps(av, _mm_mul_ps(diff, tv));
		_mm_storeu_ps(&result[i].x, rv);
	}
}

} 

#else

inline Vector4f add(const Vector4f &a, const Vector4f &b) {
	return Vector4f(a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w);
}

inline Vector4f sub(const Vector4f &a, const Vector4f &b) {
	return Vector4f(a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w);
}

inline Vector4f mul(const Vector4f &a, const Vector4f &b) {
	return Vector4f(a.x * b.x, a.y * b.y, a.z * b.z, a.w * b.w);
}

inline Vector4f mul_scalar(const Vector4f &v, float scalar) {
	return Vector4f(v.x * scalar, v.y * scalar, v.z * scalar, v.w * scalar);
}

inline float dot4(const Vector4f &a, const Vector4f &b) {
	return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
}

inline float dot3(const Vector4f &a, const Vector4f &b) {
	return a.x * b.x + a.y * b.y + a.z * b.z;
}

inline float length_squared4(const Vector4f &v) {
	return dot4(v, v);
}

inline float length_squared3(const Vector4f &v) {
	return dot3(v, v);
}

inline Vector4f min(const Vector4f &a, const Vector4f &b) {
	return Vector4f(
			std::min(a.x, b.x),
			std::min(a.y, b.y),
			std::min(a.z, b.z),
			std::min(a.w, b.w));
}

inline Vector4f max(const Vector4f &a, const Vector4f &b) {
	return Vector4f(
			std::max(a.x, b.x),
			std::max(a.y, b.y),
			std::max(a.z, b.z),
			std::max(a.w, b.w));
}

inline Vector4f abs(const Vector4f &v) {
	return Vector4f(
			v.x < 0 ? -v.x : v.x,
			v.y < 0 ? -v.y : v.y,
			v.z < 0 ? -v.z : v.z,
			v.w < 0 ? -v.w : v.w);
}

inline Vector4f lerp(const Vector4f &a, const Vector4f &b, float t) {
	return Vector4f(
			a.x + (b.x - a.x) * t,
			a.y + (b.y - a.y) * t,
			a.z + (b.z - a.z) * t,
			a.w + (b.w - a.w) * t);
}

namespace batch {

inline void add_arrays(Vector4f *result, const Vector4f *a, const Vector4f *b, size_t count) {
	for (size_t i = 0; i < count; ++i) {
		result[i] = a[i] + b[i];
	}
}

inline void sub_arrays(Vector4f *result, const Vector4f *a, const Vector4f *b, size_t count) {
	for (size_t i = 0; i < count; ++i) {
		result[i] = a[i] - b[i];
	}
}

inline void mul_scalar_arrays(Vector4f *result, const Vector4f *v, float scalar, size_t count) {
	for (size_t i = 0; i < count; ++i) {
		result[i] = v[i] * scalar;
	}
}

inline void dot4_arrays(float *result, const Vector4f *a, const Vector4f *b, size_t count) {
	for (size_t i = 0; i < count; ++i) {
		result[i] = a[i].dot(b[i]);
	}
}

inline void dot3_arrays(float *result, const Vector4f *a, const Vector4f *b, size_t count) {
	for (size_t i = 0; i < count; ++i) {
		result[i] = a[i][0] * b[i][0] + a[i][1] * b[i][1] + a[i][2] * b[i][2];
	}
}

inline void length_squared4_arrays(float *result, const Vector4f *v, size_t count) {
	for (size_t i = 0; i < count; ++i) {
		result[i] = v[i].dot(v[i]);
	}
}

inline void length_squared3_arrays(float *result, const Vector4f *v, size_t count) {
	for (size_t i = 0; i < count; ++i) {
		result[i] = v[i].x * v[i].x + v[i].y * v[i].y + v[i].z * v[i].z;
	}
}

inline void min_arrays(Vector4f *result, const Vector4f *a, const Vector4f *b, size_t count) {
	for (size_t i = 0; i < count; ++i) {
		result[i] = Vector4f(
				std::min(a[i].x, b[i].x),
				std::min(a[i].y, b[i].y),
				std::min(a[i].z, b[i].z),
				std::min(a[i].w, b[i].w));
	}
}

inline void max_arrays(Vector4f *result, const Vector4f *a, const Vector4f *b, size_t count) {
	for (size_t i = 0; i < count; ++i) {
		result[i] = Vector4f(
				std::max(a[i].x, b[i].x),
				std::max(a[i].y, b[i].y),
				std::max(a[i].z, b[i].z),
				std::max(a[i].w, b[i].w));
	}
}

inline void abs_arrays(Vector4f *result, const Vector4f *v, size_t count) {
	for (size_t i = 0; i < count; ++i) {
		result[i] = Vector4f(
				v[i].x < 0 ? -v[i].x : v[i].x,
				v[i].y < 0 ? -v[i].y : v[i].y,
				v[i].z < 0 ? -v[i].z : v[i].z,
				v[i].w < 0 ? -v[i].w : v[i].w);
	}
}

inline void lerp_arrays(Vector4f *result, const Vector4f *a, const Vector4f *b, float t, size_t count) {
	for (size_t i = 0; i < count; ++i) {
		result[i] = a[i].lerp(b[i], t);
	}
}

} 

#endif

} 





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

template <typename T>
struct BezierQuadratic {
	static_assert(std::is_floating_point_v<T>, "BezierQuadratic requires floating-point type");

	std::array<Vector3<T>, 3> points;

	BezierQuadratic() = default;

	BezierQuadratic(const Vector3<T> &p0, const Vector3<T> &p1, const Vector3<T> &p2) :
			points{ p0, p1, p2 } {}

	[[nodiscard]] Vector3<T> evaluate(T t) const {
		T u = T(1) - t;
		T tt = t * t;
		T uu = u * u;

		return points[0] * uu + points[1] * (T(2) * u * t) + points[2] * tt;
	}

	[[nodiscard]] Vector3<T> tangent(T t) const {
		return (points[1] - points[0]) * (T(2) * (T(1) - t)) +
				(points[2] - points[1]) * (T(2) * t);
	}

	[[nodiscard]] Vector3<T> second_derivative() const {
		return (points[2] - points[1] * T(2) + points[0]) * T(2);
	}

	[[nodiscard]] std::pair<BezierQuadratic, BezierQuadratic> subdivide(T t) const {
		Vector3<T> p01 = points[0].lerp(points[1], t);
		Vector3<T> p12 = points[1].lerp(points[2], t);
		Vector3<T> p012 = p01.lerp(p12, t);

		return {
			BezierQuadratic(points[0], p01, p012),
			BezierQuadratic(p012, p12, points[2])
		};
	}

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

	[[nodiscard]] const Vector3<T> &operator[](size_t i) const { return points[i]; }
	Vector3<T> &operator[](size_t i) { return points[i]; }

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

template <typename T>
struct BezierCubic {
	static_assert(std::is_floating_point_v<T>, "BezierCubic requires floating-point type");

	std::array<Vector3<T>, 4> points;

	BezierCubic() = default;

	BezierCubic(const Vector3<T> &p0, const Vector3<T> &p1,
			const Vector3<T> &p2, const Vector3<T> &p3) :
			points{ p0, p1, p2, p3 } {}

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

	[[nodiscard]] Vector3<T> tangent(T t) const {
		T u = T(1) - t;

		return (points[1] - points[0]) * (T(3) * u * u) +
				(points[2] - points[1]) * (T(6) * u * t) +
				(points[3] - points[2]) * (T(3) * t * t);
	}

	[[nodiscard]] Vector3<T> second_derivative(T t) const {
		return (points[2] - points[1] * T(2) + points[0]) * (T(6) * (T(1) - t)) +
				(points[3] - points[2] * T(2) + points[1]) * (T(6) * t);
	}

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

	[[nodiscard]] Vector3<T> normal(T t) const {
		Vector3<T> tan = tangent(t);
		Vector3<T> d2 = second_derivative(t);

		Vector3<T> bin = tan.cross(d2);
		if (bin.length() < Epsilon<T>::value) {
			return Vector3<T>::up();
		}
		return bin.cross(tan).normalized();
	}

	[[nodiscard]] CurveFrame<T> frame(T t) const {
		Vector3<T> pos = evaluate(t);
		Vector3<T> tan = tangent(t);
		Vector3<T> norm = normal(t);
		Vector3<T> bin = tan.cross(norm);

		return CurveFrame<T>(pos, tan, norm, bin);
	}

	[[nodiscard]] const Vector3<T> &operator[](size_t i) const { return points[i]; }
	Vector3<T> &operator[](size_t i) { return points[i]; }

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

	[[nodiscard]] Vector3<T> evaluate_segment(size_t segment, T t) const {
		if (points.size() < 2) {
			return points.empty() ? Vector3<T>::zero() : points[0];
		}

		size_t n = points.size();

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

		T tt = t * t;
		T ttt = tt * t;

		return p0 * (-T(0.5) * t + tt - T(0.5) * ttt) +
				p1 * (T(1) - T(2.5) * tt + T(1.5) * ttt) +
				p2 * (T(0.5) * t + T(2) * tt - T(1.5) * ttt) +
				p3 * (-T(0.5) * tt + T(0.5) * ttt);
	}

	[[nodiscard]] Vector3<T> tangent(T t) const {
		auto [segment, local_t] = get_segment(t);

		T dt = T(0.001);
		Vector3<T> p_before = evaluate_segment(segment, Math<T>::clamp(local_t - dt, T(0), T(1)));
		Vector3<T> p_after = evaluate_segment(segment, Math<T>::clamp(local_t + dt, T(0), T(1)));

		return (p_after - p_before).normalized();
	}

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

	[[nodiscard]] std::vector<Vector3<T>> sample_uniform(size_t num_samples) const {
		std::vector<Vector3<T>> result;
		result.reserve(num_samples);

		for (size_t i = 0; i < num_samples; ++i) {
			T t = T(i) / T(num_samples - 1);
			result.push_back(evaluate(t));
		}

		return result;
	}

	[[nodiscard]] Vector3<T> normal(T t) const {
		Vector3<T> tan = tangent(t);
		auto [segment, local_t] = get_segment(t);

		T dt = T(0.001);
		Vector3<T> p_before = evaluate_segment(segment, Math<T>::clamp(local_t - dt, T(0), T(1)));
		Vector3<T> p_mid = evaluate_segment(segment, local_t);
		Vector3<T> p_after = evaluate_segment(segment, Math<T>::clamp(local_t + dt, T(0), T(1)));
		Vector3<T> d2 = (p_after - p_mid * T(2) + p_before).normalized();

		Vector3<T> bin = tan.cross(d2);
		if (bin.length() < Epsilon<T>::value) {
			return Vector3<T>::up();
		}
		return bin.cross(tan).normalized();
	}

	[[nodiscard]] CurveFrame<T> frame(T t) const {
		Vector3<T> pos = evaluate(t);
		Vector3<T> tan = tangent(t);
		Vector3<T> norm = normal(t);
		Vector3<T> bin = tan.cross(norm);

		return CurveFrame<T>(pos, tan, norm, bin);
	}
};

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

	[[nodiscard]] Vector3<T> evaluate_segment(size_t segment, T t) const {
		if (segment >= points.size() - 1) {
			return points.back().position;
		}

		const auto &p0 = points[segment];
		const auto &p1 = points[segment + 1];

		T tt = t * t;
		T ttt = tt * t;

		T h00 = T(2) * ttt - T(3) * tt + T(1);
		T h10 = ttt - T(2) * tt + t;
		T h01 = -T(2) * ttt + T(3) * tt;
		T h11 = ttt - tt;

		return p0.position * h00 + p0.tangent * h10 +
				p1.position * h01 + p1.tangent * h11;
	}

	[[nodiscard]] Vector3<T> tangent(T t) const {
		auto [segment, local_t] = get_segment(t);

		if (segment >= points.size() - 1) {
			return points.back().tangent;
		}

		const auto &p0 = points[segment];
		const auto &p1 = points[segment + 1];

		T tt = local_t * local_t;

		T dh00 = T(6) * tt - T(6) * local_t;
		T dh10 = T(3) * tt - T(4) * local_t + T(1);
		T dh01 = -T(6) * tt + T(6) * local_t;
		T dh11 = T(3) * tt - T(2) * local_t;

		return (p0.position * dh00 + p0.tangent * dh10 +
				p1.position * dh01 + p1.tangent * dh11)
				.normalized();
	}

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

	[[nodiscard]] Vector3<T> normal(T t) const {
		Vector3<T> tan = tangent(t);
		Vector3<T> d2 = second_derivative(t);

		Vector3<T> bin = tan.cross(d2);
		if (bin.length() < Epsilon<T>::value) {
			return Vector3<T>::up();
		}
		return bin.cross(tan).normalized();
	}

	[[nodiscard]] CurveFrame<T> frame(T t) const {
		Vector3<T> pos = evaluate(t);
		Vector3<T> tan = tangent(t);
		Vector3<T> norm = normal(t);
		Vector3<T> bin = tan.cross(norm);

		return CurveFrame<T>(pos, tan, norm, bin);
	}

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

using BezierQuadraticf = BezierQuadratic<float>;
using BezierQuadraticd = BezierQuadratic<double>;
using BezierCubicf = BezierCubic<float>;
using BezierCubicd = BezierCubic<double>;
using CatmullRomSplinef = CatmullRomSpline<float>;
using CatmullRomSplined = CatmullRomSpline<double>;
using HermiteSplinef = HermiteSpline<float>;
using HermiteSplined = HermiteSpline<double>;






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
			T span = knots[i + static_cast<size_t>(degree) + 1] - knots[i + 1];
			T factor = Epsilon<T>::approx_zero(span) ? T(0) : T(degree) / span;
			Vector3<T> p1 = control_points[i] * weights[i];
			Vector3<T> p2 = control_points[i + 1] * weights[i + 1];
			Vector3<T> diff = (p2 - p1) * factor;
			T w_diff = (weights[i + 1] - weights[i]) * factor;

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






template <typename T>
struct Ray {
	static_assert(std::is_floating_point_v<T>, "Ray requires floating-point type");

	Vector3<T> origin;
	Vector3<T> direction;

	Ray() = default;

	explicit Ray(const Vector3<T> &origin_, const Vector3<T> &direction_) :
			origin(origin_), direction(direction_.normalized()) {}

	Ray(const Ray &other) = default;
	Ray &operator=(const Ray &other) = default;

	MML_FORCE_INLINE Vector3<T> at(T t) const {
		return origin + direction * t;
	}

	MML_FORCE_INLINE Vector3<T> point_at(T t) const {
		return at(t);
	}
};

using Rayf = Ray<float>;
using Rayd = Ray<double>;






template <typename T>
struct AABB {
	static_assert(std::is_floating_point_v<T>, "AABB requires floating-point type");

	Vector3<T> min;
	Vector3<T> max;

	AABB() = default;

	constexpr AABB(const Vector3<T> &min_, const Vector3<T> &max_) :
			min(min_), max(max_) {}

	static constexpr AABB from_center_extents(const Vector3<T> &center, const Vector3<T> &half_extents) {
		return AABB(center - half_extents, center + half_extents);
	}

	static constexpr AABB empty() {
		return AABB(
				Vector3<T>(std::numeric_limits<T>::max(), std::numeric_limits<T>::max(), std::numeric_limits<T>::max()),
				Vector3<T>(std::numeric_limits<T>::lowest(), std::numeric_limits<T>::lowest(), std::numeric_limits<T>::lowest()));
	}

	constexpr bool is_empty() const {
		return min.x > max.x || min.y > max.y || min.z > max.z;
	}

	AABB(const AABB &other) = default;
	AABB &operator=(const AABB &other) = default;

	constexpr MML_FORCE_INLINE Vector3<T> center() const {
		return (min + max) * T(0.5);
	}

	constexpr MML_FORCE_INLINE Vector3<T> half_extents() const {
		return (max - min) * T(0.5);
	}

	constexpr MML_FORCE_INLINE Vector3<T> extents() const {
		return max - min;
	}

	constexpr MML_FORCE_INLINE T volume() const {
		Vector3<T> size = extents();
		return size.x * size.y * size.z;
	}

	constexpr MML_FORCE_INLINE T surface_area() const {
		Vector3<T> size = extents();
		return T(2) * (size.x * size.y + size.y * size.z + size.z * size.x);
	}

	constexpr MML_FORCE_INLINE bool contains(const Vector3<T> &point) const {
		return point.x >= min.x && point.x <= max.x &&
				point.y >= min.y && point.y <= max.y &&
				point.z >= min.z && point.z <= max.z;
	}

	constexpr MML_FORCE_INLINE bool contains(const AABB &other) const {
		return contains(other.min) && contains(other.max);
	}

	constexpr MML_FORCE_INLINE bool intersects(const AABB &other) const {
		return min.x <= other.max.x && max.x >= other.min.x &&
				min.y <= other.max.y && max.y >= other.min.y &&
				min.z <= other.max.z && max.z >= other.min.z;
	}

	MML_FORCE_INLINE bool intersect_ray(const Ray<T> &ray, T &out_t_near, T &out_t_far) const {
		T t_near = -std::numeric_limits<T>::infinity();
		T t_far = std::numeric_limits<T>::infinity();

		for (size_t i = 0; i < 3; ++i) {
			if (Epsilon<T>::approx_zero(ray.direction[i])) {
				if (ray.origin[i] < min[i] || ray.origin[i] > max[i]) {
					return false;
				}
			} else {
				T inv_d = T(1) / ray.direction[i];
				T t1 = (min[i] - ray.origin[i]) * inv_d;
				T t2 = (max[i] - ray.origin[i]) * inv_d;

				if (t1 > t2) {
					std::swap(t1, t2);
				}

				t_near = std::max(t_near, t1);
				t_far = std::min(t_far, t2);

				if (t_near > t_far) {
					return false;
				}
			}
		}

		out_t_near = t_near;
		out_t_far = t_far;
		return true;
	}

	MML_FORCE_INLINE bool intersect_ray(const Ray<T> &ray, Vector3<T> &out_point) const {
		T t_near, t_far;
		if (intersect_ray(ray, t_near, t_far)) {
			T t = (t_near >= T(0)) ? t_near : t_far;
			if (t >= T(0)) {
				out_point = ray.at(t);
				return true;
			}
		}
		return false;
	}

	MML_FORCE_INLINE void expand(const Vector3<T> &point) {
		min.x = std::min(min.x, point.x);
		min.y = std::min(min.y, point.y);
		min.z = std::min(min.z, point.z);
		max.x = std::max(max.x, point.x);
		max.y = std::max(max.y, point.y);
		max.z = std::max(max.z, point.z);
	}

	MML_FORCE_INLINE void expand(const AABB &other) {
		expand(other.min);
		expand(other.max);
	}

	static constexpr AABB merge(const AABB &a, const AABB &b) {
		return AABB(
				Vector3<T>(
						std::min(a.min.x, b.min.x),
						std::min(a.min.y, b.min.y),
						std::min(a.min.z, b.min.z)),
				Vector3<T>(
						std::max(a.max.x, b.max.x),
						std::max(a.max.y, b.max.y),
						std::max(a.max.z, b.max.z)));
	}

	constexpr MML_FORCE_INLINE Vector3<T> closest_point(const Vector3<T> &point) const {
		return Vector3<T>(
				(point.x < min.x) ? min.x : (point.x > max.x) ? max.x
															  : point.x,
				(point.y < min.y) ? min.y : (point.y > max.y) ? max.y
															  : point.y,
				(point.z < min.z) ? min.z : (point.z > max.z) ? max.z
															  : point.z);
	}

	[[nodiscard]] MML_FORCE_INLINE AABB transformed(const Matrix4<T> &matrix) const {
		Vector3<T> center = (min + max) * T(0.5);
		Vector3<T> extents = (max - min) * T(0.5);

		Vector3<T> new_center = matrix.transform_point(center);

		Vector3<T> new_extents;
		new_extents.x = std::abs(matrix[0][0]) * extents.x + std::abs(matrix[1][0]) * extents.y + std::abs(matrix[2][0]) * extents.z;
		new_extents.y = std::abs(matrix[0][1]) * extents.x + std::abs(matrix[1][1]) * extents.y + std::abs(matrix[2][1]) * extents.z;
		new_extents.z = std::abs(matrix[0][2]) * extents.x + std::abs(matrix[1][2]) * extents.y + std::abs(matrix[2][2]) * extents.z;

		return AABB(new_center - new_extents, new_center + new_extents);
	}

	MML_FORCE_INLINE bool contains_sphere(const Vector3<T> &sphere_center, T sphere_radius) const {
		Vector3<T> r(sphere_radius, sphere_radius, sphere_radius);
		AABB sphere_aabb(sphere_center - r, sphere_center + r);
		return contains(sphere_aabb);
	}

	MML_FORCE_INLINE bool intersects_sphere(const Vector3<T> &sphere_center, T sphere_radius) const {
		Vector3<T> closest = closest_point(sphere_center);
		return sphere_center.distance_squared(closest) <= sphere_radius * sphere_radius;
	}

	friend constexpr MML_FORCE_INLINE AABB operator|(const AABB &a, const AABB &b) {
		return merge(a, b);
	}
};

using AABBf = AABB<float>;
using AABBd = AABB<double>;






template <typename T>
struct Sphere {
	static_assert(std::is_floating_point_v<T>, "Sphere requires floating-point type");

	Vector3<T> center;
	T radius;

	Sphere() = default;

	explicit Sphere(const Vector3<T> &center_, T radius_) :
			center(center_), radius(radius_) {}

	Sphere(const Sphere &other) = default;
	Sphere &operator=(const Sphere &other) = default;

	MML_FORCE_INLINE T volume() const {
		T r3 = radius * radius * radius;
		return (T(4) / T(3)) *
				Constants<T>::pi *
				r3;
	}

	MML_FORCE_INLINE T surface_area() const {
		T r2 = radius * radius;
		return T(4) * Constants<T>::pi * r2;
	}

	MML_FORCE_INLINE bool contains(const Vector3<T> &point) const {
		return center.distance_squared(point) <= radius * radius;
	}

	MML_FORCE_INLINE bool contains(const Sphere &other) const {
		T distance = center.distance(other.center);
		return distance + other.radius <= radius;
	}

	MML_FORCE_INLINE bool intersects(const Sphere &other) const {
		T distance_sq = center.distance_squared(other.center);
		T radius_sum = radius + other.radius;
		return distance_sq <= radius_sum * radius_sum;
	}

	MML_FORCE_INLINE bool intersects(const AABB<T> &aabb) const {
		Vector3<T> closest = aabb.closest_point(center);
		return center.distance_squared(closest) <= radius * radius;
	}

	MML_FORCE_INLINE bool intersect_ray(const Ray<T> &ray, T &out_t1, T &out_t2) const {
		Vector3<T> oc = ray.origin - center;

		T a = ray.direction.dot(ray.direction);
		T b = T(2) * oc.dot(ray.direction);
		T c = oc.dot(oc) - radius * radius;

		T discriminant = b * b - T(4) * a * c;

		if (discriminant < T(0)) {
			return false;
		}

		T sqrt_discriminant = Math<T>::safe_sqrt(discriminant);
		T inv_2a = T(1) / (T(2) * a);

		out_t1 = (-b - sqrt_discriminant) * inv_2a;
		out_t2 = (-b + sqrt_discriminant) * inv_2a;

		return true;
	}

	MML_FORCE_INLINE bool intersect_ray(const Ray<T> &ray, Vector3<T> &out_point) const {
		T t1, t2;
		if (intersect_ray(ray, t1, t2)) {
			T t = (t1 >= T(0)) ? t1 : t2;
			if (t >= T(0)) {
				out_point = ray.at(t);
				return true;
			}
		}
		return false;
	}

	MML_FORCE_INLINE Vector3<T> closest_point(const Vector3<T> &point) const {
		Vector3<T> dir = point - center;
		T dist = dir.length();

		if (Epsilon<T>::approx_zero(dist)) {
			return center + Vector3<T>(radius, T(0), T(0));
		}

		return center + dir * (radius / dist);
	}

	MML_FORCE_INLINE Vector3<T> normal_at(const Vector3<T> &point) const {
		return (point - center).normalized();
	}

	MML_FORCE_INLINE AABB<T> bounding_box() const {
		Vector3<T> r(radius, radius, radius);
		return AABB<T>(center - r, center + r);
	}

	MML_FORCE_INLINE Sphere transformed(const Matrix4<T> &matrix) const {
		Vector3<T> transformed_center = matrix.transform_point(center);

		Vector3<T> axis_x(T(1), T(0), T(0));
		Vector3<T> axis_y(T(0), T(1), T(0));
		Vector3<T> axis_z(T(0), T(0), T(1));

		Vector3<T> scaled_x = matrix.transform_direction(axis_x);
		Vector3<T> scaled_y = matrix.transform_direction(axis_y);
		Vector3<T> scaled_z = matrix.transform_direction(axis_z);

		T max_scale = std::max({ scaled_x.length(), scaled_y.length(), scaled_z.length() });
		T transformed_radius = radius * max_scale;

		return Sphere(transformed_center, transformed_radius);
	}

	MML_FORCE_INLINE Sphere operator|(const Sphere &other) const {
		if (contains(other)) {
			return *this;
		}
		if (other.contains(*this)) {
			return other;
		}

		Vector3<T> delta = other.center - center;
		T dist = delta.length();

		if (dist < Epsilon<T>::value) {
			return Sphere(center, std::max(radius, other.radius));
		}

		Vector3<T> dir = delta / dist;
		Vector3<T> p1 = center - dir * radius;
		Vector3<T> p2 = other.center + dir * other.radius;
		Vector3<T> new_center = (p1 + p2) * T(0.5);
		T new_radius = p2.distance(p1) * T(0.5);

		return Sphere(new_center, new_radius);
	}

	MML_FORCE_INLINE T distance_to(const Vector3<T> &point) const {
		T dist = center.distance(point);
		return std::max(T(0), dist - radius);
	}

	MML_FORCE_INLINE T distance_to(const Sphere &other) const {
		T dist = center.distance(other.center);
		return std::max(T(0), dist - radius - other.radius);
	}
};

using Spheref = Sphere<float>;
using Sphered = Sphere<double>;






template <typename T>
struct BoundingVolumeUtils {
	static constexpr bool is_floating_point_v = std::is_floating_point_v<T>;

	static inline AABB<T> union_aabb(const AABB<T> &a, const AABB<T> &b) {
		return AABB<T>::merge(a, b);
	}

	static inline Sphere<T> union_sphere(const Sphere<T> &a, const Sphere<T> &b) {
		Vector3<T> center = a.center + b.center;
		center = center * T(0.5);
		T distance = a.center.distance(b.center);
		T radius = distance * T(0.5) + std::max(a.radius, b.radius);
		return Sphere<T>(center, radius);
	}

	static inline AABB<T> union_aabb_sphere(const AABB<T> &aabb, const Sphere<T> &sphere) {
		AABB<T> sphere_aabb = sphere.bounding_box();
		return AABB<T>::merge(aabb, sphere_aabb);
	}

	static inline AABB<T> union_sphere_aabb(const Sphere<T> &sphere, const AABB<T> &aabb) {
		return union_aabb_sphere(aabb, sphere);
	}

	static inline Sphere<T> union_sphere_aabb_enclosing(const Sphere<T> &sphere, const AABB<T> &aabb) {
		Vector3<T> center = sphere.center;
		T radius = sphere.radius;
		Vector3<T> corners[8] = {
			aabb.min,
			Vector3<T>(aabb.max.x, aabb.min.y, aabb.min.z),
			Vector3<T>(aabb.min.x, aabb.max.y, aabb.min.z),
			Vector3<T>(aabb.max.x, aabb.max.y, aabb.min.z),
			Vector3<T>(aabb.min.x, aabb.min.y, aabb.max.z),
			Vector3<T>(aabb.max.x, aabb.min.y, aabb.max.z),
			Vector3<T>(aabb.min.x, aabb.max.y, aabb.max.z),
			aabb.max
		};
		for (const auto &corner : corners) {
			T dist = center.distance(corner);
			if (dist > radius) {
				radius = dist;
			}
		}
		return Sphere<T>(center, radius);
	}

	static inline bool intersects_aabb_aabb(const AABB<T> &a, const AABB<T> &b) {
		return a.intersects(b);
	}

	static inline bool intersects_sphere_sphere(const Sphere<T> &a, const Sphere<T> &b) {
		return a.intersects(b);
	}

	static inline bool intersects_aabb_sphere(const AABB<T> &aabb, const Sphere<T> &sphere) {
		return sphere.intersects(aabb);
	}

	static inline bool intersects_sphere_aabb(const Sphere<T> &sphere, const AABB<T> &aabb) {
		return sphere.intersects(aabb);
	}

	static inline bool intersects_sphere_aabb(const AABB<T> &aabb, const Sphere<T> &sphere) {
		return sphere.intersects(aabb);
	}

	static inline AABB<T> intersect_aabb_aabb(const AABB<T> &a, const AABB<T> &b) {
		Vector3<T> min(
				std::max(a.min.x, b.min.x),
				std::max(a.min.y, b.min.y),
				std::max(a.min.z, b.min.z));
		Vector3<T> max(
				std::min(a.max.x, b.max.x),
				std::min(a.max.y, b.max.y),
				std::min(a.max.z, b.max.z));

		if (min.x > max.x || min.y > max.y || min.z > max.z) {
			return AABB<T>::from_center_extents(Vector3<T>(), Vector3<T>());
		}

		return AABB<T>(min, max);
	}

	static inline AABB<T> transform_aabb(const AABB<T> &aabb, const Matrix4<T> &matrix) {
		Vector3<T> corners[8] = {
			Vector3<T>(aabb.min.x, aabb.min.y, aabb.min.z),
			Vector3<T>(aabb.max.x, aabb.min.y, aabb.min.z),
			Vector3<T>(aabb.min.x, aabb.max.y, aabb.min.z),
			Vector3<T>(aabb.max.x, aabb.max.y, aabb.min.z),
			Vector3<T>(aabb.min.x, aabb.min.y, aabb.max.z),
			Vector3<T>(aabb.max.x, aabb.min.y, aabb.max.z),
			Vector3<T>(aabb.min.x, aabb.max.y, aabb.max.z),
			Vector3<T>(aabb.max.x, aabb.max.y, aabb.max.z)
		};

		Vector3<T> transformed_corners[8];
		for (int i = 0; i < 8; ++i) {
			transformed_corners[i] = matrix.transform_point(corners[i]);
		}

		Vector3<T> new_min = transformed_corners[0];
		Vector3<T> new_max = transformed_corners[0];
		for (int i = 1; i < 8; ++i) {
			new_min.x = std::min(new_min.x, transformed_corners[i].x);
			new_min.y = std::min(new_min.y, transformed_corners[i].y);
			new_min.z = std::min(new_min.z, transformed_corners[i].z);
			new_max.x = std::max(new_max.x, transformed_corners[i].x);
			new_max.y = std::max(new_max.y, transformed_corners[i].y);
			new_max.z = std::max(new_max.z, transformed_corners[i].z);
		}

		return AABB<T>(new_min, new_max);
	}

	static inline Sphere<T> transform_sphere(const Sphere<T> &sphere, const Matrix4<T> &matrix) {
		Vector3<T> transformed_center = matrix.transform_point(sphere.center);

		Vector3<T> axis_x(T(1), T(0), T(0));
		Vector3<T> axis_y(T(0), T(1), T(0));
		Vector3<T> axis_z(T(0), T(0), T(1));

		Vector3<T> scaled_x = matrix.transform_direction(axis_x);
		Vector3<T> scaled_y = matrix.transform_direction(axis_y);
		Vector3<T> scaled_z = matrix.transform_direction(axis_z);

		T max_scale = std::max({ scaled_x.length(), scaled_y.length(), scaled_z.length() });
		T transformed_radius = sphere.radius * max_scale;

		return Sphere<T>(transformed_center, transformed_radius);
	}

	static inline AABB<T> transform_aabb_rotation_scale(const AABB<T> &aabb, const Matrix4<T> &matrix) {
		Vector3<T> corners[8] = {
			Vector3<T>(aabb.min.x, aabb.min.y, aabb.min.z),
			Vector3<T>(aabb.max.x, aabb.min.y, aabb.min.z),
			Vector3<T>(aabb.min.x, aabb.max.y, aabb.min.z),
			Vector3<T>(aabb.max.x, aabb.max.y, aabb.min.z),
			Vector3<T>(aabb.min.x, aabb.min.y, aabb.max.z),
			Vector3<T>(aabb.max.x, aabb.min.y, aabb.max.z),
			Vector3<T>(aabb.min.x, aabb.max.y, aabb.max.z),
			Vector3<T>(aabb.max.x, aabb.max.y, aabb.max.z)
		};

		AABB<T> result;
		for (const auto &corner : corners) {
			Vector3<T> transformed = matrix * corner;
			result.expand(transformed);
		}

		return result;
	}

	static inline T distance_aabb_aabb(const AABB<T> &a, const AABB<T> &b) {
		Vector3<T> d(
				std::max(T(0), std::max(a.min.x - b.max.x, b.min.x - a.max.x)),
				std::max(T(0), std::max(a.min.y - b.max.y, b.min.y - a.max.y)),
				std::max(T(0), std::max(a.min.z - b.max.z, b.min.z - a.max.z)));
		return d.length();
	}

	static inline T distance_sphere_sphere(const Sphere<T> &a, const Sphere<T> &b) {
		T dist = a.center.distance(b.center);
		return std::max(T(0), dist - a.radius - b.radius);
	}

	static inline T distance_aabb_sphere(const AABB<T> &aabb, const Sphere<T> &sphere) {
		Vector3<T> closest = aabb.closest_point(sphere.center);
		return std::max(T(0), closest.distance(sphere.center) - sphere.radius);
	}

	static inline T distance_sphere_aabb(const Sphere<T> &sphere, const AABB<T> &aabb) {
		return distance_aabb_sphere(aabb, sphere);
	}

	static inline bool contains_aabb_point(const AABB<T> &aabb, const Vector3<T> &point) {
		return aabb.contains(point);
	}

	static inline bool contains_sphere_point(const Sphere<T> &sphere, const Vector3<T> &point) {
		return sphere.contains(point);
	}

	static inline bool contains_aabb_aabb(const AABB<T> &a, const AABB<T> &b) {
		return a.contains(b);
	}

	static inline bool contains_sphere_sphere(const Sphere<T> &a, const Sphere<T> &b) {
		return a.contains(b);
	}

	static inline bool contains_aabb_sphere(const AABB<T> &aabb, const Sphere<T> &sphere) {
		AABB<T> sphere_aabb = sphere.bounding_box();
		return aabb.contains(sphere_aabb);
	}

	static inline T get_aabb_surface_area(const AABB<T> &aabb) {
		return aabb.surface_area();
	}

	static inline T get_sphere_surface_area(const Sphere<T> &sphere) {
		return sphere.surface_area();
	}

	static inline T get_aabb_volume(const AABB<T> &aabb) {
		return aabb.volume();
	}

	static inline T get_sphere_volume(const Sphere<T> &sphere) {
		return sphere.volume();
	}
};

template <typename T>
AABB<T> operator|(const AABB<T> &a, const AABB<T> &b) {
	return BoundingVolumeUtils<T>::union_aabb(a, b);
}

template <typename T>
Sphere<T> operator|(const Sphere<T> &a, const Sphere<T> &b) {
	return BoundingVolumeUtils<T>::union_sphere(a, b);
}

template <typename T>
AABB<T> transform(const AABB<T> &aabb, const Matrix4<T> &matrix) {
	return BoundingVolumeUtils<T>::transform_aabb(aabb, matrix);
}

template <typename T>
Sphere<T> transform(const Sphere<T> &sphere, const Matrix4<T> &matrix) {
	return BoundingVolumeUtils<T>::transform_sphere(sphere, matrix);
}

template <typename T>
struct BoundingFrustum {
	Vector3<T> planes[6];
	T normals[6][4];

	enum class Plane {
		Left = 0,
		Right = 1,
		Bottom = 2,
		Top = 3,
		Near = 4,
		Far = 5
	};

	BoundingFrustum() = default;

	void extract_from_matrix(const Matrix4<T> &matrix) {
		Vector4<T> rows[4] = {
			Vector4<T>(matrix[0][0], matrix[0][1], matrix[0][2], matrix[0][3]),
			Vector4<T>(matrix[1][0], matrix[1][1], matrix[1][2], matrix[1][3]),
			Vector4<T>(matrix[2][0], matrix[2][1], matrix[2][2], matrix[2][3]),
			Vector4<T>(matrix[3][0], matrix[3][1], matrix[3][2], matrix[3][3])
		};

		normals[0][0] = rows[3].x + rows[0].x;
		normals[0][1] = rows[3].y + rows[0].y;
		normals[0][2] = rows[3].z + rows[0].z;
		normals[0][3] = rows[3].w + rows[0].w;

		normals[1][0] = rows[3].x - rows[0].x;
		normals[1][1] = rows[3].y - rows[0].y;
		normals[1][2] = rows[3].z - rows[0].z;
		normals[1][3] = rows[3].w - rows[0].w;

		normals[2][0] = rows[3].x + rows[1].x;
		normals[2][1] = rows[3].y + rows[1].y;
		normals[2][2] = rows[3].z + rows[1].z;
		normals[2][3] = rows[3].w + rows[1].w;

		normals[3][0] = rows[3].x - rows[1].x;
		normals[3][1] = rows[3].y - rows[1].y;
		normals[3][2] = rows[3].z - rows[1].z;
		normals[3][3] = rows[3].w - rows[1].w;

		normals[4][0] = rows[3].x + rows[2].x;
		normals[4][1] = rows[3].y + rows[2].y;
		normals[4][2] = rows[3].z + rows[2].z;
		normals[4][3] = rows[3].w + rows[2].w;

		normals[5][0] = rows[3].x - rows[2].x;
		normals[5][1] = rows[3].y - rows[2].y;
		normals[5][2] = rows[3].z - rows[2].z;
		normals[5][3] = rows[3].w - rows[2].w;

		for (int i = 0; i < 6; ++i) {
			Vector3<T> normal(normals[i][0], normals[i][1], normals[i][2]);
			T length = normal.length();
			if (length > Epsilon<T>::value) {
				normals[i][0] /= length;
				normals[i][1] /= length;
				normals[i][2] /= length;
				planes[i] = normal;
			}
		}
	}

	bool contains(const Vector3<T> &point) const {
		for (int i = 0; i < 6; ++i) {
			Vector3<T> normal(normals[i][0], normals[i][1], normals[i][2]);
			Vector3<T> offset = point;
			if (normal.dot(offset) + normals[i][3] < T(0)) {
				return false;
			}
		}
		return true;
	}

	bool intersects(const AABB<T> &aabb) const {
		for (int i = 0; i < 6; ++i) {
			Vector3<T> normal(normals[i][0], normals[i][1], normals[i][2]);
			Vector3<T> pos_normal = Vector3<T>(
					normal.x > T(0) ? aabb.max.x : aabb.min.x,
					normal.y > T(0) ? aabb.max.y : aabb.min.y,
					normal.z > T(0) ? aabb.max.z : aabb.min.z);
			if (pos_normal.dot(normal) + normals[i][3] < T(0)) {
				return false;
			}
		}
		return true;
	}

	bool intersects(const Sphere<T> &sphere) const {
		for (int i = 0; i < 6; ++i) {
			Vector3<T> normal(normals[i][0], normals[i][1], normals[i][2]);
			T dist = sphere.center.dot(normal) + normals[i][3];
			if (dist < -sphere.radius) {
				return false;
			}
		}
		return true;
	}

	bool contains(const AABB<T> &aabb) const {
		return intersects(aabb);
	}

	bool contains(const Sphere<T> &sphere) const {
		return intersects(sphere);
	}
};

using BoundingFrustumf = BoundingFrustum<float>;
using BoundingFrustumd = BoundingFrustum<double>;






template <typename T>
struct Capsule {
	static_assert(std::is_floating_point_v<T>, "Capsule requires floating-point type");

	Vector3<T> a;
	Vector3<T> b;
	T radius;

	Capsule() = default;

	explicit Capsule(const Vector3<T> &a_, const Vector3<T> &b_, T radius_) :
			a(a_), b(b_), radius(radius_) {}

	constexpr Capsule(const Capsule &other) = default;
	Capsule &operator=(const Capsule &other) = default;

	MML_FORCE_INLINE Vector3<T> center() const {
		return (a + b) * T(0.5);
	}

	MML_FORCE_INLINE T height() const {
		return a.distance(b);
	}

	MML_FORCE_INLINE T volume() const {
		T h = height();
		T sphere_vol = (T(4) / T(3)) * Constants<T>::pi * radius * radius * radius;
		T cylinder_vol = Constants<T>::pi * radius * radius * h;
		return sphere_vol + cylinder_vol;
	}

	MML_FORCE_INLINE T surface_area() const {
		T h = height();
		T sphere_area = T(4) * Constants<T>::pi * radius * radius;
		T cylinder_area = T(2) * Constants<T>::pi * radius * h;
		return sphere_area + cylinder_area;
	}

	MML_FORCE_INLINE Vector3<T> axis() const {
		return (b - a).normalized();
	}

	MML_FORCE_INLINE bool contains(const Vector3<T> &point) const {
		Vector3<T> ab = b - a;
		Vector3<T> ap = point - a;

		T t = ap.dot(ab) / ab.length_squared();
		t = t < T(0) ? T(0) : (t > T(1) ? T(1) : t);

		Vector3<T> closest = a + ab * t;
		return closest.distance_squared(point) <= radius * radius;
	}

	MML_FORCE_INLINE Vector3<T> closest_point(const Vector3<T> &point) const {
		Vector3<T> ab = b - a;
		Vector3<T> ap = point - a;

		T t = ap.dot(ab) / ab.length_squared();
		t = t < T(0) ? T(0) : (t > T(1) ? T(1) : t);

		Vector3<T> closest = a + ab * t;
		Vector3<T> dir = point - closest;
		T dist = dir.length();

		if (Epsilon<T>::approx_zero(dist)) {
			dir = Vector3<T>::unit_y();
			dist = radius;
		}

		return closest + dir.normalized() * radius;
	}

	MML_FORCE_INLINE T distance_squared(const Vector3<T> &point) const {
		Vector3<T> ab = b - a;
		Vector3<T> ap = point - a;

		T t = ap.dot(ab) / ab.length_squared();
		t = t < T(0) ? T(0) : (t > T(1) ? T(1) : t);

		Vector3<T> closest = a + ab * t;
		T dist_sq = closest.distance_squared(point);
		T dist = Math<T>::safe_sqrt(dist_sq);

		T dist_to_surface = dist - radius;
		return dist_to_surface < T(0) ? T(0) : dist_to_surface * dist_to_surface;
	}

	MML_FORCE_INLINE T distance(const Vector3<T> &point) const {
		return Math<T>::safe_sqrt(distance_squared(point));
	}

	MML_FORCE_INLINE bool intersects(const Sphere<T> &sphere) const {
		Vector3<T> ab = b - a;
		Vector3<T> ap = sphere.center - a;

		T t = ap.dot(ab) / ab.length_squared();
		t = t < T(0) ? T(0) : (t > T(1) ? T(1) : t);

		Vector3<T> closest = a + ab * t;
		T dist_sq = closest.distance_squared(sphere.center);
		T radius_sum = radius + sphere.radius;

		return dist_sq <= radius_sum * radius_sum;
	}

	MML_FORCE_INLINE bool intersects(const AABB<T> &aabb) const {
		T radius_sq = radius * radius;

		if (aabb.contains(a) || aabb.contains(b)) {
			return true;
		}

		Vector3<T> closest = aabb.closest_point(a);
		if ((a - closest).length_squared() <= radius_sq) {
			return true;
		}

		closest = aabb.closest_point(b);
		if ((b - closest).length_squared() <= radius_sq) {
			return true;
		}

		Vector3<T> ab = b - a;
		Vector3<T> test_points[] = {
			aabb.closest_point(a + ab * T(0.25)),
			aabb.closest_point(a + ab * T(0.5)),
			aabb.closest_point(a + ab * T(0.75))
		};

		for (int i = 0; i < 3; ++i) {
			if ((test_points[i] - (a + ab * ((i + 1) * T(0.25)))).length_squared() <= radius_sq) {
				return true;
			}
		}

		return false;
	}

	MML_FORCE_INLINE AABB<T> bounding_box() const {
		AABB<T> box(a, a);
		box.expand(b);
		Vector3<T> r(radius, radius, radius);
		return AABB<T>(box.min - r, box.max + r);
	}

	MML_FORCE_INLINE static Capsule from_center_height_radius(const Vector3<T> &center, T height, T radius_, const Vector3<T> &direction = Vector3<T>::up()) {
		Vector3<T> half_axis = direction.normalized() * (height * T(0.5));
		return Capsule(center - half_axis, center + half_axis, radius_);
	}
};

using Capsulef = Capsule<float>;
using Capsuled = Capsule<double>;






template <typename T>
struct Cone {
	static_assert(std::is_floating_point_v<T>, "Cone requires floating-point type");

	Vector3<T> apex;
	Vector3<T> base_center;
	T base_radius;

	Cone() = default;

	explicit Cone(const Vector3<T> &apex_, const Vector3<T> &base_center_, T radius_) :
			apex(apex_), base_center(base_center_), base_radius(radius_) {}

	constexpr Cone(const Cone &other) = default;
	Cone &operator=(const Cone &other) = default;

	MML_FORCE_INLINE Vector3<T> axis() const {
		return (base_center - apex).normalized();
	}

	MML_FORCE_INLINE T height() const {
		return apex.distance(base_center);
	}

	MML_FORCE_INLINE T volume() const {
		return (T(1) / T(3)) * Constants<T>::pi * base_radius * base_radius * height();
	}

	MML_FORCE_INLINE T surface_area() const {
		T h = height();
		T slant = Math<T>::safe_sqrt(h * h + base_radius * base_radius);
		T base_area = Constants<T>::pi * base_radius * base_radius;
		T side_area = Constants<T>::pi * base_radius * slant;
		return base_area + side_area;
	}

	MML_FORCE_INLINE T aperture_angle() const {
		return T(2) * std::atan(base_radius / height());
	}

	MML_FORCE_INLINE bool contains(const Vector3<T> &point) const {
		Vector3<T> to_point = point - apex;
		Vector3<T> axis_ = axis();
		T proj = to_point.dot(axis_);

		if (proj < T(0) || proj > height()) {
			return false;
		}

		T radius_at_height = (proj / height()) * base_radius;
		Vector3<T> projected = apex + axis_ * proj;
		return projected.distance_squared(point) <= radius_at_height * radius_at_height;
	}

	MML_FORCE_INLINE Vector3<T> closest_point(const Vector3<T> &point) const {
		Vector3<T> to_point = point - apex;
		Vector3<T> axis_ = axis();
		T proj = to_point.dot(axis_);

		if (proj <= T(0)) {
			return apex;
		}

		if (proj >= height()) {
			Vector3<T> to_base = point - base_center;
			T dist = to_base.length();
			if (Epsilon<T>::approx_zero(dist)) {
				return base_center;
			}
			return base_center + to_base.normalized() * base_radius;
		}

		T radius_at_height = (proj / height()) * base_radius;
		Vector3<T> projected = apex + axis_ * proj;
		Vector3<T> to_surface = point - projected;
		T dist = to_surface.length();

		if (dist <= radius_at_height) {
			return projected + to_surface;
		}

		return projected + to_surface.normalized() * radius_at_height;
	}

	MML_FORCE_INLINE T distance_squared(const Vector3<T> &point) const {
		Vector3<T> closest = closest_point(point);
		return (point - closest).length_squared();
	}

	MML_FORCE_INLINE T distance(const Vector3<T> &point) const {
		return Math<T>::safe_sqrt(distance_squared(point));
	}

	MML_FORCE_INLINE AABB<T> bounding_box() const {
		Vector3<T> axis_ = axis();
		T h = height();

		Vector3<T> perp1, perp2;
		if (std::abs(axis_.x) > T(0.9)) {
			perp1 = Vector3<T>(T(0), T(1), T(0));
		} else {
			perp1 = Vector3<T>(T(1), T(0), T(0));
		}
		perp1 = perp1 - axis_ * perp1.dot(axis_);
		perp1 = perp1.normalized();
		perp2 = axis_.cross(perp1).normalized();

		Vector3<T> base_extent = perp1 * base_radius + perp2 * base_radius;

		Vector3<T> min_v = (apex).min(base_center - base_extent);
		Vector3<T> max_v = (apex).max(base_center + base_extent);

		return AABB<T>(min_v, max_v);
	}

	MML_FORCE_INLINE static Cone from_apex_axis_radius(const Vector3<T> &apex_, const Vector3<T> &axis_, T height_, T radius_) {
		Vector3<T> base = apex_ + axis_.normalized() * height_;
		return Cone(apex_, base, radius_);
	}
};

using Conef = Cone<float>;
using Coned = Cone<double>;






template <typename T>
struct Cylinder {
	static_assert(std::is_floating_point_v<T>, "Cylinder requires floating-point type");

	Vector3<T> center;
	T radius;
	T height;
	Vector3<T> axis;

	Cylinder() = default;

	explicit Cylinder(const Vector3<T> &center_, T radius_, T height_, const Vector3<T> &axis_ = Vector3<T>::up()) :
			center(center_), radius(radius_), height(height_), axis(axis_.normalized()) {}

	constexpr Cylinder(const Cylinder &other) = default;
	Cylinder &operator=(const Cylinder &other) = default;

	MML_FORCE_INLINE T volume() const {
		return Constants<T>::pi * radius * radius * height;
	}

	MML_FORCE_INLINE T surface_area() const {
		T side_area = T(2) * Constants<T>::pi * radius * height;
		T cap_area = T(2) * Constants<T>::pi * radius * radius;
		return side_area + cap_area;
	}

	MML_FORCE_INLINE Vector3<T> top_center() const {
		return center + axis * (height * T(0.5));
	}

	MML_FORCE_INLINE Vector3<T> bottom_center() const {
		return center - axis * (height * T(0.5));
	}

	MML_FORCE_INLINE bool contains(const Vector3<T> &point) const {
		Vector3<T> to_point = point - center;
		T proj = to_point.dot(axis);

		if (std::abs(proj) > height * T(0.5)) {
			return false;
		}

		Vector3<T> projected = center + axis * proj;
		return projected.distance_squared(point) <= radius * radius;
	}

	MML_FORCE_INLINE Vector3<T> closest_point(const Vector3<T> &point) const {
		Vector3<T> to_point = point - center;
		T proj = to_point.dot(axis);

		T half_height = height * T(0.5);
		T clamped_proj = proj < -half_height ? -half_height : (proj > half_height ? half_height : proj);

		Vector3<T> projected_center = center + axis * clamped_proj;
		Vector3<T> to_surface = point - projected_center;
		T dist = to_surface.length();

		if (Epsilon<T>::approx_zero(dist)) {
			return projected_center + axis * half_height;
		}

		return projected_center + to_surface.normalized() * radius;
	}

	MML_FORCE_INLINE T distance_squared(const Vector3<T> &point) const {
		Vector3<T> closest = closest_point(point);
		return (point - closest).length_squared();
	}

	MML_FORCE_INLINE T distance(const Vector3<T> &point) const {
		return Math<T>::safe_sqrt(distance_squared(point));
	}

	MML_FORCE_INLINE AABB<T> bounding_box() const {
		Vector3<T> half_height = axis * (height * T(0.5));
		Vector3<T> top = center + half_height;
		Vector3<T> bottom = center - half_height;

		Vector3<T> perp1, perp2;
		if (std::abs(axis.x) > T(0.9)) {
			perp1 = Vector3<T>(T(0), T(1), T(0));
		} else {
			perp1 = Vector3<T>(T(1), T(0), T(0));
		}
		perp1 = perp1 - axis * perp1.dot(axis);
		perp1 = perp1.normalized();
		perp2 = axis.cross(perp1).normalized();

		Vector3<T> extent = perp1 * radius + perp2 * radius;

		Vector3<T> min_v = (top - extent).min(bottom - extent);
		Vector3<T> max_v = (top + extent).max(bottom + extent);

		return AABB<T>(min_v, max_v);
	}

	MML_FORCE_INLINE static Cylinder from_top_bottom(const Vector3<T> &top, const Vector3<T> &bottom, T radius_) {
		Vector3<T> center = (top + bottom) * T(0.5);
		Vector3<T> axis_ = (top - bottom).normalized();
		T height_ = top.distance(bottom);
		return Cylinder(center, radius_, height_, axis_);
	}
};

using Cylinderf = Cylinder<float>;
using Cylinderd = Cylinder<double>;






template <typename T>
struct Plane {
	static_assert(std::is_floating_point_v<T>, "Plane requires floating-point type");

	Vector3<T> normal;
	T distance;

	Plane() = default;

	explicit Plane(const Vector3<T> &normal_, T distance_) :
			normal(normal_.normalized()), distance(distance_) {}

	static Plane from_point_normal(const Vector3<T> &point, const Vector3<T> &normal_) {
		Vector3<T> n = normal_.normalized();
		return Plane(n, n.dot(point));
	}

	static Plane from_points(const Vector3<T> &a, const Vector3<T> &b, const Vector3<T> &c) {
		Vector3<T> ab = b - a;
		Vector3<T> ac = c - a;
		Vector3<T> n = ab.cross(ac).normalized();
		return Plane(n, n.dot(a));
	}

	Plane(const Plane &other) = default;
	Plane &operator=(const Plane &other) = default;

	constexpr MML_FORCE_INLINE T distance_to(const Vector3<T> &point) const {
		return normal.dot(point) - distance;
	}

	constexpr MML_FORCE_INLINE Vector3<T> project(const Vector3<T> &point) const {
		return point - normal * distance_to(point);
	}

	MML_FORCE_INLINE bool contains(const Vector3<T> &point) const {
		return Epsilon<T>::approx_zero(distance_to(point));
	}

	MML_FORCE_INLINE bool intersect_ray(const Ray<T> &ray, T &out_t) const {
		T denom = normal.dot(ray.direction);

		if (Epsilon<T>::approx_zero(denom)) {
			return false;
		}

		out_t = (distance - normal.dot(ray.origin)) / denom;
		return true;
	}

	MML_FORCE_INLINE bool intersect_ray(const Ray<T> &ray, Vector3<T> &out_point) const {
		T t;
		if (intersect_ray(ray, t)) {
			out_point = ray.at(t);
			return true;
		}
		return false;
	}

	constexpr MML_FORCE_INLINE Plane flip() const {
		return Plane(-normal, -distance);
	}
};

using Planef = Plane<float>;
using Planed = Plane<double>;






template <typename T>
struct Frustum {
	static_assert(std::is_floating_point_v<T>, "Frustum requires floating-point type");

	enum PlaneIndex {
		LEFT = 0,
		RIGHT = 1,
		BOTTOM = 2,
		TOP = 3,
		NEAR = 4,
		FAR = 5,
		PLANE_COUNT = 6
	};

	Plane<T> planes[PLANE_COUNT];

	Frustum() = default;

	static Frustum from_matrix(const Matrix4<T> &view_proj) {
		Frustum f;

		Vector3<T> left_normal(
				view_proj[0][3] + view_proj[0][0],
				view_proj[1][3] + view_proj[1][0],
				view_proj[2][3] + view_proj[2][0]);
		T left_dist = view_proj[3][3] + view_proj[3][0];
		T left_len = left_normal.length();
		f.planes[LEFT] = Plane<T>(left_normal / left_len, -left_dist / left_len);

		Vector3<T> right_normal(
				view_proj[0][3] - view_proj[0][0],
				view_proj[1][3] - view_proj[1][0],
				view_proj[2][3] - view_proj[2][0]);
		T right_dist = view_proj[3][3] - view_proj[3][0];
		T right_len = right_normal.length();
		f.planes[RIGHT] = Plane<T>(right_normal / right_len, -right_dist / right_len);

		Vector3<T> bottom_normal(
				view_proj[0][3] + view_proj[0][1],
				view_proj[1][3] + view_proj[1][1],
				view_proj[2][3] + view_proj[2][1]);
		T bottom_dist = view_proj[3][3] + view_proj[3][1];
		T bottom_len = bottom_normal.length();
		f.planes[BOTTOM] = Plane<T>(bottom_normal / bottom_len, -bottom_dist / bottom_len);

		Vector3<T> top_normal(
				view_proj[0][3] - view_proj[0][1],
				view_proj[1][3] - view_proj[1][1],
				view_proj[2][3] - view_proj[2][1]);
		T top_dist = view_proj[3][3] - view_proj[3][1];
		T top_len = top_normal.length();
		f.planes[TOP] = Plane<T>(top_normal / top_len, -top_dist / top_len);

		Vector3<T> near_normal(
				view_proj[0][3] + view_proj[0][2],
				view_proj[1][3] + view_proj[1][2],
				view_proj[2][3] + view_proj[2][2]);
		T near_dist = view_proj[3][3] + view_proj[3][2];
		T near_len = near_normal.length();
		f.planes[NEAR] = Plane<T>(near_normal / near_len, -near_dist / near_len);

		Vector3<T> far_normal(
				view_proj[0][3] - view_proj[0][2],
				view_proj[1][3] - view_proj[1][2],
				view_proj[2][3] - view_proj[2][2]);
		T far_dist = view_proj[3][3] - view_proj[3][2];
		T far_len = far_normal.length();
		f.planes[FAR] = Plane<T>(far_normal / far_len, -far_dist / far_len);

		return f;
	}

	Frustum(const Frustum &other) = default;
	Frustum &operator=(const Frustum &other) = default;

	MML_FORCE_INLINE bool contains(const Vector3<T> &point) const {
		for (size_t i = 0; i < PLANE_COUNT; ++i) {
			if (planes[i].distance_to(point) < T(0)) {
				return false;
			}
		}
		return true;
	}

	MML_FORCE_INLINE bool intersects(const Sphere<T> &sphere) const {
		for (size_t i = 0; i < PLANE_COUNT; ++i) {
			T distance = planes[i].distance_to(sphere.center);
			if (distance < -sphere.radius) {
				return false;
			}
		}
		return true;
	}

	MML_FORCE_INLINE bool intersects(const AABB<T> &aabb) const {
		for (size_t i = 0; i < PLANE_COUNT; ++i) {
			Vector3<T> positive_vertex(
					(planes[i].normal.x >= T(0)) ? aabb.max.x : aabb.min.x,
					(planes[i].normal.y >= T(0)) ? aabb.max.y : aabb.min.y,
					(planes[i].normal.z >= T(0)) ? aabb.max.z : aabb.min.z);

			if (planes[i].distance_to(positive_vertex) < T(0)) {
				return false;
			}
		}
		return true;
	}

	MML_FORCE_INLINE bool contains(const AABB<T> &aabb) const {
		for (size_t i = 0; i < PLANE_COUNT; ++i) {
			Vector3<T> negative_vertex(
					(planes[i].normal.x >= T(0)) ? aabb.min.x : aabb.max.x,
					(planes[i].normal.y >= T(0)) ? aabb.min.y : aabb.max.y,
					(planes[i].normal.z >= T(0)) ? aabb.min.z : aabb.max.z);

			if (planes[i].distance_to(negative_vertex) < T(0)) {
				return false;
			}
		}
		return true;
	}
};

using Frustumf = Frustum<float>;
using Frustumd = Frustum<double>;






template <typename T>
struct Triangle {
	static_assert(std::is_floating_point_v<T>, "Triangle requires floating-point type");

	Vector3<T> a;
	Vector3<T> b;
	Vector3<T> c;

	Triangle() = default;

	explicit Triangle(const Vector3<T> &a_, const Vector3<T> &b_, const Vector3<T> &c_) :
			a(a_), b(b_), c(c_) {}

	constexpr Triangle(const Triangle &other) = default;
	Triangle &operator=(const Triangle &other) = default;

	MML_FORCE_INLINE Vector3<T> center() const {
		return (a + b + c) * (T(1) / T(3));
	}

	MML_FORCE_INLINE T area() const {
		return (b - a).cross(c - a).length() * T(0.5);
	}

	MML_FORCE_INLINE Vector3<T> normal() const {
		return (b - a).cross(c - a).normalized();
	}

	MML_FORCE_INLINE Plane<T> plane() const {
		return Plane<T>::from_points(a, b, c);
	}

	MML_FORCE_INLINE bool contains(const Vector3<T> &point) const {
		Vector3<T> v0 = c - a;
		Vector3<T> v1 = b - a;
		Vector3<T> v2 = point - a;

		T dot00 = v0.dot(v0);
		T dot01 = v0.dot(v1);
		T dot02 = v0.dot(v2);
		T dot11 = v1.dot(v1);
		T dot12 = v1.dot(v2);

		T inv_denom = T(1) / (dot00 * dot11 - dot01 * dot01);
		T u = (dot11 * dot02 - dot01 * dot12) * inv_denom;
		T v = (dot00 * dot12 - dot01 * dot02) * inv_denom;

		return (u >= T(0)) && (v >= T(0)) && (u + v <= T(1));
	}

	MML_FORCE_INLINE bool intersect_ray(const Ray<T> &ray, T &out_t) const {
		Vector3<T> edge1 = b - a;
		Vector3<T> edge2 = c - a;

		Vector3<T> h = ray.direction.cross(edge2);
		T a_f = edge1.dot(h);

		if (Epsilon<T>::approx_zero(a_f)) {
			return false;
		}

		T f = T(1) / a_f;
		Vector3<T> s = ray.origin - a;
		T u = f * s.dot(h);

		if (u < T(0) || u > T(1)) {
			return false;
		}

		Vector3<T> q = s.cross(edge1);
		T v = f * ray.direction.dot(q);

		if (v < T(0) || u + v > T(1)) {
			return false;
		}

		T t = f * edge2.dot(q);

		if (t > T(0)) {
			out_t = t;
			return true;
		}

		return false;
	}

	MML_FORCE_INLINE bool intersect_ray(const Ray<T> &ray, Vector3<T> &out_point) const {
		T t;
		if (intersect_ray(ray, t)) {
			out_point = ray.at(t);
			return true;
		}
		return false;
	}

	MML_FORCE_INLINE bool intersect_ray(const Ray<T> &ray, Vector3<T> &out_point, Vector3<T> &out_barycentric) const {
		T t;
		T u, v;
		if (intersect_ray(ray, t)) {
			out_point = ray.at(t);

			Vector3<T> v0 = c - a;
			Vector3<T> v1 = b - a;
			Vector3<T> v2 = out_point - a;

			T dot00 = v0.dot(v0);
			T dot01 = v0.dot(v1);
			T dot02 = v0.dot(v2);
			T dot11 = v1.dot(v1);
			T dot12 = v1.dot(v2);

			T inv_denom = T(1) / (dot00 * dot11 - dot01 * dot01);
			u = (dot11 * dot02 - dot01 * dot12) * inv_denom;
			v = (dot00 * dot12 - dot01 * dot02) * inv_denom;
			out_barycentric.x = T(1) - u - v;
			out_barycentric.y = u;
			out_barycentric.z = v;

			return true;
		}
		return false;
	}

	MML_FORCE_INLINE Vector3<T> closest_point(const Vector3<T> &point) const {
		Vector3<T> ab = b - a;
		Vector3<T> ac = c - a;
		Vector3<T> ap = point - a;

		T d1 = ab.dot(ap);
		T d2 = ac.dot(ap);

		if (d1 <= T(0) && d2 <= T(0)) {
			return a;
		}

		Vector3<T> bp = point - b;
		T d3 = ab.dot(bp);
		T d4 = ac.dot(bp);

		if (d3 >= T(0) && d4 <= d3) {
			return b;
		}

		Vector3<T> cp = point - c;
		T d5 = ab.dot(cp);
		T d6 = ac.dot(cp);

		if (d6 >= T(0) && d5 <= d6) {
			return c;
		}

		T vc = d1 * d4 - d3 * d2;
		if (vc <= T(0) && d1 >= T(0) && d3 <= T(0)) {
			T v = d1 / (d1 - d3);
			return a + ab * v;
		}

		T vb = d5 * d2 - d1 * d6;
		if (vb <= T(0) && d2 >= T(0) && d6 <= T(0)) {
			T w = d2 / (d2 - d6);
			return a + ac * w;
		}

		T va = d3 * d6 - d5 * d4;
		if (va <= T(0) && (d4 - d3) >= T(0) && (d5 - d6) >= T(0)) {
			T w = (d4 - d3) / ((d4 - d3) + (d5 - d6));
			return b + (c - b) * w;
		}

		T denom = T(1) / (va + vb + vc);
		T v = vb * denom;
		T w = vc * denom;
		return a + ab * v + ac * w;
	}

	MML_FORCE_INLINE T distance_squared(const Vector3<T> &point) const {
		Vector3<T> closest = closest_point(point);
		return (point - closest).length_squared();
	}

	MML_FORCE_INLINE T distance(const Vector3<T> &point) const {
		return Math<T>::safe_sqrt(distance_squared(point));
	}

	MML_FORCE_INLINE T signed_distance(const Vector3<T> &point) const {
		Vector3<T> n = normal();
		Plane<T> p = plane();
		T d = p.distance_to(point);
		if (d < T(0)) {
			return -distance(point);
		}
		return distance(point);
	}

	MML_FORCE_INLINE Triangle<T> flipped() const {
		return Triangle<T>(c, b, a);
	}

	MML_FORCE_INLINE void flip() {
		Vector3<T> temp = a;
		a = c;
		c = temp;
	}
};

using Trianglef = Triangle<float>;
using Triangled = Triangle<double>;






namespace intersection {

template <typename T>
inline bool ray_sphere(const Ray<T> &ray, const Vector3<T> &center, T radius, T &out_t, Vector3<T> &out_point) {
	Vector3<T> oc = ray.origin - center;
	T a = ray.direction.dot(ray.direction);
	T b = T(2) * oc.dot(ray.direction);
	T c = oc.dot(oc) - radius * radius;
	T discriminant = b * b - T(4) * a * c;

	if (discriminant < T(0)) {
		return false;
	}

	T sqrt_disc = std::sqrt(discriminant);
	T t = (-b - sqrt_disc) / (T(2) * a);

	if (t < T(0)) {
		t = (-b + sqrt_disc) / (T(2) * a);
		if (t < T(0)) {
			return false;
		}
	}

	out_t = t;
	out_point = ray.at(t);
	return true;
}

template <typename T>
inline bool ray_aabb(const Ray<T> &ray, const AABB<T> &aabb, T &out_t, Vector3<T> &out_point) {
	T t_min = T(0);
	T t_max = std::numeric_limits<T>::infinity();

	for (int i = 0; i < 3; ++i) {
		if (std::abs(ray.direction[i]) < Epsilon<T>::value) {
			if (ray.origin[i] < aabb.min[i] || ray.origin[i] > aabb.max[i]) {
				return false;
			}
		} else {
			T inv_d = T(1) / ray.direction[i];
			T t1 = (aabb.min[i] - ray.origin[i]) * inv_d;
			T t2 = (aabb.max[i] - ray.origin[i]) * inv_d;

			if (t1 > t2) {
				std::swap(t1, t2);
			}
			t_min = std::max(t_min, t1);
			t_max = std::min(t_max, t2);

			if (t_min > t_max) {
				return false;
			}
		}
	}

	out_t = t_min;
	out_point = ray.at(t_min);
	return true;
}

template <typename T>
inline bool ray_triangle(const Ray<T> &ray, const Triangle<T> &tri, T &out_t, Vector3<T> &out_point, Vector3<T> &out_barycentric) {
	Vector3<T> edge1 = tri.b - tri.a;
	Vector3<T> edge2 = tri.c - tri.a;
	Vector3<T> h = ray.direction.cross(edge2);
	T a = edge1.dot(h);

	if (std::abs(a) < Epsilon<T>::value) {
		return false;
	}

	T f = T(1) / a;
	Vector3<T> s = ray.origin - tri.a;
	T u = f * s.dot(h);

	if (u < T(0) || u > T(1)) {
		return false;
	}

	Vector3<T> q = s.cross(edge1);
	T v = f * ray.direction.dot(q);

	if (v < T(0) || u + v > T(1)) {
		return false;
	}

	T t = f * edge2.dot(q);

	if (t > Epsilon<T>::value) {
		out_t = t;
		out_point = ray.at(t);
		out_barycentric = Vector3<T>(T(1) - u - v, u, v);
		return true;
	}

	return false;
}

template <typename T>
inline bool ray_plane(const Ray<T> &ray, const Vector3<T> &plane_normal, T plane_d, T &out_t, Vector3<T> &out_point) {
	T denom = plane_normal.dot(ray.direction);

	if (std::abs(denom) < Epsilon<T>::value) {
		return false;
	}

	T t = -(plane_normal.dot(ray.origin) + plane_d) / denom;

	if (t >= T(0)) {
		out_t = t;
		out_point = ray.at(t);
		return true;
	}

	return false;
}

template <typename T>
inline bool ray_capsule(const Ray<T> &ray, const Capsule<T> &capsule, T &out_t, Vector3<T> &out_point) {
	Vector3<T> ca = capsule.b - capsule.a;
	Vector3<T> oc = ray.origin - capsule.a;

	T caca = ca.dot(ca);
	T card = ca.dot(ray.direction);
	T caoc = ca.dot(oc);

	T a = caca - card * card;
	T b = caca * oc.dot(ray.direction) - caoc * card;
	T c = caca * oc.dot(oc) - caoc * caoc - capsule.radius * capsule.radius * caca;

	T discriminant = b * b - a * c;

	if (discriminant < T(0)) {
		bool hit_a = ray_sphere(ray, capsule.a, capsule.radius, out_t, out_point);
		bool hit_b = ray_sphere(ray, capsule.b, capsule.radius, out_t, out_point);
		return hit_a || hit_b;
	}

	T t = (-b - std::sqrt(discriminant)) / a;

	T y = caoc + t * card;

	if (y >= T(0) && y <= caca) {
		if (t >= T(0)) {
			out_t = t;
			out_point = ray.at(t);
			return true;
		}
	}

	bool hit_a = ray_sphere(ray, capsule.a, capsule.radius, out_t, out_point);
	bool hit_b = ray_sphere(ray, capsule.b, capsule.radius, out_t, out_point);
	return hit_a || hit_b;
}

template <typename T>
inline bool sphere_sphere(const Vector3<T> &c1, T r1, const Vector3<T> &c2, T r2) {
	T dist_sq = c1.distance_squared(c2);
	T rad_sum = r1 + r2;
	return dist_sq <= rad_sum * rad_sum;
}

template <typename T>
inline bool sphere_aabb(const Vector3<T> &center, T radius, const AABB<T> &aabb) {
	Vector3<T> closest = aabb.closest_point(center);
	return center.distance_squared(closest) <= radius * radius;
}

template <typename T>
inline bool aabb_aabb(const AABB<T> &a, const AABB<T> &b) {
	return a.intersects(b);
}

template <typename T>
inline bool sphere_triangle(const Vector3<T> &center, T radius, const Triangle<T> &tri, Vector3<T> &out_closest_point) {
	Vector3<T> ab = tri.b - tri.a;
	Vector3<T> ac = tri.c - tri.a;
	Vector3<T> bc = tri.c - tri.b;

	Vector3<T> normal = ab.cross(ac).normalized();
	T dist_to_plane = (center - tri.a).dot(normal);
	Vector3<T> plane_point = center - normal * dist_to_plane;

	Vector3<T> ap = plane_point - tri.a;
	Vector3<T> bp = plane_point - tri.b;
	Vector3<T> cp = plane_point - tri.c;

	Vector3<T> n_ab = ab.cross(normal);
	Vector3<T> n_ac = ac.cross(normal);
	Vector3<T> n_bc = bc.cross(normal);

	bool inside = (n_ab.dot(ap) >= T(0)) && (n_ac.dot(ap) <= T(0)) && (n_bc.dot(bp) >= T(0));

	if (inside) {
		out_closest_point = plane_point;
	} else {
		out_closest_point = tri.a;
		T min_dist = center.distance_squared(tri.a);

		Vector3<T> closest_ab = tri.a + ab * std::max(T(0), std::min(T(1), (center - tri.a).dot(ab) / ab.length_squared()));
		T dist_ab = center.distance_squared(closest_ab);
		if (dist_ab < min_dist) {
			min_dist = dist_ab;
			out_closest_point = closest_ab;
		}

		Vector3<T> closest_bc = tri.b + bc * std::max(T(0), std::min(T(1), (center - tri.b).dot(bc) / bc.length_squared()));
		T dist_bc = center.distance_squared(closest_bc);
		if (dist_bc < min_dist) {
			min_dist = dist_bc;
			out_closest_point = closest_bc;
		}

		Vector3<T> ca = tri.a - tri.c;
		Vector3<T> closest_ca = tri.c + ca * std::max(T(0), std::min(T(1), (center - tri.c).dot(ca) / ca.length_squared()));
		T dist_ca = center.distance_squared(closest_ca);
		if (dist_ca < min_dist) {
			out_closest_point = closest_ca;
		}
	}

	return center.distance_squared(out_closest_point) <= radius * radius;
}

template <typename T>
inline bool capsule_aabb(const Capsule<T> &capsule, const AABB<T> &aabb) {
	Vector3<T> closest_aabb = aabb.closest_point(capsule.a);
	if (capsule.a.distance_squared(closest_aabb) <= capsule.radius * capsule.radius) {
		return true;
	}

	closest_aabb = aabb.closest_point(capsule.b);
	if (capsule.b.distance_squared(closest_aabb) <= capsule.radius * capsule.radius) {
		return true;
	}

	Vector3<T> ab = capsule.b - capsule.a;
	T ab_len_sq = ab.length_squared();

	const int samples = 8;
	for (int i = 0; i <= samples; ++i) {
		T t = T(i) / T(samples);
		Vector3<T> point = capsule.a + ab * t;
		Vector3<T> closest = aabb.closest_point(point);
		if (point.distance_squared(closest) <= capsule.radius * capsule.radius) {
			return true;
		}
	}

	return false;
}

template <typename T>
inline bool capsule_sphere(const Capsule<T> &capsule, const Vector3<T> &center, T radius) {
	Vector3<T> ab = capsule.b - capsule.a;
	T t = std::max(T(0), std::min(T(1), (center - capsule.a).dot(ab) / ab.length_squared()));
	Vector3<T> closest = capsule.a + ab * t;

	T total_radius = capsule.radius + radius;
	return center.distance_squared(closest) <= total_radius * total_radius;
}

template <typename T>
inline bool point_aabb(const Vector3<T> &point, const AABB<T> &aabb) {
	return aabb.contains(point);
}

template <typename T>
inline bool point_sphere(const Vector3<T> &point, const Vector3<T> &center, T radius) {
	return point.distance_squared(center) <= radius * radius;
}

template <typename T>
inline bool point_triangle(const Vector3<T> &point, const Triangle<T> &tri) {
	Vector3<T> ab = tri.b - tri.a;
	Vector3<T> ac = tri.c - tri.a;
	Vector3<T> ap = point - tri.a;

	T d00 = ab.dot(ab);
	T d01 = ab.dot(ac);
	T d11 = ac.dot(ac);
	T d20 = ap.dot(ab);
	T d21 = ap.dot(ac);

	T denom = d00 * d11 - d01 * d01;
	if (std::abs(denom) < Epsilon<T>::value) {
		return false;
	}

	T v = (d11 * d20 - d01 * d21) / denom;
	T w = (d00 * d21 - d01 * d20) / denom;
	T u = T(1) - v - w;

	return (u >= T(0)) && (v >= T(0)) && (w >= T(0));
}

template <typename T>
struct LineSegment {
	Vector3<T> start;
	Vector3<T> end;

	LineSegment() = default;
	LineSegment(const Vector3<T> &s, const Vector3<T> &e) :
			start(s), end(e) {}

	Vector3<T> closest_point(const Vector3<T> &p) const {
		Vector3<T> ab = end - start;
		T t = std::max(T(0), std::min(T(1), (p - start).dot(ab) / ab.length_squared()));
		return start + ab * t;
	}
};

template <typename T>
inline bool segment_sphere(const LineSegment<T> &seg, const Vector3<T> &center, T radius) {
	Vector3<T> closest = seg.closest_point(center);
	return center.distance_squared(closest) <= radius * radius;
}

template <typename T>
inline bool segment_aabb(const LineSegment<T> &seg, const AABB<T> &aabb) {
	if (aabb.contains(seg.start) || aabb.contains(seg.end)) {
		return true;
	}

	Vector3<T> dir = seg.end - seg.start;
	T len = dir.length();
	if (len < Epsilon<T>::value) {
		return false;
	}

	Ray<T> ray(seg.start, dir / len);
	T t;
	Vector3<T> point;

	if (!ray_aabb(ray, aabb, t, point)) {
		return false;
	}

	return t <= len;
}

template <typename T>
inline bool swept_sphere_aabb(const Vector3<T> &start, const Vector3<T> &end, T radius, const AABB<T> &aabb, T &out_t) {
	AABB<T> expanded(aabb.min - Vector3<T>(radius, radius, radius),
			aabb.max + Vector3<T>(radius, radius, radius));

	Vector3<T> dir = end - start;
	T len = dir.length();
	if (len < Epsilon<T>::value) {
		out_t = T(0);
		return expanded.contains(start);
	}

	Ray<T> ray(start, dir / len);
	T t;
	Vector3<T> point;

	if (!ray_aabb(ray, expanded, t, point)) {
		return false;
	}

	out_t = t / len;
	return out_t <= T(1);
}

template <typename T>
inline T distance_point_triangle(const Vector3<T> &point, const Triangle<T> &tri) {
	Vector3<T> closest;
	Vector3<T> dummy;
	sphere_triangle(point, T(0), tri, closest);
	return point.distance(closest);
}

template <typename T>
inline T distance_segment_segment(const LineSegment<T> &s1, const LineSegment<T> &s2) {
	Vector3<T> u = s1.end - s1.start;
	Vector3<T> v = s2.end - s2.start;
	Vector3<T> w = s1.start - s2.start;

	T a = u.dot(u);
	T b = u.dot(v);
	T c = v.dot(v);
	T d = u.dot(w);
	T e = v.dot(w);

	T D = a * c - b * b;
	T sc, sN, sD = D;
	T tc, tN, tD = D;

	const T SMALL_NUM = Epsilon<T>::value;

	if (D < SMALL_NUM) {
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

	sc = (std::abs(sN) < SMALL_NUM) ? T(0) : sN / sD;
	tc = (std::abs(tN) < SMALL_NUM) ? T(0) : tN / tD;

	Vector3<T> dP = w + (u * sc) - (v * tc);
	return dP.length();
}

using LineSegmentf = LineSegment<float>;
using LineSegmentd = LineSegment<double>;

} 






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
			size_t j = (i + 1) % n;
			area += (vertices[j].x - vertices[i].x) * (vertices[j].y + vertices[i].y);
		}

		return area * T(0.5);
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
			return Vector2<T>(T(0), T(0));
		}

		T a = T(0), x = T(0), y = T(0);
		size_t n = vertices.size();

		for (size_t i = 0; i < n; ++i) {
			size_t j = (i + 1) % n;
			T f = vertices[i].x * vertices[j].y - vertices[j].x * vertices[i].y;
			a += f;
			x += (vertices[i].x + vertices[j].x) * f;
			y += (vertices[i].y + vertices[j].y) * f;
		}

		if (std::abs(a) < T(0.0001)) {
			return Vector2<T>(T(0), T(0));
		}

		a *= T(0.5);
		return Vector2<T>(x / (T(6) * a), y / (T(6) * a));
	}

	MML_FORCE_INLINE bool is_convex() const {
		if (vertices.size() < 3) {
			return false;
		}

		Vector2<T> prev = get_edge(vertices.size() - 1);

		for (size_t i = 0; i < vertices.size(); ++i) {
			Vector2<T> curr = get_edge(i);
			T cross = prev.x * curr.y - prev.y * curr.x;

			if (cross < T(0)) {
				return false;
			}

			prev = curr;
		}

		return true;
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
			return false;
		}

		bool inside = false;
		for (size_t i = 0, j = n - 1; i < n; j = i++) {
			const Vector2<T> &vi = vertices[i];
			const Vector2<T> &vj = vertices[j];

			if (((vi.y > point.y) != (vj.y > point.y)) &&
					(point.x < (vj.x - vi.x) * (point.y - vi.y) / (vj.y - vi.y) + vi.x)) {
				inside = !inside;
			}
		}

		return inside;
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
		Polygon2D poly;
		poly.add_vertex(a);
		poly.add_vertex(b);
		poly.add_vertex(c);
		return poly;
	}

	MML_FORCE_INLINE static Polygon2D rectangle(const Vector2<T> &min_v, const Vector2<T> &max_v) {
		Polygon2D poly;
		poly.add_vertex(Vector2<T>(min_v.x, min_v.y));
		poly.add_vertex(Vector2<T>(max_v.x, min_v.y));
		poly.add_vertex(Vector2<T>(max_v.x, max_v.y));
		poly.add_vertex(Vector2<T>(min_v.x, max_v.y));
		return poly;
	}

	MML_FORCE_INLINE static Polygon2D regular(T radius, size_t sides, const Vector2<T> &center = Vector2<T>(T(0), T(0)), T rotation = T(0)) {
		Polygon2D poly;
		T angle_step = Constants<T>::two_pi / T(sides);

		for (size_t i = 0; i < sides; ++i) {
			T angle = rotation + angle_step * T(i);
			Vector2<T> vertex(
					center.x + radius * std::cos(angle),
					center.y + radius * std::sin(angle));
			poly.add_vertex(vertex);
		}

		return poly;
	}
};

using Polygon2Df = Polygon2D<float>;
using Polygon2Dd = Polygon2D<double>;





#if MML_USE_SIMD
#endif


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
	std::vector<PrimitiveType> storage;
	size_t max_leaf_size = 4;

	std::function<AABB<T>(const PrimitiveType &)> bounds_func_;

public:
	BVH() = default;

	explicit BVH(std::function<AABB<T>(const PrimitiveType &)> bounds_func) :
			bounds_func_(std::move(bounds_func)) {}

	void build(const std::vector<PrimitiveType> &primitives,
			std::function<AABB<T>(const PrimitiveType &)> bounds_func = nullptr) {
		if (bounds_func) {
			bounds_func_ = std::move(bounds_func);
		}

		if (!bounds_func_) {
			return;
		}

		storage = primitives;

		std::vector<PrimitiveType *> ptrs;
		ptrs.reserve(storage.size());
		for (auto &p : storage) {
			ptrs.push_back(&p);
		}

		root = build_recursive(ptrs, 0);
	}

	void build(std::vector<PrimitiveType> &&primitives) {
		storage = std::move(primitives);

		std::vector<PrimitiveType *> ptrs;
		ptrs.reserve(storage.size());
		for (auto &p : storage) {
			ptrs.push_back(&p);
		}

		root = build_recursive(ptrs, 0);
	}

	[[nodiscard]] AABB<T> get_primitive_bounds(const PrimitiveType &p) const {
		return bounds_func_(p);
	}

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

	[[nodiscard]] std::vector<PrimitiveType *> frustum_cull(
			const std::array<Vector3<T>, 6> &frustum_planes) const {
		std::vector<PrimitiveType *> result;
		if (!root) {
			return result;
		}

		frustum_cull_recursive(root.get(), frustum_planes, result);
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

	void refit() {
		if (!root) {
			return;
		}
		refit_recursive(root.get());
	}

	[[nodiscard]] AABB<T> get_bounds() const {
		return root ? root->bounds : AABB<T>::empty();
	}

	[[nodiscard]] size_t node_count() const {
		return count_nodes_recursive(root.get());
	}

	[[nodiscard]] size_t depth() const {
		return depth_recursive(root.get());
	}

	void clear() {
		root.reset();
		storage.clear();
	}

	void set_max_leaf_size(size_t size) {
		max_leaf_size = std::max(size_t(1), size);
	}

private:
	[[nodiscard]] std::unique_ptr<Node> build_recursive(std::vector<PrimitiveType *> &primitives, int depth) {
		if (primitives.empty()) {
			return nullptr;
		}

		auto node = std::make_unique<Node>();

		AABB<T> total_bounds = AABB<T>::empty();
		for (auto *p : primitives) {
			total_bounds = AABB<T>::merge(total_bounds, bounds_func_(*p));
		}
		node->bounds = total_bounds;

		if (primitives.size() <= max_leaf_size) {
			node->is_leaf = true;
			node->primitives = std::move(primitives);
			return node;
		}

		Vector3<T> extents = total_bounds.extents();
		int axis = 0;
		if (extents.y > extents.x) {
			axis = 1;
		}
		if (extents.z > (axis == 0 ? extents.x : extents.y)) {
			axis = 2;
		}

		std::sort(primitives.begin(), primitives.end(),
				[this, axis](PrimitiveType *a, PrimitiveType *b) {
					Vector3<T> center_a = bounds_func_(*a).center();
					Vector3<T> center_b = bounds_func_(*b).center();
					return center_a[axis] < center_b[axis];
				});

		size_t mid = primitives.size() / 2;
		std::vector<PrimitiveType *> left_prims(primitives.begin(), primitives.begin() + mid);
		std::vector<PrimitiveType *> right_prims(primitives.begin() + mid, primitives.end());

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

		if (tmin > closest_dist) {
			return;
		}

		if (node->is_leaf) {
			for (auto *p : node->primitives) {
				T dist = (bounds_func_(*p).center() - origin).length();
				if (dist < closest_dist) {
					closest_dist = dist;
					closest = p;
				}
			}
		} else {
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

	[[nodiscard]] bool ray_aabb_intersect(const Vector3<T> &origin, const Vector3<T> &direction,
			const AABB<T> &box) const {
		T tmin, tmax;
		return ray_aabb_intersect_distance(origin, direction, box, tmin, tmax);
	}

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

	[[nodiscard]] bool aabb_in_frustum(const AABB<T> &box, const std::array<Vector3<T>, 6> &planes) const {
		for (const auto &plane : planes) {
			Vector3<T> p;
			p.x = (plane.x > 0) ? box.min.x : box.max.x;
			p.y = (plane.y > 0) ? box.min.y : box.max.y;
			p.z = (plane.z > 0) ? box.min.z : box.max.z;

			T distance = plane.x * p.x + plane.y * p.y + plane.z * p.z + plane.w;

			if (distance < 0) {
				return false;
			}
		}

		return true;
	}
};

using BVHf = BVH<float, void>;
using BVHd = BVH<double, void>;






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
using KDTreed = KDTree<double, void>;






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






template <typename T>
struct Transform {
	static_assert(std::is_floating_point_v<T>, "Transform requires floating-point type");

	Vector3<T> position;
	Quaternion<T> rotation;
	Vector3<T> scale;

	Transform() :
			position(Vector3<T>(T(0), T(0), T(0))), rotation(Quaternion<T>::identity()), scale(Vector3<T>(T(1), T(1), T(1))) {}

	explicit Transform(const Vector3<T> &pos, const Quaternion<T> &rot, const Vector3<T> &scl) :
			position(pos), rotation(rot), scale(scl) {}

	Transform(const Transform &other) = default;
	Transform &operator=(const Transform &other) = default;

	static Transform identity() {
		return Transform();
	}

	Matrix4<T> to_matrix() const {
		Matrix4<T> s = Matrix4<T>::scale(scale);

		Matrix4<T> r = rotation.to_matrix4();

		Matrix4<T> t = Matrix4<T>::translation(position);

		return t * r * s;
	}

	static Transform from_matrix(const Matrix4<T> &m) {
		Transform result;

		result.position = Vector3<T>(m[3][0], m[3][1], m[3][2]);

		Vector3<T> scale_x = Vector3<T>(m[0][0], m[0][1], m[0][2]);
		Vector3<T> scale_y = Vector3<T>(m[1][0], m[1][1], m[1][2]);
		Vector3<T> scale_z = Vector3<T>(m[2][0], m[2][1], m[2][2]);

		result.scale = Vector3<T>(
				scale_x.length(),
				scale_y.length(),
				scale_z.length());

		Matrix3<T> rotate_matrix(
				scale_x / result.scale.x,
				scale_y / result.scale.y,
				scale_z / result.scale.z);

		result.rotation = Quaternion<T>::from_matrix(rotate_matrix);

		return result;
	}

	MML_FORCE_INLINE Vector3<T> transform_point(const Vector3<T> &point) const {
		Vector3<T> scaled = Vector3<T>(point.x * scale.x, point.y * scale.y, point.z * scale.z);
		Vector3<T> rotated = rotation.rotate(scaled);
		return rotated + position;
	}

	MML_FORCE_INLINE Vector3<T> transform_direction(const Vector3<T> &direction) const {
		return rotation.rotate(direction);
	}

	MML_FORCE_INLINE Transform operator*(const Transform &other) const {
		Transform result;
		result.position = position + rotation.rotate(Vector3<T>(other.position.x * scale.x, other.position.y * scale.y, other.position.z * scale.z));
		result.rotation = rotation * other.rotation;
		result.scale = Vector3<T>(
				scale.x * other.scale.x,
				scale.y * other.scale.y,
				scale.z * other.scale.z);
		return result;
	}

	Transform inverse() const {
		Transform result;

		result.rotation = rotation.conjugate();

		result.scale = Vector3<T>(
				Epsilon<T>::is_zero(scale.x) ? T(0) : T(1) / scale.x,
				Epsilon<T>::is_zero(scale.y) ? T(0) : T(1) / scale.y,
				Epsilon<T>::is_zero(scale.z) ? T(0) : T(1) / scale.z);

		Vector3<T> inv_pos = -position;
		Vector3<T> rotated = result.rotation.rotate(inv_pos);
		result.position = Vector3<T>(
				rotated.x * result.scale.x,
				rotated.y * result.scale.y,
				rotated.z * result.scale.z);

		return result;
	}

	static Transform lerp(const Transform &a, const Transform &b, T t) {
		Transform result;
		result.position = a.position + (b.position - a.position) * t;
		result.rotation = Quaternion<T>::slerp(a.rotation, b.rotation, t);
		result.scale = a.scale + (b.scale - a.scale) * t;
		return result;
	}

	static Transform look_at(const Vector3<T> &position, const Vector3<T> &target, const Vector3<T> &up) {
		Vector3<T> forward = (target - position).normalized();
		Vector3<T> right = forward.cross(up).normalized();
		Vector3<T> new_up = right.cross(forward);

		Matrix3<T> rotate_matrix(right, new_up, -forward);

		Transform result;
		result.position = position;
		result.rotation = Quaternion<T>::from_matrix(rotate_matrix);
		result.scale = Vector3<T>(T(1), T(1), T(1));

		return result;
	}
};

using Transformf = Transform<float>;
using Transformd = Transform<double>;






namespace transform_flags {
constexpr uint32_t POSITION_CHANGED = 1 << 0;
constexpr uint32_t ROTATION_CHANGED = 1 << 1;
constexpr uint32_t SCALE_CHANGED = 1 << 2;
constexpr uint32_t LOCAL_CHANGED = POSITION_CHANGED | ROTATION_CHANGED | SCALE_CHANGED;
constexpr uint32_t PARENT_CHANGED = 1 << 3;
constexpr uint32_t WORLD_CHANGED = LOCAL_CHANGED | PARENT_CHANGED;
} 

template <typename T>
struct TransformNode {
	Vector3<T> position;
	Quaternion<T> rotation;
	Vector3<T> scale;

private:
	mutable Matrix4<T> _local_matrix_cache;
	mutable Matrix4<T> _world_matrix_cache;
	mutable uint32_t _dirty_flags;
	TransformNode *_parent;
	std::vector<TransformNode *> _children;

public:
	TransformNode() :
			position(T(0), T(0), T(0)), rotation(Quaternion<T>::identity()), scale(T(1), T(1), T(1)), _local_matrix_cache(Matrix4<T>::identity()), _world_matrix_cache(Matrix4<T>::identity()), _dirty_flags(transform_flags::WORLD_CHANGED), _parent(nullptr) {}

	explicit TransformNode(const Vector3<T> &pos, const Quaternion<T> &rot, const Vector3<T> &scl) :
			position(pos), rotation(rot), scale(scl), _local_matrix_cache(Matrix4<T>::identity()), _world_matrix_cache(Matrix4<T>::identity()), _dirty_flags(transform_flags::WORLD_CHANGED), _parent(nullptr) {}

	TransformNode(const TransformNode &other) :
			position(other.position), rotation(other.rotation), scale(other.scale), _local_matrix_cache(other._local_matrix_cache), _world_matrix_cache(other._world_matrix_cache), _dirty_flags(other._dirty_flags), _parent(nullptr) {}

	~TransformNode() {
		set_parent(nullptr);
		for (TransformNode *child : _children) {
			if (child && child->_parent == this) {
				child->_parent = nullptr;
			}
		}
	}

	TransformNode &operator=(const TransformNode &other) {
		if (this != &other) {
			position = other.position;
			rotation = other.rotation;
			scale = other.scale;
			_local_matrix_cache = other._local_matrix_cache;
			_world_matrix_cache = other._world_matrix_cache;
			mark_dirty(transform_flags::LOCAL_CHANGED);
		}
		return *this;
	}

	static TransformNode identity() {
		return TransformNode();
	}

	void set_position(const Vector3<T> &pos) {
		if (pos != position) {
			position = pos;
			mark_dirty(transform_flags::POSITION_CHANGED);
		}
	}

	void set_position(T x, T y, T z) {
		set_position(Vector3<T>(x, y, z));
	}

	void set_rotation(const Quaternion<T> &rot) {
		if (rot != rotation) {
			rotation = rot;
			mark_dirty(transform_flags::ROTATION_CHANGED);
		}
	}

	void set_scale(const Vector3<T> &scl) {
		if (scl != scale) {
			scale = scl;
			mark_dirty(transform_flags::SCALE_CHANGED);
		}
	}

	void set_scale(T x, T y, T z) {
		set_scale(Vector3<T>(x, y, z));
	}

	void set_uniform_scale(T s) {
		set_scale(Vector3<T>(s, s, s));
	}

	void set_transform(const Vector3<T> &pos, const Quaternion<T> &rot, const Vector3<T> &scl) {
		position = pos;
		rotation = rot;
		scale = scl;
		mark_dirty(transform_flags::LOCAL_CHANGED);
	}

	MML_FORCE_INLINE const Vector3<T> &get_position() const { return position; }
	MML_FORCE_INLINE const Quaternion<T> &get_rotation() const { return rotation; }
	MML_FORCE_INLINE const Vector3<T> &get_scale() const { return scale; }

	MML_FORCE_INLINE Vector3<T> get_world_position() const {
		update_world_matrix();
		return Vector3<T>(_world_matrix_cache[3][0], _world_matrix_cache[3][1], _world_matrix_cache[3][2]);
	}

	MML_FORCE_INLINE Quaternion<T> get_world_rotation() const {
		update_world_matrix();
		Matrix4<T> parent_matrix = _parent ? _parent->get_world_matrix() : Matrix4<T>::identity();
		Matrix4<T> parent_inv = parent_matrix.inverse_affine();
		Matrix3<T> rot_matrix = (parent_inv * _world_matrix_cache).to_matrix3();
		return Quaternion<T>::from_matrix(rot_matrix);
	}

	MML_FORCE_INLINE Vector3<T> get_world_scale() const {
		update_world_matrix();
		Matrix4<T> parent_matrix = _parent ? _parent->get_world_matrix() : Matrix4<T>::identity();
		Matrix4<T> parent_inv = parent_matrix.inverse_affine();
		Matrix4<T> local_world = parent_inv * _world_matrix_cache;

		Vector3<T> scale_x(local_world[0][0], local_world[0][1], local_world[0][2]);
		Vector3<T> scale_y(local_world[1][0], local_world[1][1], local_world[1][2]);
		Vector3<T> scale_z(local_world[2][0], local_world[2][1], local_world[2][2]);

		return Vector3<T>(scale_x.length(), scale_y.length(), scale_z.length());
	}

	MML_FORCE_INLINE const Matrix4<T> &get_local_matrix() const {
		if (_dirty_flags & transform_flags::LOCAL_CHANGED) {
			_local_matrix_cache = compute_local_matrix();
			_dirty_flags &= ~transform_flags::LOCAL_CHANGED;
		}
		return _local_matrix_cache;
	}

	MML_FORCE_INLINE const Matrix4<T> &get_world_matrix() const {
		update_world_matrix();
		return _world_matrix_cache;
	}

	void update_world_matrix() const {
		if (!(_dirty_flags & transform_flags::WORLD_CHANGED)) {
			return;
		}

		if (_parent) {
			_parent->update_world_matrix();
			_world_matrix_cache = _parent->_world_matrix_cache * get_local_matrix();
		} else {
			_world_matrix_cache = get_local_matrix();
		}

		_dirty_flags &= ~transform_flags::WORLD_CHANGED;
	}

	void set_parent(TransformNode *parent) {
		if (_parent == parent) {
			return;
		}

		if (_parent) {
			auto it = std::find(_parent->_children.begin(), _parent->_children.end(), this);
			if (it != _parent->_children.end()) {
				_parent->_children.erase(it);
			}
		}

		_parent = parent;

		if (_parent) {
			_parent->_children.push_back(this);
		}

		mark_dirty(transform_flags::PARENT_CHANGED);
	}

	TransformNode *get_parent() const { return _parent; }
	const std::vector<TransformNode *> &get_children() const { return _children; }
	size_t get_child_count() const { return _children.size(); }

	MML_FORCE_INLINE Vector3<T> transform_point(const Vector3<T> &point) const {
		return get_world_matrix().transform_point(point);
	}

	MML_FORCE_INLINE Vector3<T> transform_direction(const Vector3<T> &direction) const {
		return get_world_matrix().transform_direction(direction);
	}

	MML_FORCE_INLINE Vector3<T> inverse_transform_point(const Vector3<T> &point) const {
		Matrix4<T> inv = get_world_matrix().inverse();
		return inv.transform_point(point);
	}

	MML_FORCE_INLINE Vector3<T> inverse_transform_direction(const Vector3<T> &direction) const {
		Matrix4<T> inv = get_world_matrix().inverse_affine();
		return inv.transform_direction(direction);
	}

	void mark_dirty(uint32_t flags) const {
		_dirty_flags |= flags;
		for (TransformNode *child : _children) {
			if (child) {
				child->mark_dirty(transform_flags::PARENT_CHANGED);
			}
		}
	}

	MML_FORCE_INLINE bool is_dirty() const { return _dirty_flags != 0; }
	MML_FORCE_INLINE uint32_t get_dirty_flags() const { return _dirty_flags; }
	MML_FORCE_INLINE void clear_dirty_flags() { _dirty_flags = 0; }

	static TransformNode look_at(const Vector3<T> &eye, const Vector3<T> &target, const Vector3<T> &up) {
		Vector3<T> forward = (target - eye).normalized();
		Vector3<T> right = forward.cross(up).normalized();
		Vector3<T> new_up = right.cross(forward);

		Matrix3<T> rot_matrix(right, new_up, -forward);
		Quaternion<T> rot = Quaternion<T>::from_matrix(rot_matrix);

		TransformNode result;
		result.position = eye;
		result.rotation = rot;
		return result;
	}

	void translate(const Vector3<T> &delta) {
		set_position(position + delta);
	}

	void translate(T x, T y, T z) {
		translate(Vector3<T>(x, y, z));
	}

	void rotate(const Quaternion<T> &delta) {
		set_rotation(delta * rotation);
	}

	void rotate_axis(const Vector3<T> &axis, T angle) {
		Quaternion<T> delta = Quaternion<T>::from_axis_angle(axis, angle);
		rotate(delta);
	}

	void rotate_euler(T pitch, T yaw, T roll) {
		Quaternion<T> delta = Quaternion<T>::from_euler(pitch, yaw, roll);
		rotate(delta);
	}

	void rotate_around(const Vector3<T> &point, const Quaternion<T> &delta) {
		Vector3<T> offset = position - point;
		offset = delta.rotate(offset);
		position = point + offset;
		rotation = delta * rotation;
		mark_dirty(transform_flags::LOCAL_CHANGED);
	}

private:
	MML_FORCE_INLINE Matrix4<T> compute_local_matrix() const {
		Matrix4<T> s = Matrix4<T>::scale(scale);
		Matrix4<T> r = rotation.to_matrix4();
		Matrix4<T> t = Matrix4<T>::translation(position);
		return t * r * s;
	}
};

using TransformNodef = TransformNode<float>;
using TransformNoded = TransformNode<double>;

template <typename T>
struct TransformHierarchy {
	std::vector<std::unique_ptr<TransformNode<T>>> nodes;

	TransformNode<T> *create_node(const Vector3<T> &pos = Vector3<T>(),
			const Quaternion<T> &rot = Quaternion<T>::identity(),
			const Vector3<T> &scale = Vector3<T>(T(1), T(1), T(1))) {
		nodes.emplace_back(std::make_unique<TransformNode<T>>(pos, rot, scale));
		return nodes.back().get();
	}

	void clear() {
		nodes.clear();
	}

	void update_all() const {
		for (const auto &node : nodes) {
			if (node) {
				node->update_world_matrix();
			}
		}
	}

	size_t size() const { return nodes.size(); }

	TransformNode<T> *operator[](size_t index) { return nodes[index].get(); }
	const TransformNode<T> *operator[](size_t index) const { return nodes[index].get(); }
};

using TransformHierarchyf = TransformHierarchy<float>;
using TransformHierarchyd = TransformHierarchy<double>;






template <typename T>
struct Angle {
	static_assert(std::is_floating_point_v<T>, "Angle requires floating-point type");

	MML_FORCE_INLINE static constexpr T to_radians(T degrees) {
		return degrees * Constants<T>::pi / T(180);
	}

	MML_FORCE_INLINE static constexpr T to_degrees(T radians) {
		return radians * T(180) / Constants<T>::pi;
	}

	MML_FORCE_INLINE static T normalize(T radians) {
		T result = std::fmod(radians, Constants<T>::two_pi);
		if (result < T(0)) {
			result += Constants<T>::two_pi;
		}
		return result;
	}

	MML_FORCE_INLINE static T normalize_signed(T radians) {
		T result = normalize(radians);
		if (result >= Constants<T>::pi) {
			result -= Constants<T>::two_pi;
		}
		return result;
	}

	MML_FORCE_INLINE static T shortest_distance(T from, T to) {
		T diff = normalize_signed(to - from);
		return diff;
	}

	MML_FORCE_INLINE static T lerp(T from, T to, T t) {
		T diff = shortest_distance(from, to);
		return normalize(from + diff * t);
	}
};

using Anglef = Angle<float>;
using Angled = Angle<double>;






template <typename T>
struct Basis3 {
	Vector3<T> right;
	Vector3<T> up;
	Vector3<T> forward;

	constexpr Basis3() = default;
	constexpr Basis3(const Vector3<T> &r, const Vector3<T> &u, const Vector3<T> &f) :
			right(r), up(u), forward(f) {}

	constexpr Basis3(T rx, T ry, T rz, T ux, T uy, T uz, T fx, T fy, T fz) :
			right(rx, ry, rz), up(ux, uy, uz), forward(fx, fy, fz) {}

	static constexpr Basis3 identity() {
		return Basis3(
				Vector3<T>(T(1), T(0), T(0)),
				Vector3<T>(T(0), T(1), T(0)),
				Vector3<T>(T(0), T(0), T(1)));
	}

	constexpr Matrix3<T> to_matrix3() const {
		return Matrix3<T>(right, up, forward);
	}

	constexpr Matrix4<T> to_matrix4() const {
		return Matrix4<T>(
				Vector4<T>(right.x, right.y, right.z, T(0)),
				Vector4<T>(up.x, up.y, up.z, T(0)),
				Vector4<T>(forward.x, forward.y, forward.z, T(0)),
				Vector4<T>(T(0), T(0), T(0), T(1)));
	}
};

template <typename T>
inline Basis3<T> make_basis_from_forward(const Vector3<T> &forward, const Vector3<T> &world_up = Vector3<T>(T(0), T(1), T(0))) {
	Vector3<T> f = forward.normalized();

	Vector3<T> r = f.cross(world_up);
	if (r.length_squared() < Epsilon<T>::value) {
		r = f.cross(Vector3<T>(T(0), T(0), T(1)));
	}
	r = r.normalized();

	Vector3<T> u = r.cross(f);
	u = u.normalized();

	return Basis3<T>(r, u, f);
}

template <typename T>
inline Basis3<T> make_basis_from_up(const Vector3<T> &up, const Vector3<T> &world_forward = Vector3<T>(T(0), T(0), T(1))) {
	Vector3<T> u = up.normalized();

	Vector3<T> r = u.cross(world_forward);
	if (r.length_squared() < Epsilon<T>::value) {
		r = u.cross(Vector3<T>(T(1), T(0), T(0)));
	}
	r = r.normalized();

	Vector3<T> f = r.cross(u);
	f = f.normalized();

	return Basis3<T>(r, u, f);
}

template <typename T>
inline Basis3<T> make_basis_from_right(const Vector3<T> &right, const Vector3<T> &world_up = Vector3<T>(T(0), T(1), T(0))) {
	Vector3<T> r = right.normalized();

	Vector3<T> f = r.cross(world_up);
	if (f.length_squared() < Epsilon<T>::value) {
		f = r.cross(Vector3<T>(T(0), T(0), T(1)));
	}
	f = f.normalized();

	Vector3<T> u = f.cross(r);
	u = u.normalized();

	return Basis3<T>(r, u, f);
}

template <typename T>
inline Basis3<T> make_basis_from_vectors(const Vector3<T> &a, const Vector3<T> &b, const Vector3<T> &c, int primary_axis = 0) {
	Vector3<T> v[3] = { a, b, c };
	int order[3] = { 0, 1, 2 };

	for (int i = 0; i < 3; ++i) {
		order[i] = (primary_axis + i) % 3;
	}

	Vector3<T> axes[3];
	axes[order[0]] = v[order[0]].normalized();

	axes[order[1]] = v[order[1]];
	T proj = axes[order[1]].dot(axes[order[0]]);
	axes[order[1]] = axes[order[1]] - axes[order[0]] * proj;
	if (axes[order[1]].length_squared() < Epsilon<T>::value) {
		Vector3<T> alt = axes[order[0]].cross(Vector3<T>(T(0), T(0), T(1)));
		if (alt.length_squared() < Epsilon<T>::value) {
			alt = axes[order[0]].cross(Vector3<T>(T(0), T(1), T(0)));
		}
		axes[order[1]] = alt.normalized();
	} else {
		axes[order[1]] = axes[order[1]].normalized();
	}

	axes[order[2]] = axes[order[0]].cross(axes[order[1]]);

	return Basis3<T>(axes[0], axes[1], axes[2]);
}

template <typename T>
inline void orthonormalize(Vector3<T> &right, Vector3<T> &up, Vector3<T> &forward,
		int preserve_axis = 0) {
	int order[3] = { 0, 1, 2 };
	Vector3<T> *vecs[3] = { &right, &up, &forward };

	for (int i = 0; i < 3; ++i) {
		order[i] = (preserve_axis + i) % 3;
	}

	*vecs[order[0]] = vecs[order[0]]->normalized();

	for (int i = 1; i < 3; ++i) {
		T proj = vecs[order[i]]->dot(*vecs[order[0]]);
		*vecs[order[i]] = *vecs[order[i]] - *vecs[order[0]] * proj;

		for (int j = 1; j < i; ++j) {
			proj = vecs[order[i]]->dot(*vecs[order[j]]);
			*vecs[order[i]] = *vecs[order[i]] - *vecs[order[j]] * proj;
		}

		T len_sq = vecs[order[i]]->length_squared();
		if (len_sq > Epsilon<T>::value) {
			T len = Math<T>::safe_sqrt(len_sq);
			*vecs[order[i]] = *vecs[order[i]] / len;
		} else {
			Vector3<T> alt = vecs[order[0]]->cross(*vecs[order[1]]);
			if (alt.length_squared() < Epsilon<T>::value) {
				for (int k = 0; k < 3; ++k) {
					if (k != order[0] && k != order[1]) {
						alt = vecs[order[0]]->cross(Vector3<T>(
								T(k == 0), T(k == 1), T(k == 2)));
						break;
					}
				}
			}
			*vecs[order[i]] = alt.normalized();
		}
	}

	for (int i = 0; i < 3; ++i) {
		Vector3<T> &v = (i == 0) ? right : (i == 1) ? up
													: forward;
		v = *vecs[i];
	}
}

template <typename T>
inline Basis3<T> orthonormalize_basis(const Basis3<T> &basis, int preserve_axis = 0) {
	Basis3<T> result = basis;
	orthonormalize(result.right, result.up, result.forward, preserve_axis);
	return result;
}

template <typename T>
inline Matrix3<T> orthonormalize_matrix(const Matrix3<T> &m, int preserve_axis = 0) {
	Matrix3<T> result = m;
	orthonormalize(result[0], result[1], result[2], preserve_axis);
	return result;
}

template <typename T>
inline Basis3<T> make_look_at_basis(const Vector3<T> &eye, const Vector3<T> &target, const Vector3<T> &world_up = Vector3<T>(T(0), T(1), T(0))) {
	Vector3<T> forward = (target - eye).normalized();
	return make_basis_from_forward(forward, world_up);
}

template <typename T>
inline Basis3<T> align_basis_to_normal(const Vector3<T> &normal, const Vector3<T> &reference_up = Vector3<T>(T(0), T(1), T(0))) {
	Vector3<T> forward = normal.normalized();
	return make_basis_from_forward(forward, reference_up);
}

template <typename T>
inline T basis_volume(const Basis3<T> &basis) {
	Vector3<T> r = basis.right;
	Vector3<T> u = basis.up;
	Vector3<T> f = basis.forward;
	T det = r.x * (u.y * f.z - u.z * f.y) -
			r.y * (u.x * f.z - u.z * f.x) +
			r.z * (u.x * f.y - u.y * f.x);
	return det;
}

template <typename T>
inline bool is_right_handed(const Basis3<T> &basis, T epsilon = Epsilon<T>::value) {
	return basis_volume(basis) > epsilon;
}

template <typename T>
inline void make_right_handed(Basis3<T> &basis) {
	if (!is_right_handed(basis)) {
		basis.right = -basis.right;
	}
}

template <typename T>
inline void make_left_handed(Basis3<T> &basis) {
	if (is_right_handed(basis)) {
		basis.right = -basis.right;
	}
}

template <typename T>
inline Basis3<T> rotate_basis(const Basis3<T> &basis, const Vector3<T> &axis, T angle) {
	Quaternion<T> q = Quaternion<T>::from_axis_angle(axis, angle);
	return Basis3<T>(
			q.rotate(basis.right),
			q.rotate(basis.up),
			q.rotate(basis.forward));
}

template <typename T>
inline Basis3<T> rotate_basis(const Basis3<T> &basis, const Quaternion<T> &q) {
	return Basis3<T>(
			q.rotate(basis.right),
			q.rotate(basis.up),
			q.rotate(basis.forward));
}

template <typename T>
inline T basis_similarity(const Basis3<T> &a, const Basis3<T> &b) {
	T dr = (a.right - b.right).length();
	T du = (a.up - b.up).length();
	T df = (a.forward - b.forward).length();
	return dr + du + df;
}

template <typename T>
inline bool basis_equals(const Basis3<T> &a, const Basis3<T> &b, T epsilon = Epsilon<T>::value) {
	return Epsilon<T>::equal(a.right.dot(b.right), T(1), epsilon) &&
			Epsilon<T>::equal(a.up.dot(b.up), T(1), epsilon) &&
			Epsilon<T>::equal(a.forward.dot(b.forward), T(1), epsilon);
}

using Basis3f = Basis3<float>;
using Basis3d = Basis3<double>;






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






template <typename T>
struct Interpolation {
	static_assert(std::is_floating_point_v<T>, "Interpolation requires floating-point type");

	MML_FORCE_INLINE static T lerp(T a, T b, T t) {
		return a + (b - a) * t;
	}

	MML_FORCE_INLINE static T inverse_lerp(T a, T b, T value) {
		if (a == b) {
			return T(0);
		}
		return (value - a) / (b - a);
	}

	MML_FORCE_INLINE static T clamp(T value, T min_val, T max_val) {
		return value < min_val ? min_val : (value > max_val ? max_val : value);
	}

	MML_FORCE_INLINE static T lerp_clamped(T a, T b, T t) {
		return lerp(a, b, clamp(t, T(0), T(1)));
	}

	MML_FORCE_INLINE static T smoothstep(T a, T b, T t) {
		T x = clamp((t - a) / (b - a), T(0), T(1));
		return x * x * (T(3) - T(2) * x);
	}

	MML_FORCE_INLINE static T smootherstep(T a, T b, T t) {
		T x = clamp((t - a) / (b - a), T(0), T(1));
		return x * x * x * (x * (x * T(6) - T(15)) + T(10));
	}

	MML_FORCE_INLINE static T cosine(T a, T b, T t) {
		T mu = (T(1) - std::cos(t * Constants<T>::pi)) * T(0.5);
		return lerp(a, b, mu);
	}

	MML_FORCE_INLINE static T cubic(T p0, T p1, T p2, T p3, T t) {
		T t2 = t * t;
		T t3 = t2 * t;

		T a0 = p3 - p2 - p0 + p1;
		T a1 = p0 - p1 - a0;
		T a2 = p2 - p0;
		T a3 = p1;

		return a0 * t3 + a1 * t2 + a2 * t + a3;
	}

	MML_FORCE_INLINE static T hermite(T p0, T m0, T p1, T m1, T t) {
		T t2 = t * t;
		T t3 = t2 * t;

		T h00 = T(2) * t3 - T(3) * t2 + T(1);
		T h10 = t3 - T(2) * t2 + t;
		T h01 = -T(2) * t3 + T(3) * t2;
		T h11 = t3 - t2;

		return h00 * p0 + h10 * m0 + h01 * p1 + h11 * m1;
	}

	MML_FORCE_INLINE static T bezier_quadratic(T p0, T p1, T p2, T t) {
		T u = T(1) - t;
		return u * u * p0 + T(2) * u * t * p1 + t * t * p2;
	}

	MML_FORCE_INLINE static T bezier_cubic(T p0, T p1, T p2, T p3, T t) {
		T u = T(1) - t;
		T u2 = u * u;
		T u3 = u2 * u;
		T t2 = t * t;
		T t3 = t2 * t;

		return u3 * p0 + T(3) * u2 * t * p1 + T(3) * u * t2 * p2 + t3 * p3;
	}

	MML_FORCE_INLINE static T ease_in_expo(T t) {
		return t <= T(0) ? T(0) : std::pow(T(2), T(10) * (t - T(1)));
	}

	MML_FORCE_INLINE static T ease_out_expo(T t) {
		return t >= T(1) ? T(1) : T(1) - std::pow(T(2), -T(10) * t);
	}

	MML_FORCE_INLINE static T ease_in_out_expo(T t) {
		if (t <= T(0)) {
			return T(0);
		}
		if (t >= T(1)) {
			return T(1);
		}
		if (t < T(0.5)) {
			return std::pow(T(2), T(20) * t - T(10)) * T(0.5);
		} else {
			return (T(2) - std::pow(T(2), -T(20) * t + T(10))) * T(0.5);
		}
	}

	MML_FORCE_INLINE static T remap(T value, T in_min, T in_max, T out_min, T out_max) {
		T t = inverse_lerp(in_min, in_max, value);
		return lerp(out_min, out_max, t);
	}

	MML_FORCE_INLINE static T remap_clamped(T value, T in_min, T in_max, T out_min, T out_max) {
		T t = clamp(inverse_lerp(in_min, in_max, value), T(0), T(1));
		return lerp(out_min, out_max, t);
	}
};

using Interpolationf = Interpolation<float>;
using Interpolationd = Interpolation<double>;






template <typename T>
struct Projection {
	static_assert(std::is_floating_point_v<T>, "Projection requires floating-point type");

	static Matrix4<T> perspective(T fov_y, T aspect_ratio, T near_plane, T far_plane) {
		T tan_half_fov = std::tan(fov_y * T(0.5));

		Matrix4<T> result = Matrix4<T>::zero();
		result[0][0] = T(1) / (aspect_ratio * tan_half_fov);
		result[1][1] = T(1) / tan_half_fov;
		result[2][2] = far_plane / (near_plane - far_plane);
		result[2][3] = -T(1);
		result[3][2] = -(far_plane * near_plane) / (far_plane - near_plane);

		return result;
	}

	static Matrix4<T> perspective_opengl(T fov_y, T aspect_ratio, T near_plane, T far_plane) {
		T tan_half_fov = std::tan(fov_y * T(0.5));

		Matrix4<T> result = Matrix4<T>::zero();
		result[0][0] = T(1) / (aspect_ratio * tan_half_fov);
		result[1][1] = T(1) / tan_half_fov;
		result[2][2] = -(far_plane + near_plane) / (far_plane - near_plane);
		result[2][3] = -T(1);
		result[3][2] = -(T(2) * far_plane * near_plane) / (far_plane - near_plane);

		return result;
	}

	static Matrix4<T> perspective_infinite(T fov_y, T aspect_ratio, T near_plane) {
		T tan_half_fov = std::tan(fov_y * T(0.5));

		Matrix4<T> result = Matrix4<T>::zero();
		result[0][0] = T(1) / (aspect_ratio * tan_half_fov);
		result[1][1] = T(1) / tan_half_fov;
		result[2][2] = -T(1);
		result[2][3] = -T(1);
		result[3][2] = -near_plane;

		return result;
	}

	static Matrix4<T> orthographic(T left, T right, T bottom, T top, T near_plane, T far_plane) {
		Matrix4<T> result = Matrix4<T>::identity();

		result[0][0] = T(2) / (right - left);
		result[1][1] = T(2) / (top - bottom);
		result[2][2] = -T(1) / (far_plane - near_plane);
		result[3][0] = -(right + left) / (right - left);
		result[3][1] = -(top + bottom) / (top - bottom);
		result[3][2] = -near_plane / (far_plane - near_plane);

		return result;
	}

	static Matrix4<T> orthographic_opengl(T left, T right, T bottom, T top, T near_plane, T far_plane) {
		Matrix4<T> result = Matrix4<T>::identity();

		result[0][0] = T(2) / (right - left);
		result[1][1] = T(2) / (top - bottom);
		result[2][2] = -T(2) / (far_plane - near_plane);
		result[3][0] = -(right + left) / (right - left);
		result[3][1] = -(top + bottom) / (top - bottom);
		result[3][2] = -(far_plane + near_plane) / (far_plane - near_plane);

		return result;
	}

	static Matrix4<T> orthographic_centered(T width, T height, T near_plane, T far_plane) {
		T half_width = width * T(0.5);
		T half_height = height * T(0.5);
		return orthographic(-half_width, half_width, -half_height, half_height, near_plane, far_plane);
	}

	static Matrix4<T> look_at(const Vector3<T> &eye, const Vector3<T> &target, const Vector3<T> &up) {
		Vector3<T> forward = (target - eye).normalized();
		Vector3<T> right = forward.cross(up).normalized();
		Vector3<T> new_up = right.cross(forward);

		Matrix4<T> result = Matrix4<T>::identity();

		result[0][0] = right.x;
		result[1][0] = right.y;
		result[2][0] = right.z;

		result[0][1] = new_up.x;
		result[1][1] = new_up.y;
		result[2][1] = new_up.z;

		result[0][2] = -forward.x;
		result[1][2] = -forward.y;
		result[2][2] = -forward.z;

		result[3][0] = -right.dot(eye);
		result[3][1] = -new_up.dot(eye);
		result[3][2] = forward.dot(eye);

		return result;
	}

	static T extract_fov_y(const Matrix4<T> &proj) {
		return T(2) * std::atan(T(1) / proj[1][1]);
	}

	static T extract_aspect_ratio(const Matrix4<T> &proj) {
		return proj[1][1] / proj[0][0];
	}
};

using Projectionf = Projection<float>;
using Projectiond = Projection<double>;






template <typename T>
struct Random {
	static_assert(std::is_arithmetic_v<T>, "Random requires arithmetic type");

private:
	static std::mt19937 &engine() {
		thread_local std::mt19937 rng(std::random_device{}());
		return rng;
	}

	static std::uniform_real_distribution<T> &dist() {
		thread_local std::uniform_real_distribution<T> d(T(0), T(1));
		return d;
	}

public:
	MML_FORCE_INLINE static void seed(unsigned int value) {
		engine().seed(value);
	}

	MML_FORCE_INLINE static void seed_time() {
		std::random_device rd;
		engine().seed(rd());
	}

	MML_FORCE_INLINE static T range(T min_val, T max_val) {
		if constexpr (std::is_integral_v<T>) {
			std::uniform_int_distribution<T> d(min_val, max_val);
			return d(engine());
		} else {
			std::uniform_real_distribution<T> d(min_val, max_val);
			return d(engine());
		}
	}

	MML_FORCE_INLINE static T range_inclusive(T min_val, T max_val) {
		return range(min_val, max_val);
	}

	MML_FORCE_INLINE static int range_int(int min_val, int max_val) {
		std::uniform_int_distribution<int> d(min_val, max_val);
		return d(engine());
	}

	MML_FORCE_INLINE static T value() {
		if constexpr (std::is_integral_v<T>) {
			return range(T(0), std::numeric_limits<T>::max());
		} else {
			return dist()(engine());
		}
	}

	MML_FORCE_INLINE static T value_centered() {
		return value() * T(2) - T(1);
	}

	MML_FORCE_INLINE static bool chance(T probability) {
		return value() < probability;
	}

	MML_FORCE_INLINE static T normal(T mean = T(0), T std_dev = T(1)) {
		std::normal_distribution<T> d(mean, std_dev);
		return d(engine());
	}

	MML_FORCE_INLINE static Vector2<T> inside_unit_circle() {
		std::uniform_real_distribution<T> d(T(0), T(1));
		T angle = d(engine()) * Constants<T>::two_pi;
		T radius = std::sqrt(d(engine()));
		return Vector2<T>(std::cos(angle) * radius, std::sin(angle) * radius);
	}

	MML_FORCE_INLINE static Vector2<T> on_unit_circle() {
		std::uniform_real_distribution<T> d(T(0), T(1));
		T angle = d(engine()) * Constants<T>::two_pi;
		return Vector2<T>(std::cos(angle), std::sin(angle));
	}

	MML_FORCE_INLINE static Vector3<T> inside_unit_sphere() {
		std::uniform_real_distribution<T> d(T(0), T(1));
		T theta = d(engine()) * Constants<T>::two_pi;
		T phi = Math<T>::safe_acos(T(1) - d(engine()) * T(2));
		T radius = std::cbrt(d(engine()));

		T x = radius * std::sin(phi) * std::cos(theta);
		T y = radius * std::sin(phi) * std::sin(theta);
		T z = radius * std::cos(phi);

		return Vector3<T>(x, y, z);
	}

	MML_FORCE_INLINE static Vector3<T> on_unit_sphere() {
		std::uniform_real_distribution<T> d(T(0), T(1));
		T theta = d(engine()) * Constants<T>::two_pi;
		T phi = Math<T>::safe_acos(T(1) - d(engine()) * T(2));

		T x = std::sin(phi) * std::cos(theta);
		T y = std::sin(phi) * std::sin(theta);
		T z = std::cos(phi);

		return Vector3<T>(x, y, z);
	}

	MML_FORCE_INLINE static Vector2<T> range_2d(const Vector2<T> &min_val, const Vector2<T> &max_val) {
		return Vector2<T>(range(min_val.x, max_val.x), range(min_val.y, max_val.y));
	}

	MML_FORCE_INLINE static Vector3<T> range_3d(const Vector3<T> &min_val, const Vector3<T> &max_val) {
		return Vector3<T>(range(min_val.x, max_val.x), range(min_val.y, max_val.y), range(min_val.z, max_val.z));
	}

	MML_FORCE_INLINE static Vector3<T> inside_unit_cube() {
		return Vector3<T>(value(), value(), value());
	}

	MML_FORCE_INLINE static Vector3<T> on_unit_cube() {
		std::uniform_int_distribution<int> d(0, 1);
		return Vector3<T>(d(engine()) ? T(1) : T(-1), d(engine()) ? T(1) : T(-1), d(engine()) ? T(1) : T(-1));
	}

	MML_FORCE_INLINE static T perlin(T x, T y, T z) {
		static const T persistence = T(0.5);
		static const int octaves = 4;
		T total = T(0);
		T frequency = T(1);
		T amplitude = T(1);
		T maxValue = T(0);

		for (int i = 0; i < octaves; ++i) {
			total += std::sin(x * frequency + y * frequency + z * frequency) * amplitude;
			maxValue += amplitude;
			amplitude *= persistence;
			frequency *= T(2);
		}

		return total / maxValue;
	}
};

using Randomf = Random<float>;
using Randomd = Random<double>;
using Randomi = Random<int>;






namespace random {

class LCG32 {
public:
	using result_type = uint32_t;

	static constexpr uint32_t default_seed = 123456789u;

	static constexpr uint64_t multiplier = 1664525ull;
	static constexpr uint64_t increment = 1013904223ull;
	static constexpr uint64_t modulus = 4294967296ull;

	explicit LCG32(uint32_t seed = default_seed) :
			state_(seed), initial_seed_(seed) {}

	void seed(uint32_t s) {
		state_ = s;
		initial_seed_ = s;
	}

	uint32_t operator()() {
		state_ = static_cast<uint32_t>((multiplier * state_ + increment) % modulus);
		return state_;
	}

	uint32_t next_uint32() { return (*this)(); }

	uint64_t next_uint64() {
		uint64_t high = (*this)();
		uint64_t low = (*this)();
		return (high << 32) | low;
	}

	float next_float() {
		return static_cast<float>((*this)()) * (1.0f / 4294967296.0f);
	}

	double next_double() {
		return static_cast<double>((*this)()) * (1.0 / 4294967296.0);
	}

	uint32_t range_uint32(uint32_t min_val, uint32_t max_val) {
		if (min_val >= max_val) {
			return min_val;
		}
		uint32_t range = max_val - min_val + 1;
		return min_val + ((*this)() % range);
	}

	int32_t range_int32(int32_t min_val, int32_t max_val) {
		if (min_val >= max_val) {
			return min_val;
		}
		uint32_t range = static_cast<uint32_t>(max_val - min_val + 1);
		return min_val + static_cast<int32_t>((*this)() % range);
	}

	float range_float(float min_val, float max_val) {
		return min_val + next_float() * (max_val - min_val);
	}

	double range_double(double min_val, double max_val) {
		return min_val + next_double() * (max_val - min_val);
	}

	bool chance(float probability) {
		return next_float() < probability;
	}

	void reset() { state_ = initial_seed_; }

	static constexpr uint32_t min() { return 0; }
	static constexpr uint32_t max() { return std::numeric_limits<uint32_t>::max(); }

private:
	uint32_t state_;
	uint32_t initial_seed_;
};

class PCG32 {
public:
	using result_type = uint32_t;

	static constexpr uint64_t default_seed = 0x853c49e6748fea9bull;
	static constexpr uint64_t default_stream = 0xda3e39cb94b95bdbull;

	explicit PCG32(uint64_t seed = default_seed, uint64_t stream = default_stream) :
			initial_seed_(seed), initial_stream_(stream) {
		seed_(seed, stream);
	}

	void seed(uint64_t s, uint64_t stream = default_stream) {
		initial_seed_ = s;
		initial_stream_ = stream;
		seed_(s, stream);
	}

	uint32_t operator()() {
		uint64_t oldstate = state_;
		state_ = oldstate * 6364136223846793005ull + (inc_ | 1);
		uint32_t xorshifted = static_cast<uint32_t>(((oldstate >> 18u) ^ oldstate) >> 27u);
		uint32_t rot = static_cast<uint32_t>(oldstate >> 59u);
		return (xorshifted >> rot) | (xorshifted << ((-static_cast<int32_t>(rot)) & 31));
	}

	uint32_t next_uint32() { return (*this)(); }

	uint64_t next_uint64() {
		uint64_t high = (*this)();
		uint64_t low = (*this)();
		return (high << 32) | low;
	}

	float next_float() {
		return static_cast<float>((*this)()) * (1.0f / 4294967296.0f);
	}

	double next_double() {
		uint32_t high = (*this)();
		uint32_t low = (*this)();
		uint64_t val = (static_cast<uint64_t>(high) << 32) | low;
		return static_cast<double>(val >> 11) * (1.0 / 9007199254740992.0);
	}

	uint32_t range_uint32(uint32_t min_val, uint32_t max_val) {
		if (min_val >= max_val) {
			return min_val;
		}
		uint32_t range = max_val - min_val + 1;

		uint32_t threshold = (std::numeric_limits<uint32_t>::max() / range) * range;
		uint32_t result;
		do {
			result = (*this)();
		} while (result >= threshold);

		return min_val + (result % range);
	}

	int32_t range_int32(int32_t min_val, int32_t max_val) {
		if (min_val >= max_val) {
			return min_val;
		}
		return static_cast<int32_t>(range_uint32(0, static_cast<uint32_t>(max_val - min_val))) + min_val;
	}

	float range_float(float min_val, float max_val) {
		return min_val + next_float() * (max_val - min_val);
	}

	double range_double(double min_val, double max_val) {
		return min_val + next_double() * (max_val - min_val);
	}

	bool chance(float probability) {
		return next_float() < probability;
	}

	void reset() { seed_(initial_seed_, initial_stream_); }

	static constexpr uint32_t min() { return 0; }
	static constexpr uint32_t max() { return std::numeric_limits<uint32_t>::max(); }

private:
	uint64_t state_;
	uint64_t inc_;
	uint64_t initial_seed_;
	uint64_t initial_stream_;

	void seed_(uint64_t seed, uint64_t stream) {
		state_ = 0u;
		inc_ = (stream << 1u) | 1u;
		(*this)();
		state_ += seed;
		(*this)();
	}
};

class Xoroshiro128Plus {
public:
	using result_type = uint64_t;

	static constexpr uint64_t default_seed = 0x9e3779b97f4a7c15ull;

	explicit Xoroshiro128Plus(uint64_t seed = default_seed) :
			initial_seed_(seed) {
		seed_(seed);
	}

	void seed(uint64_t s) {
		initial_seed_ = s;
		seed_(s);
	}

	uint64_t operator()() {
		const uint64_t s0 = s_[0];
		uint64_t s1 = s_[1];
		const uint64_t result = s0 + s1;

		s1 ^= s0;
		s_[0] = rotl_(s0, 55) ^ s1 ^ (s1 << 14);
		s_[1] = rotl_(s1, 36);

		return result;
	}

	uint32_t next_uint32() {
		return static_cast<uint32_t>((*this)());
	}

	uint64_t next_uint64() { return (*this)(); }

	float next_float() {
		return static_cast<float>((*this)() >> 40) * (1.0f / 16777216.0f);
	}

	double next_double() {
		return static_cast<double>((*this)() >> 11) * (1.0 / 9007199254740992.0);
	}

	uint32_t range_uint32(uint32_t min_val, uint32_t max_val) {
		if (min_val >= max_val) {
			return min_val;
		}
		uint32_t range = max_val - min_val + 1;

		uint32_t threshold = (std::numeric_limits<uint32_t>::max() / range) * range;
		uint32_t result;
		do {
			result = next_uint32();
		} while (result >= threshold);

		return min_val + (result % range);
	}

	int32_t range_int32(int32_t min_val, int32_t max_val) {
		if (min_val >= max_val) {
			return min_val;
		}
		return static_cast<int32_t>(range_uint32(0, static_cast<uint32_t>(max_val - min_val))) + min_val;
	}

	uint64_t range_uint64(uint64_t min_val, uint64_t max_val) {
		if (min_val >= max_val) {
			return min_val;
		}
		uint64_t range = max_val - min_val + 1;

		uint64_t threshold = (std::numeric_limits<uint64_t>::max() / range) * range;
		uint64_t result;
		do {
			result = (*this)();
		} while (result >= threshold);

		return min_val + (result % range);
	}

	int64_t range_int64(int64_t min_val, int64_t max_val) {
		if (min_val >= max_val) {
			return min_val;
		}
		return static_cast<int64_t>(range_uint64(0, static_cast<uint64_t>(max_val - min_val))) + min_val;
	}

	float range_float(float min_val, float max_val) {
		return min_val + next_float() * (max_val - min_val);
	}

	double range_double(double min_val, double max_val) {
		return min_val + next_double() * (max_val - min_val);
	}

	bool chance(float probability) {
		return next_float() < probability;
	}

	void jump() {
		static const uint64_t JUMP[] = { 0xbeac0467eba5facb, 0xd86b048b86aa9922 };

		uint64_t s0 = 0;
		uint64_t s1 = 0;

		for (int i = 0; i < 2; ++i) {
			for (int b = 0; b < 64; ++b) {
				if ((JUMP[i] >> b) & 1) {
					s0 ^= s_[0];
					s1 ^= s_[1];
				}
				(*this)();
			}
		}

		s_[0] = s0;
		s_[1] = s1;
	}

	void reset() { seed_(initial_seed_); }

	static constexpr uint64_t min() { return 0; }
	static constexpr uint64_t max() { return std::numeric_limits<uint64_t>::max(); }

private:
	uint64_t s_[2];
	uint64_t initial_seed_;

	static uint64_t rotl_(uint64_t x, int k) {
		return (x << k) | (x >> (64 - k));
	}

	void seed_(uint64_t seed) {
		uint64_t z = seed + 0x9e3779b97f4a7c15ull;
		z = (z ^ (z >> 30)) * 0xbf58476d1ce4e5b9ull;
		z = (z ^ (z >> 27)) * 0x94d049bb133111ebull;
		s_[0] = z ^ (z >> 31);

		z = s_[0] + 0x9e3779b97f4a7c15ull;
		z = (z ^ (z >> 30)) * 0xbf58476d1ce4e5b9ull;
		z = (z ^ (z >> 27)) * 0x94d049bb133111ebull;
		s_[1] = z ^ (z >> 31);
	}
};

class SplitMix64 {
public:
	using result_type = uint64_t;

	static constexpr uint64_t default_seed = 0x9e3779b97f4a7c15ull;

	explicit SplitMix64(uint64_t seed = default_seed) :
			state_(seed), initial_seed_(seed) {}

	void seed(uint64_t s) {
		state_ = s;
		initial_seed_ = s;
	}

	uint64_t operator()() {
		uint64_t z = (state_ += 0x9e3779b97f4a7c15ull);
		z = (z ^ (z >> 30)) * 0xbf58476d1ce4e5b9ull;
		z = (z ^ (z >> 27)) * 0x94d049bb133111ebull;
		return z ^ (z >> 31);
	}

	uint32_t next_uint32() {
		return static_cast<uint32_t>((*this)());
	}

	uint64_t next_uint64() { return (*this)(); }

	float next_float() {
		return static_cast<float>((*this)() >> 40) * (1.0f / 16777216.0f);
	}

	double next_double() {
		return static_cast<double>((*this)() >> 11) * (1.0 / 9007199254740992.0);
	}

	uint64_t range_uint64(uint64_t min_val, uint64_t max_val) {
		if (min_val >= max_val) {
			return min_val;
		}
		uint64_t range = max_val - min_val + 1;

		uint64_t threshold = (std::numeric_limits<uint64_t>::max() / range) * range;
		uint64_t result;
		do {
			result = (*this)();
		} while (result >= threshold);

		return min_val + (result % range);
	}

	float range_float(float min_val, float max_val) {
		return min_val + next_float() * (max_val - min_val);
	}

	double range_double(double min_val, double max_val) {
		return min_val + next_double() * (max_val - min_val);
	}

	bool chance(float probability) {
		return next_float() < probability;
	}

	void reset() { state_ = initial_seed_; }

	static constexpr uint64_t min() { return 0; }
	static constexpr uint64_t max() { return std::numeric_limits<uint64_t>::max(); }

private:
	uint64_t state_;
	uint64_t initial_seed_;
};

using RandomLCG = LCG32;
using RandomPCG = PCG32;
using RandomXoroshiro = Xoroshiro128Plus;
using RandomSplitMix = SplitMix64;

} 






template <typename T>
struct Vector3SoA {
	std::vector<T> x;
	std::vector<T> y;
	std::vector<T> z;

	explicit Vector3SoA(size_t count = 0) :
			x(count), y(count), z(count) {}

	void resize(size_t count) {
		x.resize(count);
		y.resize(count);
		z.resize(count);
	}

	size_t size() const { return x.size(); }

	void clear() {
		x.clear();
		y.clear();
		z.clear();
	}

	void from_aos(const std::vector<Vector3<T>> &aos) {
		resize(aos.size());
		for (size_t i = 0; i < aos.size(); ++i) {
			x[i] = aos[i].x;
			y[i] = aos[i].y;
			z[i] = aos[i].z;
		}
	}

	std::vector<Vector3<T>> to_aos() const {
		std::vector<Vector3<T>> result(size());
		for (size_t i = 0; i < size(); ++i) {
			result[i] = Vector3<T>(x[i], y[i], z[i]);
		}
		return result;
	}

	void set(size_t i, const Vector3<T> &v) {
		x[i] = v.x;
		y[i] = v.y;
		z[i] = v.z;
	}

	Vector3<T> get(size_t i) const {
		return Vector3<T>(x[i], y[i], z[i]);
	}

	void add(const Vector3SoA &other) {
		const size_t n = size();
#if MML_SIMD_SSE2 && defined(__SSE2__)
		size_t i = 0;
		for (; i + 3 < n; i += 4) {
			__m128 xv = _mm_loadu_ps(&x[i]);
			__m128 yv = _mm_loadu_ps(&y[i]);
			__m128 zv = _mm_loadu_ps(&z[i]);
			__m128 ox = _mm_loadu_ps(&other.x[i]);
			__m128 oy = _mm_loadu_ps(&other.y[i]);
			__m128 oz = _mm_loadu_ps(&other.z[i]);
			_mm_storeu_ps(&x[i], _mm_add_ps(xv, ox));
			_mm_storeu_ps(&y[i], _mm_add_ps(yv, oy));
			_mm_storeu_ps(&z[i], _mm_add_ps(zv, oz));
		}
		for (; i < n; ++i) {
			x[i] += other.x[i];
			y[i] += other.y[i];
			z[i] += other.z[i];
		}
#else
		for (size_t i = 0; i < n; ++i) {
			x[i] += other.x[i];
			y[i] += other.y[i];
			z[i] += other.z[i];
		}
#endif
	}

	void scale(T scalar) {
		const size_t n = size();
#if MML_SIMD_SSE2 && defined(__SSE2__)
		__m128 sv = _mm_set1_ps(scalar);
		size_t i = 0;
		for (; i + 3 < n; i += 4) {
			_mm_storeu_ps(&x[i], _mm_mul_ps(_mm_loadu_ps(&x[i]), sv));
			_mm_storeu_ps(&y[i], _mm_mul_ps(_mm_loadu_ps(&y[i]), sv));
			_mm_storeu_ps(&z[i], _mm_mul_ps(_mm_loadu_ps(&z[i]), sv));
		}
		for (; i < n; ++i) {
			x[i] *= scalar;
			y[i] *= scalar;
			z[i] *= scalar;
		}
#else
		for (size_t i = 0; i < n; ++i) {
			x[i] *= scalar;
			y[i] *= scalar;
			z[i] *= scalar;
		}
#endif
	}

	std::vector<T> dot(const Vector3SoA &other) const {
		const size_t n = size();
		std::vector<T> result(n);
#if MML_SIMD_SSE2 && defined(__SSE2__)
		size_t i = 0;
		for (; i + 3 < n; i += 4) {
			__m128 xv = _mm_loadu_ps(&x[i]);
			__m128 yv = _mm_loadu_ps(&y[i]);
			__m128 zv = _mm_loadu_ps(&z[i]);
			__m128 ox = _mm_loadu_ps(&other.x[i]);
			__m128 oy = _mm_loadu_ps(&other.y[i]);
			__m128 oz = _mm_loadu_ps(&other.z[i]);
			__m128 dot = _mm_add_ps(_mm_add_ps(_mm_mul_ps(xv, ox), _mm_mul_ps(yv, oy)), _mm_mul_ps(zv, oz));
			_mm_storeu_ps(&result[i], dot);
		}
		for (; i < n; ++i) {
			result[i] = x[i] * other.x[i] + y[i] * other.y[i] + z[i] * other.z[i];
		}
#else
		for (size_t i = 0; i < n; ++i) {
			result[i] = x[i] * other.x[i] + y[i] * other.y[i] + z[i] * other.z[i];
		}
#endif
		return result;
	}

	std::vector<T> length_squared() const {
		const size_t n = size();
		std::vector<T> result(n);
#if MML_SIMD_SSE2 && defined(__SSE2__)
		size_t i = 0;
		for (; i + 3 < n; i += 4) {
			__m128 xv = _mm_loadu_ps(&x[i]);
			__m128 yv = _mm_loadu_ps(&y[i]);
			__m128 zv = _mm_loadu_ps(&z[i]);
			__m128 len = _mm_add_ps(_mm_add_ps(_mm_mul_ps(xv, xv), _mm_mul_ps(yv, yv)), _mm_mul_ps(zv, zv));
			_mm_storeu_ps(&result[i], len);
		}
		for (; i < n; ++i) {
			result[i] = x[i] * x[i] + y[i] * y[i] + z[i] * z[i];
		}
#else
		for (size_t i = 0; i < n; ++i) {
			result[i] = x[i] * x[i] + y[i] * y[i] + z[i] * z[i];
		}
#endif
		return result;
	}

	void normalize() {
		const size_t n = size();
		for (size_t i = 0; i < n; ++i) {
			T len = std::sqrt(x[i] * x[i] + y[i] * y[i] + z[i] * z[i]);
			if (len > Epsilon<T>::value) {
				T inv = T(1) / len;
				x[i] *= inv;
				y[i] *= inv;
				z[i] *= inv;
			}
		}
	}

	void transform(const Matrix4<T> &matrix) {
		const size_t n = size();
		for (size_t i = 0; i < n; ++i) {
			Vector4<T> v(x[i], y[i], z[i], T(1));
			Vector4<T> result = matrix * v;
			x[i] = result.x;
			y[i] = result.y;
			z[i] = result.z;
		}
	}

	void cross(const Vector3SoA &other) {
		const size_t n = size();
		std::vector<T> nx(n), ny(n), nz(n);
#if MML_SIMD_SSE2 && defined(__SSE2__)
		size_t i = 0;
		for (; i + 3 < n; i += 4) {
			__m128 xv = _mm_loadu_ps(&x[i]);
			__m128 yv = _mm_loadu_ps(&y[i]);
			__m128 zv = _mm_loadu_ps(&z[i]);
			__m128 ox = _mm_loadu_ps(&other.x[i]);
			__m128 oy = _mm_loadu_ps(&other.y[i]);
			__m128 oz = _mm_loadu_ps(&other.z[i]);
			__m128 cx = _mm_sub_ps(_mm_mul_ps(yv, oz), _mm_mul_ps(zv, oy));
			__m128 cy = _mm_sub_ps(_mm_mul_ps(zv, ox), _mm_mul_ps(xv, oz));
			__m128 cz = _mm_sub_ps(_mm_mul_ps(xv, oy), _mm_mul_ps(yv, ox));
			_mm_storeu_ps(&nx[i], cx);
			_mm_storeu_ps(&ny[i], cy);
			_mm_storeu_ps(&nz[i], cz);
		}
		for (; i < n; ++i) {
			nx[i] = y[i] * other.z[i] - z[i] * other.y[i];
			ny[i] = z[i] * other.x[i] - x[i] * other.z[i];
			nz[i] = x[i] * other.y[i] - y[i] * other.x[i];
		}
#else
		for (size_t i = 0; i < n; ++i) {
			nx[i] = y[i] * other.z[i] - z[i] * other.y[i];
			ny[i] = z[i] * other.x[i] - x[i] * other.z[i];
			nz[i] = x[i] * other.y[i] - y[i] * other.x[i];
		}
#endif
		x = std::move(nx);
		y = std::move(ny);
		z = std::move(nz);
	}
};

using Vector3SoAf = Vector3SoA<float>;
using Vector3SoAd = Vector3SoA<double>;

template <typename T>
struct Vector4SoA {
	std::vector<T> x;
	std::vector<T> y;
	std::vector<T> z;
	std::vector<T> w;

	explicit Vector4SoA(size_t count = 0) :
			x(count), y(count), z(count), w(count) {}

	void resize(size_t count) {
		x.resize(count);
		y.resize(count);
		z.resize(count);
		w.resize(count);
	}

	size_t size() const { return x.size(); }

	void clear() {
		x.clear();
		y.clear();
		z.clear();
		w.clear();
	}

	void from_aos(const std::vector<Vector4<T>> &aos) {
		resize(aos.size());
		for (size_t i = 0; i < aos.size(); ++i) {
			x[i] = aos[i].x;
			y[i] = aos[i].y;
			z[i] = aos[i].z;
			w[i] = aos[i].w;
		}
	}

	std::vector<Vector4<T>> to_aos() const {
		std::vector<Vector4<T>> result(size());
		for (size_t i = 0; i < size(); ++i) {
			result[i] = Vector4<T>(x[i], y[i], z[i], w[i]);
		}
		return result;
	}

	void set(size_t i, const Vector4<T> &v) {
		x[i] = v.x;
		y[i] = v.y;
		z[i] = v.z;
		w[i] = v.w;
	}

	Vector4<T> get(size_t i) const {
		return Vector4<T>(x[i], y[i], z[i], w[i]);
	}

	void add(const Vector4SoA &other) {
		const size_t n = size();
#if MML_SIMD_SSE2 && defined(__SSE2__)
		size_t i = 0;
		for (; i + 3 < n; i += 4) {
			_mm_storeu_ps(&x[i], _mm_add_ps(_mm_loadu_ps(&x[i]), _mm_loadu_ps(&other.x[i])));
			_mm_storeu_ps(&y[i], _mm_add_ps(_mm_loadu_ps(&y[i]), _mm_loadu_ps(&other.y[i])));
			_mm_storeu_ps(&z[i], _mm_add_ps(_mm_loadu_ps(&z[i]), _mm_loadu_ps(&other.z[i])));
			_mm_storeu_ps(&w[i], _mm_add_ps(_mm_loadu_ps(&w[i]), _mm_loadu_ps(&other.w[i])));
		}
		for (; i < n; ++i) {
			x[i] += other.x[i];
			y[i] += other.y[i];
			z[i] += other.z[i];
			w[i] += other.w[i];
		}
#else
		for (size_t i = 0; i < n; ++i) {
			x[i] += other.x[i];
			y[i] += other.y[i];
			z[i] += other.z[i];
			w[i] += other.w[i];
		}
#endif
	}

	std::vector<T> dot(const Vector4SoA &other) const {
		const size_t n = size();
		std::vector<T> result(n);
#if MML_SIMD_SSE2 && defined(__SSE2__)
		size_t i = 0;
		for (; i + 3 < n; i += 4) {
			__m128 dot = _mm_add_ps(
					_mm_add_ps(_mm_mul_ps(_mm_loadu_ps(&x[i]), _mm_loadu_ps(&other.x[i])),
							_mm_mul_ps(_mm_loadu_ps(&y[i]), _mm_loadu_ps(&other.y[i]))),
					_mm_add_ps(_mm_mul_ps(_mm_loadu_ps(&z[i]), _mm_loadu_ps(&other.z[i])),
							_mm_mul_ps(_mm_loadu_ps(&w[i]), _mm_loadu_ps(&other.w[i]))));
			_mm_storeu_ps(&result[i], dot);
		}
		for (; i < n; ++i) {
			result[i] = x[i] * other.x[i] + y[i] * other.y[i] + z[i] * other.z[i] + w[i] * other.w[i];
		}
#else
		for (size_t i = 0; i < n; ++i) {
			result[i] = x[i] * other.x[i] + y[i] * other.y[i] + z[i] * other.z[i] + w[i] * other.w[i];
		}
#endif
		return result;
	}

	void transform(const Matrix4<T> &matrix) {
		const size_t n = size();
		for (size_t i = 0; i < n; ++i) {
			Vector4<T> v(x[i], y[i], z[i], w[i]);
			Vector4<T> result = matrix * v;
			x[i] = result.x;
			y[i] = result.y;
			z[i] = result.z;
			w[i] = result.w;
		}
	}
};

using Vector4SoAf = Vector4SoA<float>;
using Vector4SoAd = Vector4SoA<double>;

template <typename T>
struct BatchTransform {
	Vector3SoA<T> positions;
	std::vector<Quaternion<T>> rotations;
	Vector3SoA<T> scales;

	explicit BatchTransform(size_t count = 0) :
			positions(count), rotations(count), scales(count) {
		for (size_t i = 0; i < count; ++i) {
			scales.x[i] = scales.y[i] = scales.z[i] = T(1);
		}
	}

	void resize(size_t count) {
		size_t old_size = positions.size();
		positions.resize(count);
		rotations.resize(count);
		scales.resize(count);

		for (size_t i = old_size; i < count; ++i) {
			scales.x[i] = scales.y[i] = scales.z[i] = T(1);
			rotations[i] = Quaternion<T>::identity();
		}
	}

	size_t size() const { return positions.size(); }

	std::vector<Matrix4<T>> compute_matrices() const {
		std::vector<Matrix4<T>> matrices(size());
		for (size_t i = 0; i < size(); ++i) {
			Matrix4<T> s = Matrix4<T>::scale(scales.x[i], scales.y[i], scales.z[i]);
			Matrix4<T> r = rotations[i].to_matrix4();
			Matrix4<T> t = Matrix4<T>::translation(positions.x[i], positions.y[i], positions.z[i]);
			matrices[i] = t * r * s;
		}
		return matrices;
	}

	Vector3SoA<T> transform_points(const Vector3SoA<T> &points) const {
		Vector3SoA<T> result(points.size());
		for (size_t i = 0; i < size(); ++i) {
			Matrix4<T> matrix = compute_matrix(i);
			for (size_t j = 0; j < points.size(); ++j) {
				Vector4<T> p(points.x[j], points.y[j], points.z[j], T(1));
				Vector4<T> transformed = matrix * p;
				result.x[j] = transformed.x;
				result.y[j] = transformed.y;
				result.z[j] = transformed.z;
			}
		}
		return result;
	}

	Matrix4<T> compute_matrix(size_t i) const {
		Matrix4<T> s = Matrix4<T>::scale(scales.x[i], scales.y[i], scales.z[i]);
		Matrix4<T> r = rotations[i].to_matrix4();
		Matrix4<T> t = Matrix4<T>::translation(positions.x[i], positions.y[i], positions.z[i]);
		return t * r * s;
	}

	void translate(const Vector3<T> &delta) {
		const size_t n = size();
#if MML_SIMD_SSE2 && defined(__SSE2__)
		__m128 dx = _mm_set1_ps(delta.x);
		__m128 dy = _mm_set1_ps(delta.y);
		__m128 dz = _mm_set1_ps(delta.z);
		size_t i = 0;
		for (; i + 3 < n; i += 4) {
			_mm_storeu_ps(&positions.x[i], _mm_add_ps(_mm_loadu_ps(&positions.x[i]), dx));
			_mm_storeu_ps(&positions.y[i], _mm_add_ps(_mm_loadu_ps(&positions.y[i]), dy));
			_mm_storeu_ps(&positions.z[i], _mm_add_ps(_mm_loadu_ps(&positions.z[i]), dz));
		}
		for (; i < n; ++i) {
			positions.x[i] += delta.x;
			positions.y[i] += delta.y;
			positions.z[i] += delta.z;
		}
#else
		for (size_t i = 0; i < n; ++i) {
			positions.x[i] += delta.x;
			positions.y[i] += delta.y;
			positions.z[i] += delta.z;
		}
#endif
	}

	void rotate(const Quaternion<T> &q) {
		for (size_t i = 0; i < size(); ++i) {
			rotations[i] = q * rotations[i];
		}
	}

	void scale(T s) {
		const size_t n = size();
#if MML_SIMD_SSE2 && defined(__SSE2__)
		__m128 sv = _mm_set1_ps(s);
		size_t i = 0;
		for (; i + 3 < n; i += 4) {
			_mm_storeu_ps(&scales.x[i], _mm_mul_ps(_mm_loadu_ps(&scales.x[i]), sv));
			_mm_storeu_ps(&scales.y[i], _mm_mul_ps(_mm_loadu_ps(&scales.y[i]), sv));
			_mm_storeu_ps(&scales.z[i], _mm_mul_ps(_mm_loadu_ps(&scales.z[i]), sv));
		}
		for (; i < n; ++i) {
			scales.x[i] *= s;
			scales.y[i] *= s;
			scales.z[i] *= s;
		}
#else
		for (size_t i = 0; i < n; ++i) {
			scales.x[i] *= s;
			scales.y[i] *= s;
			scales.z[i] *= s;
		}
#endif
	}
};

using BatchTransformf = BatchTransform<float>;
using BatchTransformd = BatchTransform<double>;

template <typename T>
struct BatchAABB {
	Vector3SoA<T> min;
	Vector3SoA<T> max;

	explicit BatchAABB(size_t count = 0) :
			min(count), max(count) {}

	void resize(size_t count) {
		min.resize(count);
		max.resize(count);
	}

	size_t size() const { return min.size(); }

	Vector3SoA<T> centers() const {
		Vector3SoA<T> result(size());
		const size_t n = size();
		for (size_t i = 0; i < n; ++i) {
			result.x[i] = (min.x[i] + max.x[i]) * T(0.5);
			result.y[i] = (min.y[i] + max.y[i]) * T(0.5);
			result.z[i] = (min.z[i] + max.z[i]) * T(0.5);
		}
		return result;
	}

	Vector3SoA<T> extents() const {
		Vector3SoA<T> result(size());
		const size_t n = size();
		for (size_t i = 0; i < n; ++i) {
			result.x[i] = max.x[i] - min.x[i];
			result.y[i] = max.y[i] - min.y[i];
			result.z[i] = max.z[i] - min.z[i];
		}
		return result;
	}

	void transform(const Matrix4<T> &matrix) {
		const size_t n = size();
		for (size_t i = 0; i < n; ++i) {
			AABB<T> aabb(
					Vector3<T>(min.x[i], min.y[i], min.z[i]),
					Vector3<T>(max.x[i], max.y[i], max.z[i]));
			AABB<T> transformed = aabb.transformed(matrix);
			min.x[i] = transformed.min.x;
			min.y[i] = transformed.min.y;
			min.z[i] = transformed.min.z;
			max.x[i] = transformed.max.x;
			max.y[i] = transformed.max.y;
			max.z[i] = transformed.max.z;
		}
	}

	std::vector<bool> contains(const Vector3<T> &point) const {
		std::vector<bool> result(size());
		const size_t n = size();
		for (size_t i = 0; i < n; ++i) {
			result[i] = point.x >= min.x[i] && point.x <= max.x[i] &&
					point.y >= min.y[i] && point.y <= max.y[i] &&
					point.z >= min.z[i] && point.z <= max.z[i];
		}
		return result;
	}
};

using BatchAABBf = BatchAABB<float>;
using BatchAABBd = BatchAABB<double>;

} // namespace mml

#endif // MML_HPP
