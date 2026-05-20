/**************************************************************************/
/*  02_quaternion_slerp.cpp                                               */
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

#include "vulkan_app.hpp"

class QuaternionSlerpDemo : public VulkanApp {
	std::vector<mml::Quaternionf> keyRotations;
	std::vector<mml::Vector3f> objectPositions;
	mml::BezierCubicf bezier;

	void generateMesh() override {
		vertices.clear();
		indices.clear();

		vertices.push_back(Vertex{ mml::Vector3f(-0.5f, -0.5f, -0.5f), mml::Vector3f(0, 0, 0) });
		vertices.push_back(Vertex{ mml::Vector3f(0.5f, -0.5f, -0.5f), mml::Vector3f(1, 0, 0) });
		vertices.push_back(Vertex{ mml::Vector3f(0.5f, 0.5f, -0.5f), mml::Vector3f(1, 1, 0) });
		vertices.push_back(Vertex{ mml::Vector3f(-0.5f, 0.5f, -0.5f), mml::Vector3f(0, 1, 0) });
		vertices.push_back(Vertex{ mml::Vector3f(-0.5f, -0.5f, 0.5f), mml::Vector3f(0, 0, 1) });
		vertices.push_back(Vertex{ mml::Vector3f(0.5f, -0.5f, 0.5f), mml::Vector3f(1, 0, 1) });
		vertices.push_back(Vertex{ mml::Vector3f(0.5f, 0.5f, 0.5f), mml::Vector3f(1, 1, 1) });
		vertices.push_back(Vertex{ mml::Vector3f(-0.5f, 0.5f, 0.5f), mml::Vector3f(0.5f, 0.5f, 0.5f) });

		uint16_t cubeIndices[] = {
			0, 1, 2, 2, 3, 0,
			1, 5, 6, 6, 2, 1,
			7, 6, 5, 5, 4, 7,
			4, 0, 3, 3, 7, 4,
			4, 5, 1, 1, 0, 4,
			3, 2, 6, 6, 7, 3
		};
		indices.insert(indices.end(), cubeIndices, cubeIndices + 36);
	}

public:
	QuaternionSlerpDemo() : bezier(
									mml::Vector3f(-3, 0, 0),
									mml::Vector3f(-1, 2, 0),
									mml::Vector3f(1, -2, 0),
									mml::Vector3f(3, 0, 0)) {}

	void initScene() override {
		keyRotations = {
			mml::Quaternionf::identity(),
			mml::Quaternionf::from_axis_angle(mml::Vector3f::unit_x(), mml::Constantsf::half_pi),
			mml::Quaternionf::from_axis_angle(mml::Vector3f::unit_y(), mml::Constantsf::half_pi),
			mml::Quaternionf::from_axis_angle(mml::Vector3f::unit_z(), mml::Constantsf::half_pi),
			mml::Quaternionf::from_axis_angle((mml::Vector3f::unit_x() + mml::Vector3f::unit_y()).normalized(), mml::Constantsf::half_pi)
		};
		glfwSetWindowTitle(window, "Quaternion Slerp with Bezier Path - Vulkan");
	}

	mml::Matrix4f getModelMatrix(float time) override {
		float t = mml::Mathf::fmod(time * 0.3f, 1.0f);
		int index = (int)(time * 0.3f) % (keyRotations.size() - 1);
		float localT = mml::Mathf::fmod(time * 0.3f, 1.0f);

		mml::Quaternionf q1 = keyRotations[index];
		mml::Quaternionf q2 = keyRotations[index + 1];
		mml::Quaternionf slerped = mml::Quaternionf::slerp(q1, q2, localT);

		mml::Vector3f bezierPos = bezier.evaluate(t);

		mml::Matrix4f model = mml::Matrix4f::translation(bezierPos);
		model = model * slerped.to_matrix4();
		model = model * mml::Matrix4f::scale(0.8f, 0.8f, 0.8f);

		return model;
	}
};

int main() {
	QuaternionSlerpDemo app;
	app.initWindow("Quaternion Slerp with Bezier Path - Vulkan", 1280, 720);
	try {
		app.initVulkan();
		app.run();
	} catch (const std::exception &e) {
		std::cerr << "Error: " << e.what() << std::endl;
		return 1;
	}
	app.cleanup();
	return 0;
}