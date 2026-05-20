/**************************************************************************/
/*  vector4.hpp                                                           */
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

#include "../core/config.hpp"
#include "../core/epsilon.hpp"
#include "../core/simd.hpp"
#include "../utils/math.hpp"
#include "vector2.hpp"
#include "vector3.hpp"
#include <cmath>
#include <type_traits>

namespace mml {

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

} //namespace mml