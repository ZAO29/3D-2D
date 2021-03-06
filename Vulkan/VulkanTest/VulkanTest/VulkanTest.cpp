// VulkanTest.cpp : Defines the entry point for the console application.
//

#include "VulkanApp.h"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>

#include <iostream>


int main() {
	
	VulkanApp app;

	app.initWindow();
	app.initVulkan();
	uint32_t extensionCount = 0;
	vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);

	std::cout << extensionCount << " extensions supported" << std::endl;

	glm::mat4 matrix;
	glm::vec4 vec;
	auto test = matrix * vec;

	app.mainLoop();

	app.cleanup();

	return 0;
}

