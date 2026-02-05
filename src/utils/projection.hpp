/**************************************************************************/
/*  projection.hpp                                                        */
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
#include "../core/config.hpp"
#include "../core/constants.hpp"
#include <cmath>
#include <type_traits>

namespace mml {

// Projection matrix utilities (right-handed coordinate system)
template <typename T>
struct Projection {
	static_assert(std::is_floating_point_v<T>, "Projection requires floating-point type");

	// Create perspective projection matrix (field of view in radians)
	// right-handed, depth range [0, 1] (Vulkan/D3D style)
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

	// Create perspective projection matrix (OpenGL style, depth range [-1, 1])
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

	// Create infinite perspective projection (no far plane)
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

	// Create orthographic projection matrix
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

	// Create orthographic projection matrix (OpenGL style)
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

	// Create orthographic projection centered at origin
	static Matrix4<T> orthographic_centered(T width, T height, T near_plane, T far_plane) {
		T half_width = width * T(0.5);
		T half_height = height * T(0.5);
		return orthographic(-half_width, half_width, -half_height, half_height, near_plane, far_plane);
	}

	// Create look-at view matrix (right-handed)
	static Matrix4<T> look_at(const Vector3<T> &eye, const Vector3<T> &target, const Vector3<T> &up) {
		Vector3<T> forward = (target - eye).normalized();
		Vector3<T> right = forward.cross(up).normalized();
		Vector3<T> new_up = right.cross(forward);

		Matrix4<T> result = Matrix4<T>::identity();

		// Set rotation part
		result[0][0] = right.x;
		result[1][0] = right.y;
		result[2][0] = right.z;

		result[0][1] = new_up.x;
		result[1][1] = new_up.y;
		result[2][1] = new_up.z;

		result[0][2] = -forward.x;
		result[1][2] = -forward.y;
		result[2][2] = -forward.z;

		// Set translation part
		result[3][0] = -right.dot(eye);
		result[3][1] = -new_up.dot(eye);
		result[3][2] = forward.dot(eye);

		return result;
	}

	// Extract field of view from perspective projection matrix
	static T extract_fov_y(const Matrix4<T> &proj) {
		return T(2) * std::atan(T(1) / proj[1][1]);
	}

	// Extract aspect ratio from perspective projection matrix
	static T extract_aspect_ratio(const Matrix4<T> &proj) {
		return proj[1][1] / proj[0][0];
	}
};

// Type aliases
using Projectionf = Projection<float>;
using Projectiond = Projection<double>;

} // namespace mml