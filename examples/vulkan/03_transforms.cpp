/**************************************************************************/
/*  03_transforms.cpp                                                     */
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

class TransformsDemo : public VulkanApp {
	mml::TransformNodef root;
	mml::TransformNodef child;
	mml::TransformNodef grandchild;
	mml::TransformNodef greatGrandchild;

	void generateMesh() override {
		vertices.clear();
		indices.clear();

		auto addCube = [&](mml::Vector3f pos, mml::Vector3f scale, mml::Vector3f color) {
			uint16_t base = (uint16_t)vertices.size();
			mml::Vector3f corners[8] = {
				mml::Vector3f(-0.5f, -0.5f, -0.5f).hadamard(scale) + pos,
				mml::Vector3f(0.5f, -0.5f, -0.5f).hadamard(scale) + pos,
				mml::Vector3f(0.5f, 0.5f, -0.5f).hadamard(scale) + pos,
				mml::Vector3f(-0.5f, 0.5f, -0.5f).hadamard(scale) + pos,
				mml::Vector3f(-0.5f, -0.5f, 0.5f).hadamard(scale) + pos,
				mml::Vector3f(0.5f, -0.5f, 0.5f).hadamard(scale) + pos,
				mml::Vector3f(0.5f, 0.5f, 0.5f).hadamard(scale) + pos,
				mml::Vector3f(-0.5f, 0.5f, 0.5f).hadamard(scale) + pos
			};

			for (int i = 0; i < 8; i++) {
				vertices.push_back(Vertex{ corners[i], color });
			}

			uint16_t cube[] = {
				0, 1, 2, 2, 3, 0,
				1, 5, 6, 6, 2, 1,
				7, 6, 5, 5, 4, 7,
				4, 0, 3, 3, 7, 4,
				4, 5, 1, 1, 0, 4,
				3, 2, 6, 6, 7, 3
			};
			for (uint16_t idx : cube) {
				indices.push_back(base + idx);
			}
		};

		addCube(mml::Vector3f::zero(), mml::Vector3f(1, 1, 1), mml::Vector3f(1, 0, 0));
		addCube(mml::Vector3f(2, 0, 0), mml::Vector3f(0.5f, 0.5f, 0.5f), mml::Vector3f(0, 1, 0));
		addCube(mml::Vector3f(4, 0, 0), mml::Vector3f(0.3f, 0.3f, 0.3f), mml::Vector3f(0, 0, 1));
		addCube(mml::Vector3f(5.5f, 0, 0), mml::Vector3f(0.2f, 0.2f, 0.2f), mml::Vector3f(1, 1, 0));
	}

public:
	void initScene() override {
		root.set_position(0, 0, 0);
		root.set_rotation(mml::Quaternionf::identity());

		child.set_parent(&root);
		child.set_position(2, 0, 0);

		grandchild.set_parent(&child);
		grandchild.set_position(2, 0, 0);

		greatGrandchild.set_parent(&grandchild);
		greatGrandchild.set_position(1.5f, 0, 0);

		glfwSetWindowTitle(window, "MML Transform Hierarchy - Vulkan");
	}

	void update(float time) override {
		root.set_rotation(mml::Quaternionf::from_axis_angle(mml::Vector3f::unit_y(), time * 0.5f));
		child.set_rotation(mml::Quaternionf::from_axis_angle(mml::Vector3f::unit_x(), time * 1.5f));
		grandchild.set_rotation(mml::Quaternionf::from_axis_angle(mml::Vector3f::unit_z(), time * 2.0f));
		greatGrandchild.set_rotation(mml::Quaternionf::from_axis_angle((mml::Vector3f::unit_x() + mml::Vector3f::unit_y()).normalized(), time * 3.0f));
	}

	mml::Matrix4f getModelMatrix(float time) override {
		return greatGrandchild.get_world_matrix();
	}
};

int main() {
	TransformsDemo app;
	app.initWindow("MML Transform Hierarchy - Vulkan", 1280, 720);
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