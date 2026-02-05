/**************************************************************************/
/*  simd.hpp                                                              */
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

#include "config.hpp"
#include "traits.hpp"
#include <type_traits>

namespace mml {
namespace simd {

#if MML_SIMD_SSE2 && defined(__SSE2__)

#include <emmintrin.h>
#if MML_SIMD_SSE4_1
#include <smmintrin.h>
#endif

// 128-bit float vector (4 floats)
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

// 128-bit double vector (2 doubles)
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
#include <immintrin.h>

// 256-bit float vector (8 floats)
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

	// 256-bit double vector (4 doubles)
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

// Fallback scalar implementation
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

} // namespace simd
} // namespace mml