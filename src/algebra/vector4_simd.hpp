/**************************************************************************/
/*  vector4_simd.hpp                                                      */
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

#include "../algebra/vector4.hpp"
#include "../core/config.hpp"
#include "../core/simd.hpp"
#include <type_traits>

namespace mml {
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

} //namespace batch

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

} //namespace batch

#endif

} //namespace simd
} //namespace mml