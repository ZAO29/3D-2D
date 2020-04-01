#pragma once

#include <vector>
#include <optional>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>



#define WIDTH 800
#define HEIGHT 600

struct QueueFamilyIndices
{
	std::optional<uint32_t> graphicsFamily; 
	//std::optional<T>.has_value return false until value is assigned 
	std::optional<uint32_t> presentFamily;


	bool isComplete()
	{
		return (graphicsFamily.has_value() && presentFamily.has_value());
	}
};

struct SwapChainSupportDetails {
	VkSurfaceCapabilitiesKHR capabilities;
	std::vector<VkSurfaceFormatKHR> formats;
	std::vector<VkPresentModeKHR> presentModes;
};

struct SwapChainInfo
{
	VkFormat imageFormat;
	VkExtent2D extent;
};






VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger);
void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator);





class VulkanApp
{
public:
	VulkanApp();
	~VulkanApp();

	void initWindow();
	void initVulkan();
	void mainLoop();
	void cleanup();
	void initLog();

private :

	static const std::vector<const char*> s_validationLayers;

	static const std::vector<const char *> s_deviceExtensions;

	static std::vector<const char*> getRequiredExtensions();

	static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
		VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
		VkDebugUtilsMessageTypeFlagsEXT messageType,
		const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
		void* pUserData);
	static bool checkValidationLayerSupport();

	SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device);
	static VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
	static VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
	static VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);
	static std::vector<char> readFile(const std::string& filename);

	
	void pickPhysicalDevices();
	bool isDeviceSuitable(VkPhysicalDevice device);
	void createLogicalDevice();
	void createSurface();
	QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);
	bool checkDeviceExtensionSupport(VkPhysicalDevice device);
	void createSwapChain();
	void createImageViews();
	void createGraphicsPipeline();
	void createRenderPass();
	VkShaderModule createShaderModule(const std::vector<char>& code);
	void createFrameBuffers();
	void createCommandPool();
	void createCommandBuffers();
	void createSemaphores();

	void drawFrame();
	
	#ifdef NDEBUG
		static const bool s_enableValidationLayers = false;
	#else
		static const bool s_enableValidationLayers = true;
	#endif

	

	GLFWwindow * m_pWindow = nullptr;
	VkInstance m_vkInstance;
	VkPhysicalDevice m_physicalDevice = VK_NULL_HANDLE;
	VkDevice m_device;
	VkDebugUtilsMessengerEXT m_debugMessenger;
	VkQueue m_graphicsQueue;
	VkSurfaceKHR m_surface;
	VkQueue m_presentQueue;
	VkSwapchainKHR m_swapChain;
	std::vector<VkImage> m_swapChainImages;
	SwapChainInfo m_swapChainInfo;
	std::vector<VkImageView> m_swapChainImageViews;
	VkPipelineLayout m_pipelineLayout;
	VkRenderPass m_renderPass;
	VkPipeline m_graphicsPipeline;
	std::vector<VkFramebuffer> m_swapChainFramebuffers;
	VkCommandPool m_commandPool;
	std::vector<VkCommandBuffer> m_commandBuffers;
	VkSemaphore m_imageAvailableSemaphore;
	VkSemaphore m_renderFinishedSemaphore;

};

