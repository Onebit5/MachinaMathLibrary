/**************************************************************************/
/*  basis.hpp                                                             */
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

#include "../algebra/matrix3.hpp"
#include "../algebra/matrix4.hpp"
#include "../algebra/quaternion.hpp"
#include "../algebra/vector3.hpp"
#include "../core/config.hpp"
#include <type_traits>

namespace mml {

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

} //namespace mml