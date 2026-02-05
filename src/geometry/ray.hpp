/**************************************************************************/
/*  ray.hpp                                                               */
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
#include <type_traits>

namespace mml {

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

} //namespace mml