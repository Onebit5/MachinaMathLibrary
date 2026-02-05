#pragma once

#include "../src/mml.hpp"
#include <cassert>
#include <cmath>
#include <iostream>

namespace mml_test {

void test_matrix3() {
	std::cout << "\n=== Matrix3 Tests ===" << std::endl;

	mml::Matrix3f identity = mml::Matrix3f::identity();
	assert(std::abs(identity[0][0] - 1.0f) < 0.001f);
	assert(std::abs(identity[1][1] - 1.0f) < 0.001f);
	assert(std::abs(identity[2][2] - 1.0f) < 0.001f);

	mml::Matrix3f m(1.0f, 0.0f, 0.0f, 0.0f, 2.0f, 0.0f, 0.0f, 0.0f, 3.0f);
	float det = m.determinant();
	assert(std::abs(det - 6.0f) < 0.001f);

	mml::Matrix3f inv = m.inverse();
	mml::Matrix3f prod = m * inv;
	assert(std::abs(prod[0][0] - 1.0f) < 0.01f);
	assert(std::abs(prod[1][1] - 1.0f) < 0.01f);

	mml::Matrix3f frob = mml::Matrix3f::identity();
	float norm = frob.frobenius_norm();
	assert(std::abs(norm - 1.732f) < 0.01f);

	std::cout << "  [PASS] Matrix3 identity" << std::endl;
	std::cout << "  [PASS] Matrix3 determinant" << std::endl;
	std::cout << "  [PASS] Matrix3 inverse" << std::endl;
	std::cout << "  [PASS] Matrix3 frobenius norm" << std::endl;
}

void test_matrix4() {
	std::cout << "\n=== Matrix4 Tests ===" << std::endl;

	mml::Matrix4f identity = mml::Matrix4f::identity();
	assert(std::abs(identity[0][0] - 1.0f) < 0.001f);
	assert(std::abs(identity[3][3] - 1.0f) < 0.001f);

	mml::Matrix4f trans = mml::Matrix4f::translation(1.0f, 2.0f, 3.0f);
	assert(std::abs(trans[3][0] - 1.0f) < 0.001f);
	assert(std::abs(trans[3][1] - 2.0f) < 0.001f);
	assert(std::abs(trans[3][2] - 3.0f) < 0.001f);

	mml::Vector4f point(0.0f, 0.0f, 0.0f, 1.0f);
	mml::Vector4f translated = trans * point;
	assert(std::abs(translated.x - 1.0f) < 0.001f);
	assert(std::abs(translated.y - 2.0f) < 0.001f);
	assert(std::abs(translated.z - 3.0f) < 0.001f);

	mml::Matrix4f scale = mml::Matrix4f::scale(2.0f, 3.0f, 4.0f);
	assert(std::abs(scale[0][0] - 2.0f) < 0.001f);
	assert(std::abs(scale[1][1] - 3.0f) < 0.001f);
	assert(std::abs(scale[2][2] - 4.0f) < 0.001f);

	mml::Matrix4f rot_y = mml::Matrix4f::rotation_y(mml::Constantsf::half_pi);
	mml::Vector4f v_x(1.0f, 0.0f, 0.0f, 0.0f);
	mml::Vector4f v_rotated = rot_y * v_x;
	assert(std::abs(v_rotated.z - (-1.0f)) < 0.01f);

	mml::Matrix4f frob = mml::Matrix4f::identity();
	float norm = frob.frobenius_norm();
	assert(std::abs(norm - 2.0f) < 0.01f);

	mml::Matrix4f m4_euler = mml::Matrix4f::from_euler(mml::Vector3f(0.0f, mml::Constantsf::half_pi, 0.0f));
	assert(std::abs(m4_euler[0][0]) < 0.1f);

	std::cout << "  [PASS] Matrix4 identity" << std::endl;
	std::cout << "  [PASS] Matrix4 translation" << std::endl;
	std::cout << "  [PASS] Matrix4 scale" << std::endl;
	std::cout << "  [PASS] Matrix4 rotation" << std::endl;
	std::cout << "  [PASS] Matrix4 from_euler" << std::endl;
}

void run_matrix_tests() {
	test_matrix3();
	test_matrix4();
	std::cout << "\nAll matrix tests passed!" << std::endl;
}

} // namespace mml_test
