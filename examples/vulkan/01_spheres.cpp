#include "vulkan_app.hpp"

class PrimitivesDemo : public VulkanApp {
	std::vector<mml::Spheref> spheres;
	std::vector<mml::Vector3f> sphereColors;
	std::vector<float> spherePhases;

	void generateMesh() override {
		vertices.clear();
		indices.clear();

		int segments = 32;
		int rings = 16;

		auto addSphere = [&](mml::Vector3f center, float radius, mml::Vector3f color, float phase) {
			uint16_t baseIndex = (uint16_t)vertices.size();
			int vOffset = 0;

			for (int y = 0; y <= rings; y++) {
				float v = (float)y / rings;
				float phi = v * mml::Constantsf::pi;
				float sinPhi = mml::Mathf::sin(phi);
				float cosPhi = mml::Mathf::cos(phi);

				for (int x = 0; x <= segments; x++) {
					float u = (float)x / segments;
					float theta = u * mml::Constantsf::two_pi;
					float sinTheta = mml::Mathf::sin(theta);
					float cosTheta = mml::Mathf::cos(theta);

					mml::Vector3f pos(center.x + radius * sinPhi * cosTheta,
							center.y + radius * cosPhi,
							center.z + radius * sinPhi * sinTheta);

					vertices.push_back(Vertex{ pos, color });
				}
			}

			for (int y = 0; y < rings; y++) {
				for (int x = 0; x < segments; x++) {
					int curr = baseIndex + y * (segments + 1) + x;
					int next = curr + segments + 1;
					indices.push_back(curr);
					indices.push_back(next);
					indices.push_back(curr + 1);
					indices.push_back(curr + 1);
					indices.push_back(next);
					indices.push_back(next + 1);
				}
			}
		};

		for (int i = 0; i < 8; i++) {
			float angle = (float)i / 8.0f * mml::Constantsf::two_pi;
			float x = mml::Mathf::cos(angle) * 4.0f;
			float z = mml::Mathf::sin(angle) * 4.0f;
			mml::Vector3f color(
					0.5f + 0.5f * mml::Mathf::cos(angle + 0),
					0.5f + 0.5f * mml::Mathf::cos(angle + 2),
					0.5f + 0.5f * mml::Mathf::cos(angle + 4));
			spheres.push_back(mml::Spheref(mml::Vector3f(x, 0, z), 0.5f));
			sphereColors.push_back(color);
			spherePhases.push_back(angle);
			addSphere(mml::Vector3f(x, 0, z), 0.5f, color, angle);
		}
	}

public:
	void initScene() override {
		glfwSetWindowTitle(window, "MML Spheres - Vulkan");
	}

	mml::Matrix4f getModelMatrix(float time) override {
		return mml::Matrix4f::identity();
	}

	void update(float time) override {
		for (size_t i = 0; i < spheres.size(); i++) {
			float offset = spherePhases[i];
			mml::Vector3f center(
					mml::Mathf::cos(offset + time) * 4.0f,
					mml::Mathf::sin(time * 2.0f + offset) * 1.5f,
					mml::Mathf::sin(offset + time) * 4.0f);
			spheres[i].center = center;
		}
	}
};

int main() {
	PrimitivesDemo app;
	app.initWindow("MML Spheres - Vulkan", 1280, 720);
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
