/**************************************************************************/
/*  soa.hpp                                                               */
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

#include "../algebra/matrix4.hpp"
#include "../algebra/quaternion.hpp"
#include "../algebra/vector3.hpp"
#include "../algebra/vector4.hpp"
#include "../core/config.hpp"
#include "../core/simd.hpp"
#include "../geometry/aabb.hpp"
#include <algorithm>
#include <memory>
#include <vector>

namespace mml {

// ============================================================================
// Structure of Arrays (SoA) for Vector3 - optimal for batch processing
// ============================================================================

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

	// Set from AoS (Array of Structures)
	void from_aos(const std::vector<Vector3<T>> &aos) {
		resize(aos.size());
		for (size_t i = 0; i < aos.size(); ++i) {
			x[i] = aos[i].x;
			y[i] = aos[i].y;
			z[i] = aos[i].z;
		}
	}

	// Convert to AoS
	std::vector<Vector3<T>> to_aos() const {
		std::vector<Vector3<T>> result(size());
		for (size_t i = 0; i < size(); ++i) {
			result[i] = Vector3<T>(x[i], y[i], z[i]);
		}
		return result;
	}

	// Set single element
	void set(size_t i, const Vector3<T> &v) {
		x[i] = v.x;
		y[i] = v.y;
		z[i] = v.z;
	}

	// Get single element
	Vector3<T> get(size_t i) const {
		return Vector3<T>(x[i], y[i], z[i]);
	}

	// Batch addition
	void add(const Vector3SoA &other) {
		const size_t n = size();
#if MML_SIMD_SSE2 && defined(__SSE2__)
		// Process 4 elements at a time with SIMD
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
		// Scalar remainder
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

	// Batch scalar multiplication
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

	// Batch dot products
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

	// Batch length squared
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

	// Batch normalize
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

	// Batch transform by matrix
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

	// Batch cross product
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

// ============================================================================
// Structure of Arrays (SoA) for Vector4
// ============================================================================

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

	// Batch addition
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

	// Batch dot products
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

	// Batch matrix transform
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

// ============================================================================
// Batch Transform Operations for ECS
// ============================================================================

template <typename T>
struct BatchTransform {
	Vector3SoA<T> positions;
	std::vector<Quaternion<T>> rotations;
	Vector3SoA<T> scales;

	explicit BatchTransform(size_t count = 0) :
			positions(count), rotations(count), scales(count) {
		// Initialize scales to 1
		for (size_t i = 0; i < count; ++i) {
			scales.x[i] = scales.y[i] = scales.z[i] = T(1);
		}
	}

	void resize(size_t count) {
		size_t old_size = positions.size();
		positions.resize(count);
		rotations.resize(count);
		scales.resize(count);

		// Initialize new scales to 1
		for (size_t i = old_size; i < count; ++i) {
			scales.x[i] = scales.y[i] = scales.z[i] = T(1);
			rotations[i] = Quaternion<T>::identity();
		}
	}

	size_t size() const { return positions.size(); }

	// Compute matrices for all transforms
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

	// Transform array of points by all transforms
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

	// Translate all positions
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

	// Rotate all by quaternion
	void rotate(const Quaternion<T> &q) {
		for (size_t i = 0; i < size(); ++i) {
			rotations[i] = q * rotations[i];
		}
	}

	// Scale all uniformly
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

// ============================================================================
// Batch AABB Operations
// ============================================================================

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

	// Compute centers
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

	// Compute extents
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

	// Transform all AABBs
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

	// Check if point is inside any AABB
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