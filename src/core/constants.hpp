/**************************************************************************/
/*  constants.hpp                                                         */
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
#include <limits>

namespace mml {

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

} //namespace mml