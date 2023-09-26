#include "graphics/surface/vulkan/spk_graphical_api_device.hpp"

#include <cstring>
#include <string>
#include <iostream>
#include <set>
#include <unordered_set>
#include "spk_basic_functions.hpp"
#include "graphics/pipeline/vulkan/spk_graphical_api_pipeline_basic_functions.hpp"

namespace spk::GraphicalAPI
{
	static std::wstring to_wstring(const VkDebugUtilsMessageSeverityFlagBitsEXT& p_messageSeverity)
	{
		switch (p_messageSeverity)
		{
		case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT:
			return L"VERBOSE";
		case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:
			return L"INFO";
		case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
			return L"WARNING";
		case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
			return L"ERROR";
		default:
			return L"UNKNOWN";
		}
	}

	static std::wstring to_wstring(const VkDebugUtilsMessageTypeFlagsEXT& p_messageType)
	{
		switch (p_messageType)
		{
		case VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT:
			return L"GENERAL";
		case VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT:
			return L"VALIDATION";
		case VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT:
			return L"PERFORMANCE";
		default:
			return L"UNKNOWN";
		}
	}

	static VKAPI_ATTR VkBool32 VKAPI_CALL validationCallback(
		VkDebugUtilsMessageSeverityFlagBitsEXT p_messageSeverity,
		VkDebugUtilsMessageTypeFlagsEXT p_messageType,
		const VkDebugUtilsMessengerCallbackDataEXT* p_pCallbackData,
		void* p_pUserData)
	{
		std::wstring message;
		size_t length = strlen(p_pCallbackData->pMessage);

		message.reserve(length);
		for (size_t i = 0; i < length; ++i)
		{
			message.push_back(p_pCallbackData->pMessage[i]);
			if (message.back() == ':')
				message += L"\n\t";
		}

		spk::cerr << L"validation layer {" <<
			to_wstring(p_messageSeverity) <<
			L"} type {" <<
			to_wstring(p_messageType) <<
			L"} : \n\tmessage: " <<
			message << std::endl;

		return VK_FALSE;
	}


	static void DestroyDebugUtilsMessengerEXT(
		vk::Instance p_instance,
		VkDebugUtilsMessengerEXT p_debugMessenger,
		const VkAllocationCallbacks* p_pAllocator)
	{
		auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)p_instance.getProcAddr(
			"vkDestroyDebugUtilsMessengerEXT");
		if (func != nullptr)
			func(p_instance, p_debugMessenger, p_pAllocator);
	}

	// class member functions
	Device::Device(const Frame* p_frame)
	{
		_createInstance();
		_setupDebugMessenger();
		_createSurface(p_frame);
		_pickPhysicalDevice();
		_createLogicalDevice();
		_createCommandPool();
		SpirvHelper::init();
	}

	Device::~Device()
	{
		SpirvHelper::finalize();
		_device.destroyCommandPool(_commandPool);
		_device.destroy();

		if (enableValidationLayers == true)
			_instance.destroyDebugUtilsMessengerEXT(_debugMessenger, nullptr, _dispatcher.dynamicDispatcher());

		_instance.destroySurfaceKHR(_surface, nullptr);
		_instance.destroy();
		//? spk::vulkan::Dispatcher::release();
	}

	void Device::_checkExtensionsSupport(const std::vector<const char*>& p_extensions)
	{
		std::vector<vk::ExtensionProperties> availableExtensions = vk::enumerateInstanceExtensionProperties();

		for (const auto& extension : p_extensions)
		{
			bool extensionFound = false;

			for (const auto& extensionProperties : availableExtensions)
				if (strcmp(extension, extensionProperties.extensionName) == 0)
				{
					extensionFound = true;
					break;
				}
			if (extensionFound == false)
				spk::throwException(L"Extension not supported: " + spk::to_wstring(extension));
		}
	}

	void Device::_addValidationLayers(vk::InstanceCreateInfo& p_createInfo,
		const std::vector<const char*>& p_layers,
		vk::DebugUtilsMessengerCreateInfoEXT& p_debugCreateInfo)
	{
		p_createInfo.setEnabledLayerCount(static_cast<uint32_t>(p_layers.size()));
		p_createInfo.setPpEnabledLayerNames(p_layers.data());

		_populateDebugMessengerCreateInfo(p_debugCreateInfo);

		p_createInfo.setPNext(&p_debugCreateInfo);
	}

	void Device::_createInstance()
	{
		if (enableValidationLayers && _checkValidationLayerSupport() == false)
			spk::throwException(L"validation layers requested, but not available");

		vk::ApplicationInfo appInfo(
			"Sparkle",
			VK_MAKE_API_VERSION(0, 1, 0, 0),
			"Sparkle Engine",
			VK_MAKE_API_VERSION(0, 1, 0, 0),
			VK_API_VERSION_1_0
		);

		std::vector<const char*> extensions({
			VK_KHR_SURFACE_EXTENSION_NAME
			});
#ifdef __linux__
		extensions.push_back(VK_KHR_XCB_SURFACE_EXTENSION_NAME);
#elif _WIN32
		extensions.push_back(VK_KHR_WIN32_SURFACE_EXTENSION_NAME);
#endif

		if (enableValidationLayers == true)
			extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);

		_checkExtensionsSupport(extensions);

		vk::InstanceCreateInfo createInfo(
			vk::InstanceCreateFlags(),
			&appInfo,
			0,
			nullptr,
			static_cast<uint32_t>(extensions.size()),
			extensions.data()
		);

		vk::DebugUtilsMessengerCreateInfoEXT debugCreateInfo;
		if (enableValidationLayers == true)
			_addValidationLayers(createInfo, _validationLayers, debugCreateInfo);

		if (vk::createInstance(&createInfo, nullptr, &_instance) != vk::Result::eSuccess)
			spk::throwException(L"failed to create instance");

		_dispatcher.init(_instance);
	}

	void Device::_pickPhysicalDevice()
	{
		std::vector<vk::PhysicalDevice> devices(_instance.enumeratePhysicalDevices());

		if (devices.size() == 0)
			spk::throwException(L"failed to find GPUs with Vulkan support");
		spk::cout << "Device count: " << devices.size() << std::endl;

		for (const auto& device : devices)
			if (_isDeviceSuitable(device))
			{
				_physicalDevice = device;
				break;
			}

		if (static_cast<bool>(_physicalDevice) == false)
			spk::throwException(L"failed to find a suitable GPU");

		_physicalDevice.getProperties(&properties);
		std::cout << "physical device: " << properties.deviceName << std::endl;
	}

	void Device::_createLogicalDevice()
	{
		QueueFamilyIndices indices = _findQueueFamilies(_physicalDevice);

		std::vector<vk::DeviceQueueCreateInfo> queueCreateInfos;
		std::set<uint32_t> uniqueQueueFamilies = { indices.graphicsFamily, indices.presentFamily };

		float queuePriority = 1.0f;
		for (uint32_t queueFamily : uniqueQueueFamilies) {
			vk::DeviceQueueCreateInfo queueCreateInfo(
				vk::DeviceQueueCreateFlags(),
				queueFamily,
				1,
				&queuePriority
			);
			queueCreateInfos.push_back(queueCreateInfo);
		}

		vk::PhysicalDeviceFeatures deviceFeatures;
		deviceFeatures.setSamplerAnisotropy(VK_TRUE);

		vk::DeviceCreateInfo createInfo(
			vk::DeviceCreateFlags(),
			static_cast<uint32_t>(queueCreateInfos.size()),
			queueCreateInfos.data(),
			0,
			nullptr,
			static_cast<uint32_t>(_deviceExtensions.size()),
			_deviceExtensions.data(),
			&deviceFeatures
		);

		//? might not be necessary anymore because device specific validation layers are deprecated
		if (enableValidationLayers == true)
		{
			createInfo.setEnabledLayerCount(static_cast<uint32_t>(_validationLayers.size()));
			createInfo.setPpEnabledLayerNames(_validationLayers.data());
		}

		if (_physicalDevice.createDevice(&createInfo, nullptr, &_device) != vk::Result::eSuccess)
			spk::throwException(L"failed to create logical device");

		_device.getQueue(indices.graphicsFamily, 0, &_graphicsQueue);
		_device.getQueue(indices.presentFamily, 0, &_presentQueue);
	}

	void Device::_createCommandPool()
	{
		QueueFamilyIndices queueFamilyIndices = findPhysicalQueueFamilies();

		vk::CommandPoolCreateInfo poolInfo(
			vk::CommandPoolCreateFlags(),
			queueFamilyIndices.graphicsFamily
		);
		poolInfo.setFlags(vk::CommandPoolCreateFlagBits::eTransient |
			vk::CommandPoolCreateFlagBits::eResetCommandBuffer);

		if (_device.createCommandPool(&poolInfo, nullptr, &_commandPool) != vk::Result::eSuccess)
			spk::throwException(L"failed to create command pool");
	}

	void Device::_createSurface(const Frame* p_frame)
	{
#ifdef __linux__
		vk::XcbSurfaceCreateInfoKHR createInfo(
			vk::XcbSurfaceCreateFlagsKHR(),
			p_frame->connection(),
			p_frame->window()
		);

		if (_instance.createXcbSurfaceKHR(&createInfo, nullptr, &_surface) != vk::Result::eSuccess)
			spk::throwException(L"Failed to create Xcb surface");
#elif _WIN32
		vk::Win32SurfaceCreateInfoKHR createInfo(
			vk::Win32SurfaceCreateFlagsKHR(),
			p_frame->hInstance(),
			p_frame->windowFrame()
		);

		if (_instance.createWin32SurfaceKHR(&createInfo, nullptr, &_surface) != vk::Result::eSuccess)
			spk::throwException(L"Failed to create Win32 surface");
#endif
	}

	bool Device::_isDeviceSuitable(vk::PhysicalDevice p_device)
	{
		QueueFamilyIndices indices = _findQueueFamilies(p_device);

		bool extensionsSupported = _checkDeviceExtensionSupport(p_device);

		bool swapChainAdequate = false;
		if (extensionsSupported == true)
		{
			SwapChainSupportDetails swapChainSupport = _querySwapChainSupport(p_device);
			swapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
		}

		vk::PhysicalDeviceFeatures supportedFeatures(p_device.getFeatures());

		return (indices.isComplete() &&
			extensionsSupported && swapChainAdequate &&
			supportedFeatures.samplerAnisotropy
			);
	}

	void Device::_populateDebugMessengerCreateInfo(
		vk::DebugUtilsMessengerCreateInfoEXT& p_createInfo)
	{
		p_createInfo = vk::DebugUtilsMessengerCreateInfoEXT(
			vk::DebugUtilsMessengerCreateFlagsEXT(),
			vk::DebugUtilsMessageSeverityFlagBitsEXT::eVerbose |
			vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning |
			vk::DebugUtilsMessageSeverityFlagBitsEXT::eError,
			vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral |
			vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation |
			vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance,
			validationCallback);
	}

	void Device::_setupDebugMessenger()
	{
		if (enableValidationLayers == false)
			return;
		vk::DebugUtilsMessengerCreateInfoEXT createInfo;
		_populateDebugMessengerCreateInfo(createInfo);

		_debugMessenger = _instance.createDebugUtilsMessengerEXT(createInfo, nullptr, _dispatcher.dynamicDispatcher());
	}

	bool Device::_checkValidationLayerSupport()
	{
		std::vector<vk::LayerProperties> availableLayers(vk::enumerateInstanceLayerProperties());

		for (const char* layerName : _validationLayers) {
			bool layerFound = false;

			for (const auto& layerProperties : availableLayers) {
				if (strcmp(layerName, layerProperties.layerName) == 0) {
					layerFound = true;
					break;
				}
			}

			if (!layerFound) {
				return false;
			}
		}

		return true;
	}

	bool Device::_checkDeviceExtensionSupport(vk::PhysicalDevice p_device)
	{
		std::vector<vk::ExtensionProperties> availableExtensions = p_device.enumerateDeviceExtensionProperties();

		std::set<std::string> requiredExtensions(_deviceExtensions.begin(), _deviceExtensions.end());

		for (const auto& extension : availableExtensions)
		{
			requiredExtensions.erase(extension.extensionName);
		}

		return (requiredExtensions.empty());
	}

	QueueFamilyIndices Device::_findQueueFamilies(vk::PhysicalDevice p_physicalDevice)
	{
		QueueFamilyIndices indices;
		std::vector<vk::QueueFamilyProperties> queueFamilies(p_physicalDevice.getQueueFamilyProperties());

		int i = 0;
		for (const auto& queueFamily : queueFamilies) {
			if (queueFamily.queueCount > 0 && queueFamily.queueFlags & vk::QueueFlagBits::eGraphics)
			{
				indices.graphicsFamily = i;
				indices.graphicsFamilyHasValue = true;
			}
			VkBool32 presentSupport = false;
			if (p_physicalDevice.getSurfaceSupportKHR(i, _surface, &presentSupport) != vk::Result::eSuccess)
				spk::throwException(L"failed to get surface support");
			if (queueFamily.queueCount > 0 && presentSupport == static_cast<VkBool32>(true))
			{
				indices.presentFamily = i;
				indices.presentFamilyHasValue = true;
			}
			if (indices.isComplete())
			{
				break;
			}

			++i;
		}

		return indices;
	}

	SwapChainSupportDetails Device::_querySwapChainSupport(vk::PhysicalDevice p_physicalDevice)
	{
		SwapChainSupportDetails details;

		details.capabilities = p_physicalDevice.getSurfaceCapabilitiesKHR(_surface);
		details.formats = p_physicalDevice.getSurfaceFormatsKHR(_surface);
		details.presentModes = p_physicalDevice.getSurfacePresentModesKHR(_surface);

		return details;
	}

	vk::Format Device::findSupportedFormat(const std::vector<vk::Format>& p_candidates,
		vk::ImageTiling p_tiling, vk::FormatFeatureFlags p_features)
	{
		for (vk::Format format : p_candidates) {
			vk::FormatProperties props(_physicalDevice.getFormatProperties(format));

			if (p_tiling == vk::ImageTiling::eLinear &&
				(props.linearTilingFeatures & p_features) == p_features)
				return format;
			else if (p_tiling == vk::ImageTiling::eOptimal &&
				(props.optimalTilingFeatures & p_features) == p_features)
				return format;
		}
		spk::throwException(L"failed to find supported format");
		return (vk::Format::eUndefined);
	}

	uint32_t Device::findMemoryType(uint32_t p_typeFilter, vk::MemoryPropertyFlags p_properties)
	{
		vk::PhysicalDeviceMemoryProperties memProperties(_physicalDevice.getMemoryProperties());
		for (uint32_t i = 0; i < memProperties.memoryTypeCount; ++i)
		{
			if ((p_typeFilter & (1 << i)) &&
				(memProperties.memoryTypes[i].propertyFlags & p_properties) == p_properties) {
				return i;
			}
		}

		spk::throwException(L"failed to find suitable memory type");
		return (0);
	}

	void Device::createBuffer(
		vk::DeviceSize p_size,
		vk::BufferUsageFlags p_usage,
		vk::MemoryPropertyFlags p_properties,
		vk::Buffer& p_buffer,
		vk::DeviceMemory& p_bufferMemory)
	{
		vk::BufferCreateInfo bufferInfo(
			vk::BufferCreateFlags(),
			p_size,
			p_usage,
			vk::SharingMode::eExclusive
		);

		if (_device.createBuffer(&bufferInfo, nullptr, &p_buffer) != vk::Result::eSuccess)
			spk::throwException(L"failed to create vertex buffer");

		vk::MemoryRequirements memRequirements;
		_device.getBufferMemoryRequirements(p_buffer, &memRequirements);

		vk::MemoryAllocateInfo allocInfo(
			memRequirements.size,
			findMemoryType(memRequirements.memoryTypeBits, p_properties)
		);

		if (_device.allocateMemory(&allocInfo, nullptr, &p_bufferMemory) != vk::Result::eSuccess) {
			spk::throwException(L"failed to allocate vertex buffer memory");
		}

		_device.bindBufferMemory(p_buffer, p_bufferMemory, 0);
	}

	vk::CommandBuffer Device::beginSingleTimeCommands()
	{
		vk::CommandBufferAllocateInfo allocInfo(
			_commandPool,
			vk::CommandBufferLevel::ePrimary,
			1
		);

		vk::CommandBuffer commandBuffer;
		if (_device.allocateCommandBuffers(&allocInfo, &commandBuffer) != vk::Result::eSuccess)
			spk::throwException(L"failed to allocate command buffers");

		vk::CommandBufferBeginInfo beginInfo(
			vk::CommandBufferUsageFlagBits::eOneTimeSubmit
		);

		if (commandBuffer.begin(&beginInfo) != vk::Result::eSuccess)
			spk::throwException(L"failed to begin recording command buffer");
		return commandBuffer;
	}

	void Device::endSingleTimeCommands(vk::CommandBuffer p_commandBuffer)
	{
		p_commandBuffer.end();

		vk::SubmitInfo submitInfo(
			0,
			nullptr,
			nullptr,
			1,
			&p_commandBuffer
		);

		if (_graphicsQueue.submit(1, &submitInfo, VK_NULL_HANDLE) != vk::Result::eSuccess)
			spk::throwException(L"failed to submit copy command buffer");
		_graphicsQueue.waitIdle();

		_device.freeCommandBuffers(_commandPool, 1, &p_commandBuffer);
	}

	void Device::copyBuffer(vk::Buffer p_srcBuffer, vk::Buffer p_dstBuffer, vk::DeviceSize p_size)
	{
		vk::CommandBuffer commandBuffer = beginSingleTimeCommands();

		vk::BufferCopy copyRegion(
			0,
			0,
			p_size
		);
		commandBuffer.copyBuffer(p_srcBuffer, p_dstBuffer, 1, &copyRegion);

		endSingleTimeCommands(commandBuffer);
	}

	void Device::copyBufferToImage(vk::Buffer p_buffer, vk::Image p_image,
		uint32_t p_width, uint32_t p_height, uint32_t p_layerCount)
	{
		vk::CommandBuffer commandBuffer = beginSingleTimeCommands();

		vk::BufferImageCopy region(
			0,
			0,
			0,
			vk::ImageSubresourceLayers(
				vk::ImageAspectFlagBits::eColor,
				0,
				0,
				p_layerCount
			),
			vk::Offset3D(0, 0, 0),
			vk::Extent3D(p_width, p_height, 1)
		);

		commandBuffer.copyBufferToImage(
			p_buffer,
			p_image,
			vk::ImageLayout::eTransferDstOptimal,
			1,
			&region);
		endSingleTimeCommands(commandBuffer);
	}

	void Device::createImageWithInfo(
		const vk::ImageCreateInfo& p_imageInfo,
		vk::MemoryPropertyFlags p_properties,
		vk::Image& p_image,
		vk::DeviceMemory& p_imageMemory)
	{
		if (_device.createImage(&p_imageInfo, nullptr, &p_image) != vk::Result::eSuccess)
			spk::throwException(L"failed to create image");

		vk::MemoryRequirements memRequirements(_device.getImageMemoryRequirements(p_image));

		vk::MemoryAllocateInfo allocInfo(
			memRequirements.size,
			findMemoryType(memRequirements.memoryTypeBits, p_properties)
		);

		if (_device.allocateMemory(&allocInfo, nullptr, &p_imageMemory) != vk::Result::eSuccess)
			spk::throwException(L"failed to allocate image memory");
		_device.bindImageMemory(p_image, p_imageMemory, 0);
	}
}