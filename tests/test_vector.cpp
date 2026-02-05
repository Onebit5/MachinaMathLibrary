#pragma once

#include "../src/mml.hpp"
#include <cassert>
#include <cmath>
#include <iostream>

namespace mml_test {

void test_vector2() {
	mml::Vector2f v(3.0f, 4.0f);
	assert(std::abs(v.x - 3.0f) < 0.001f);
	assert(std::abs(v.y - 4.0f) < 0.001f);

	mml::Vector2f zero = mml::Vector2f::zero();
	assert(std::abs(zero.x) < 0.001f);

	mml::Vector2f sum = v + mml::Vector2f(1.0f, 2.0f);
	assert(std::abs(sum.x - 4.0f) < 0.001f);
	assert(std::abs(sum.y - 6.0f) < 0.001f);

	float dot = v.dot(mml::Vector2f(1.0f, 2.0f));
	assert(std::abs(dot - 11.0f) < 0.001f);

	float len = v.length();
	assert(std::abs(len - 5.0f) < 0.001f);

	std::cout << "  [PASS] Vector2 tests" << std::endl;
}

void test_vector3() {
	mml::Vector3f a(1.0f, 0.0f, 0.0f);
	mml::Vector3f b(0.0f, 1.0f, 0.0f);

	mml::Vector3f cross = a.cross(b);
	assert(std::abs(cross.z - 1.0f) < 0.001f);

	float dot = a.dot(b);
	assert(std::abs(dot) < 0.001f);

	mml::Vector3f c(1.0f, 2.0f, 3.0f);

	std::cout << "  [PASS] Vector3 tests" << std::endl;
}

void test_vector4() {
	mml::Vector4f a(1.0f, 2.0f, 3.0f, 4.0f);
	mml::Vector4f b(2.0f, 3.0f, 4.0f, 5.0f);

	float dot = a.dot(b);
	assert(std::abs(dot - 40.0f) < 0.001f);

	mml::Vector3f xyz = a.xyz();
	assert(std::abs(xyz.x - 1.0f) < 0.001f);

	std::cout << "  [PASS] Vector4 tests" << std::endl;
}

void test_vectors() {
	std::cout << "\n=== Vector Tests ===" << std::endl;
	test_vector2();
	test_vector3();
	test_vector4();
}

} // namespace mml_test
