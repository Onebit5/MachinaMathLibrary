/**************************************************************************/
/*  test_quaternion.cpp                                                   */
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

#include "../src/mml.hpp"
#include <cassert>
#include <cmath>
#include <iostream>

namespace mml_test {

void test_quaternion() {
	std::cout << "\n=== Quaternion Tests ===" << std::endl;

	mml::Quaternionf q_identity = mml::Quaternionf::identity();
	assert(std::abs(q_identity.w - 1.0f) < 0.001f);
	assert(std::abs(q_identity.x) < 0.001f);
	assert(std::abs(q_identity.y) < 0.001f);
	assert(std::abs(q_identity.z) < 0.001f);

	mml::Quaternionf q_axis = mml::Quaternionf::from_axis_angle(
			mml::Vector3f(0.0f, 0.0f, 1.0f), mml::Constantsf::half_pi);
	assert(std::abs(q_axis.length() - 1.0f) < 0.001f);

	float angle = q_axis.angle();
	assert(std::abs(angle - mml::Constantsf::half_pi) < 0.01f);

	mml::Vector3f axis;
	float out_angle;
	q_axis.to_axis_angle(axis, out_angle);
	assert(std::abs(axis.z - 1.0f) < 0.001f);
	assert(std::abs(out_angle - mml::Constantsf::half_pi) < 0.01f);

	mml::Quaternionf q_conj = q_axis.conjugate();
	assert(std::abs(q_conj.w - q_axis.w) < 0.001f);
	assert(std::abs(q_conj.x + q_axis.x) < 0.001f);

	mml::Vector3f v(1.0f, 0.0f, 0.0f);
	mml::Vector3f rotated = q_axis.rotate(v);
	assert(std::abs(rotated.x) < 0.1f);
	assert(std::abs(rotated.y) < 0.1f);

	mml::Quaternionf q1 = mml::Quaternionf::identity();
	mml::Quaternionf q2 = mml::Quaternionf::from_axis_angle(mml::Vector3f::unit_y(), mml::Constantsf::pi);
	mml::Quaternionf slerped = mml::Quaternionf::slerp(q1, q2, 0.5f);
	assert(std::abs(slerped.length() - 1.0f) < 0.01f);

	mml::Quaternionf q_from_euler = mml::Quaternionf::from_euler(0.5f, 0.3f, 0.2f);
	assert(std::abs(q_from_euler.length() - 1.0f) < 0.01f);

	mml::Quaternionf rot_between = mml::Quaternionf::rotation_between(mml::Vector3f::unit_x(), mml::Vector3f::unit_y());
	assert(std::abs(rot_between.length() - 1.0f) < 0.01f);

	float angle_to = q1.angle_to(q2);
	assert(std::abs(angle_to - mml::Constantsf::pi) < 0.01f);

	std::cout << "  [PASS] Quaternion identity" << std::endl;
	std::cout << "  [PASS] Quaternion from_axis_angle" << std::endl;
	std::cout << "  [PASS] Quaternion to_axis_angle" << std::endl;
	std::cout << "  [PASS] Quaternion rotate" << std::endl;
	std::cout << "  [PASS] Quaternion slerp" << std::endl;
	std::cout << "  [PASS] Quaternion from_euler" << std::endl;
	std::cout << "  [PASS] Quaternion rotation_between" << std::endl;
	std::cout << "  [PASS] Quaternion angle_to" << std::endl;
}

void run_quaternion_tests() {
	test_quaternion();
	std::cout << "\nAll quaternion tests passed!" << std::endl;
}

} // namespace mml_test