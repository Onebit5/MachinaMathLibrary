/**************************************************************************/
/*  04_free_camera.cpp                                                    */
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

class FreeFlyCamera : public VulkanApp {
	mml::Vector3f cameraPos = mml::Vector3f(0, 3, 10);
	mml::Vector3f cameraForward = mml::Vector3f(0, -0.2f, -1).normalized();
	mml::Vector3f cameraUp = mml::Vector3f::unit_y();
	float yaw = 90.0f;
	float pitch = -11.0f;
	float sensitivity = 0.08f;
	float speed = 15.0f;
	bool mouseCaptured = false;
	double lastMouseX = 0;
	double lastMouseY = 0;
	bool firstMouseMove = true;
	bool keys[6] = { false };

	static float deg2rad(float deg) {
		return deg * mml::Constantsf::pi / 180.0f;
	}

	void generateMesh() override {
		vertices.clear();
		indices.clear();

		float s = 1.5f;
		mml::Vector3f corners[8] = {
			mml::Vector3f(-s, -s, -s), mml::Vector3f(s, -s, -s),
			mml::Vector3f(s, s, -s), mml::Vector3f(-s, s, -s),
			mml::Vector3f(-s, -s, s), mml::Vector3f(s, -s, s),
			mml::Vector3f(s, s, s), mml::Vector3f(-s, s, s)
		};
		mml::Vector3f colors[6] = {
			mml::Vector3f(0.9f, 0.2f, 0.2f), mml::Vector3f(0.2f, 0.9f, 0.2f),
			mml::Vector3f(0.2f, 0.2f, 0.9f), mml::Vector3f(0.9f, 0.9f, 0.2f),
			mml::Vector3f(0.9f, 0.2f, 0.9f), mml::Vector3f(0.2f, 0.9f, 0.9f)
		};
		uint16_t faces[6][4] = {
			{ 0, 1, 2, 3 }, { 1, 5, 6, 2 }, { 5, 4, 7, 6 },
			{ 4, 0, 3, 7 }, { 3, 2, 6, 7 }, { 4, 5, 1, 0 }
		};

		for (int f = 0; f < 6; f++) {
			uint16_t base = (uint16_t)vertices.size();
			for (int i = 0; i < 4; i++) {
				vertices.push_back(Vertex{ corners[faces[f][i]], colors[f] });
			}
			indices.push_back(base);
			indices.push_back(base + 1);
			indices.push_back(base + 2);
			indices.push_back(base);
			indices.push_back(base + 2);
			indices.push_back(base + 3);
		}

		float gridSize = 50.0f;
		float gridStep = 5.0f;
		for (float x = -gridSize; x <= gridSize; x += gridStep) {
			uint16_t base = (uint16_t)vertices.size();
			vertices.push_back(Vertex{ mml::Vector3f(x, 0, -gridSize), mml::Vector3f(0.35f, 0.35f, 0.35f) });
			vertices.push_back(Vertex{ mml::Vector3f(x, 0, gridSize), mml::Vector3f(0.35f, 0.35f, 0.35f) });
			indices.push_back(base);
			indices.push_back(base + 1);
		}
		for (float z = -gridSize; z <= gridSize; z += gridStep) {
			uint16_t base = (uint16_t)vertices.size();
			vertices.push_back(Vertex{ mml::Vector3f(-gridSize, 0, z), mml::Vector3f(0.35f, 0.35f, 0.35f) });
			vertices.push_back(Vertex{ mml::Vector3f(gridSize, 0, z), mml::Vector3f(0.35f, 0.35f, 0.35f) });
			indices.push_back(base);
			indices.push_back(base + 1);
		}
	}

	static void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods) {
		FreeFlyCamera *app = (FreeFlyCamera *)glfwGetWindowUserPointer(window);
		if (key == GLFW_KEY_W) {
			app->keys[0] = (action == GLFW_PRESS || action == GLFW_REPEAT);
		}
		if (key == GLFW_KEY_S) {
			app->keys[1] = (action == GLFW_PRESS || action == GLFW_REPEAT);
		}
		if (key == GLFW_KEY_A) {
			app->keys[2] = (action == GLFW_PRESS || action == GLFW_REPEAT);
		}
		if (key == GLFW_KEY_D) {
			app->keys[3] = (action == GLFW_PRESS || action == GLFW_REPEAT);
		}
		if (key == GLFW_KEY_SPACE) {
			app->keys[4] = (action == GLFW_PRESS || action == GLFW_REPEAT);
		}
		if (key == GLFW_KEY_LEFT_SHIFT) {
			app->keys[5] = (action == GLFW_PRESS || action == GLFW_REPEAT);
		}
	}

	static void mouseButtonCallback(GLFWwindow *window, int button, int action, int mods) {
		FreeFlyCamera *app = (FreeFlyCamera *)glfwGetWindowUserPointer(window);
		if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
			app->mouseCaptured = !app->mouseCaptured;
			if (app->mouseCaptured) {
				glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
				app->firstMouseMove = true;
			} else {
				glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			}
		}
	}

	static void cursorPosCallback(GLFWwindow *window, double xpos, double ypos) {
		FreeFlyCamera *app = (FreeFlyCamera *)glfwGetWindowUserPointer(window);
		if (app->mouseCaptured) {
			if (app->firstMouseMove) {
				app->lastMouseX = xpos;
				app->lastMouseY = ypos;
				app->firstMouseMove = false;
				return;
			}

			float dx = (float)(xpos - app->lastMouseX);
			float dy = (float)(ypos - app->lastMouseY);
			app->lastMouseX = xpos;
			app->lastMouseY = ypos;

			app->yaw += dx * app->sensitivity;
			app->pitch -= dy * app->sensitivity;
			if (app->pitch > 89.0f) {
				app->pitch = 89.0f;
			}
			if (app->pitch < -89.0f) {
				app->pitch = -89.0f;
			}

			app->updateCameraDirection();
		}
	}

	void updateCameraDirection() {
		mml::Vector3f front;
		front.x = cos(deg2rad(yaw)) * cos(deg2rad(pitch));
		front.y = sin(deg2rad(pitch));
		front.z = sin(deg2rad(yaw)) * cos(deg2rad(pitch));
		cameraForward = front.normalized();
	}

public:
	void initScene() override {
		updateCameraDirection();
		glfwSetWindowTitle(window, "Free Fly Camera | WASD: Move | Space/Shift: Up/Down | Mouse: Look | Left Click: Toggle Cursor");
		glfwSetKeyCallback(window, keyCallback);
		glfwSetMouseButtonCallback(window, mouseButtonCallback);
		glfwSetCursorPosCallback(window, cursorPosCallback);
	}

	void update(float time) override {
		float dt = 0.016f;
		mml::Vector3f right = cameraForward.cross(cameraUp).normalized();

		mml::Vector3f moveDir = mml::Vector3f::zero();
		if (keys[0]) {
			moveDir += cameraForward;
		}
		if (keys[1]) {
			moveDir -= cameraForward;
		}
		if (keys[2]) {
			moveDir -= right;
		}
		if (keys[3]) {
			moveDir += right;
		}
		if (keys[4]) {
			moveDir += mml::Vector3f::unit_y();
		}
		if (keys[5]) {
			moveDir -= mml::Vector3f::unit_y();
		}

		if (moveDir.length_squared() > 0.001f) {
			moveDir = moveDir.normalized();
			cameraPos += moveDir * speed * dt;
		}

		UniformBufferObject ubo{};
		ubo.model = mml::Matrix4f::identity();
		mml::Vector3f target = cameraPos + cameraForward;
		ubo.view = mml::Matrix4f::look_at(cameraPos, target, cameraUp);
		float aspect = (float)swapChainExtent.width / (float)swapChainExtent.height;
		ubo.projection = mml::Matrix4f::perspective(mml::Constantsf::half_pi / aspect, aspect, 0.1f, 1000.0f);
		ubo.projection[1][1] *= -1;

		for (size_t i = 0; i < uniformBuffers.size(); i++) {
			void *data;
			vkMapMemory(device, uniformBuffersMemory[i], 0, sizeof(ubo), 0, &data);
			memcpy(data, &ubo, sizeof(ubo));
			vkUnmapMemory(device, uniformBuffersMemory[i]);
		}
	}

	void updateUniformBuffer(uint32_t imageIndex, float time) override {
	}

	mml::Matrix4f getModelMatrix(float time) override {
		return mml::Matrix4f::identity();
	}
};

int main() {
	FreeFlyCamera app;
	app.initWindow("Free Fly Camera Demo", 1280, 720);
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