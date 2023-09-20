#pragma once

#include "spk_system_define.hpp"
#include <vulkan/vulkan.hpp>
#include <string>
#include <vector>

#include "design_pattern/spk_singleton.hpp"
#include "graphics/spk_graphical_api_frame.hpp"

namespace spk::GraphicalAPI
{
	struct SwapChainSupportDetails {
		vk::SurfaceCapabilitiesKHR capabilities;
		std::vector<vk::SurfaceFormatKHR> formats;
		std::vector<vk::PresentModeKHR> presentModes;
	};

	struct QueueFamilyIndices {
		uint32_t graphicsFamily;
		uint32_t presentFamily;
		bool graphicsFamilyHasValue = false;
		bool presentFamilyHasValue = false;
		bool isComplete() { return graphicsFamilyHasValue && presentFamilyHasValue; }
	};

	class Device
	{
	public:
		class Pipeline
		{
		public:
			struct ConfigInfo
			{
				ConfigInfo() = default;
				ConfigInfo(const ConfigInfo&) = delete;
				ConfigInfo& operator=(const ConfigInfo&) = delete;

				std::vector<vk::VertexInputBindingDescription> bindingDescriptions;
				std::vector<vk::VertexInputAttributeDescription> attributeDescriptions;
				vk::PipelineViewportStateCreateInfo viewportInfo;
				vk::PipelineInputAssemblyStateCreateInfo inputAssemblyInfo;
				vk::PipelineRasterizationStateCreateInfo rasterizationInfo;
				vk::PipelineMultisampleStateCreateInfo multisampleInfo;
				vk::PipelineColorBlendAttachmentState colorBlendAttachment;
				vk::PipelineColorBlendStateCreateInfo colorBlendInfo;
				vk::PipelineDepthStencilStateCreateInfo depthStencilInfo;
				std::vector<vk::DynamicState> dynamicStateEnables;
				vk::PipelineDynamicStateCreateInfo dynamicStateInfo;
				vk::PipelineLayout pipelineLayout;
				vk::RenderPass renderPass;
				uint32_t subpass{ 0 };
			};

			Pipeline(Device& p_device,
				const std::filesystem::path& p_vertexShaderPath,
				const std::filesystem::path& p_fragmentShaderPath,
				const ConfigInfo& p_configInfo);

			~Pipeline();

			Pipeline(const Pipeline&) = delete;
			Pipeline& operator=(const Pipeline&) = delete;

			static void defaultPipelineConfigInfo(ConfigInfo& p_configInfo);

			void bind(vk::CommandBuffer p_commandBuffer);

		private:

			static std::vector<char> _readFile(const std::filesystem::path& p_filePath);

			void _createGraphicsPipeline(const std::filesystem::path& p_vertexShaderPath,
				const std::filesystem::path& p_fragmentShaderPath, const ConfigInfo& p_configInfo);

			void _createShaderModule(const std::vector<char>& p_code, vk::ShaderModule* p_shaderModule);

			Device& _device;
			vk::Pipeline _graphicsPipeline;
			vk::ShaderModule _vertexShaderModule;
			vk::ShaderModule _fragmentShaderModule;
		}; //? class Pipeline
	private:
		class Dispatcher
		{
		private:
			vk::DynamicLoader _dynamicLoader;
			vk::DispatchLoaderDynamic _dynamicDispatcher;

		public:
			Dispatcher()
			{
				PFN_vkGetInstanceProcAddr vkGetInstanceProcAddr = _dynamicLoader.getProcAddress<PFN_vkGetInstanceProcAddr>("vkGetInstanceProcAddr");

				init(vkGetInstanceProcAddr);
			}

			template <typename TType>
			void init(TType& p_initData)
			{
				_dynamicDispatcher.init(p_initData);
			}

			~Dispatcher() = default;

			vk::DispatchLoaderDynamic& dynamicDispatcher() { return _dynamicDispatcher; }
		}; //? class Dispatcher

	public:
#ifdef NDEBUG
		const bool enableValidationLayers = false;
#else
		const bool enableValidationLayers = true;
#endif

		Device(const Frame* p_frame);
		~Device();

		// Not copyable
		Device(const Device&) = delete;
		Device& operator=(const Device&) = delete;

		vk::CommandPool commandPool() { return _commandPool; }
		vk::Device device() { return _device; }
		vk::SurfaceKHR surface() { return _surface; }
		vk::Queue graphicsQueue() { return _graphicsQueue; }
		vk::Queue presentQueue() { return _presentQueue; }

		SwapChainSupportDetails swapChainSupport() { return _querySwapChainSupport(_physicalDevice); }
		uint32_t findMemoryType(uint32_t p_typeFilter, vk::MemoryPropertyFlags p_properties);
		QueueFamilyIndices findPhysicalQueueFamilies() { return _findQueueFamilies(_physicalDevice); }
		vk::Format findSupportedFormat(const std::vector<vk::Format>& p_candidates, vk::ImageTiling p_tiling, vk::FormatFeatureFlags p_features);

		// Buffer Helper Functions
		void createBuffer(vk::DeviceSize p_size, vk::BufferUsageFlags p_usage, vk::MemoryPropertyFlags p_properties, vk::Buffer& p_buffer, vk::DeviceMemory& p_bufferMemory);
		void copyBuffer(vk::Buffer p_srcBuffer, vk::Buffer p_dstBuffer, vk::DeviceSize p_size);
		void copyBufferToImage(vk::Buffer p_buffer, vk::Image p_image, uint32_t p_width, uint32_t p_height, uint32_t p_layerCount);

		void createImageWithInfo(const vk::ImageCreateInfo& p_imageInfo, vk::MemoryPropertyFlags p_properties, vk::Image& p_image, vk::DeviceMemory& p_imageMemory);

		vk::CommandBuffer beginSingleTimeCommands();
		void endSingleTimeCommands(vk::CommandBuffer p_commandBuffer);

		vk::PhysicalDeviceProperties properties;

	private:
		void _createInstance();
		void _setupDebugMessenger();
		void _createSurface(const Frame* p_frame);
		void _pickPhysicalDevice();
		void _createLogicalDevice();
		void _createCommandPool();

		// helper functions
		bool _isDeviceSuitable(vk::PhysicalDevice p_device);

		void _checkExtensionsSupport(const std::vector<const char*>& p_extensions);
		bool _checkValidationLayerSupport();
		void _addValidationLayers(vk::InstanceCreateInfo& p_createInfo,
			const std::vector<const char*>& p_layers,
			vk::DebugUtilsMessengerCreateInfoEXT& p_debugCreateInfo);

		QueueFamilyIndices _findQueueFamilies(vk::PhysicalDevice p_device);
		void _populateDebugMessengerCreateInfo(vk::DebugUtilsMessengerCreateInfoEXT& p_createInfo);
		bool _checkDeviceExtensionSupport(vk::PhysicalDevice p_device);
		SwapChainSupportDetails _querySwapChainSupport(vk::PhysicalDevice p_device);

		Dispatcher _dispatcher;
		vk::Instance _instance;
		vk::DebugUtilsMessengerEXT _debugMessenger;
		vk::PhysicalDevice _physicalDevice;
		vk::CommandPool _commandPool;

		vk::Device _device;
		vk::SurfaceKHR _surface;
		vk::Queue _graphicsQueue;
		vk::Queue _presentQueue;

		const std::vector<const char*> _validationLayers = { "VK_LAYER_KHRONOS_validation" };
		const std::vector<const char*> _deviceExtensions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };
	};
}