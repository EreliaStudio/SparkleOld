#pragma once

#include <vulkan/vulkan.hpp>
#include "graphics/surface/vulkan/spk_graphical_api_device.hpp"
#include "graphics/surface/vulkan/spk_graphical_api_renderer.hpp"
#include <filesystem>
#include "graphics/pipeline/spk_graphical_api_abstract_pipeline.hpp"

namespace spk::GraphicalAPI
{
	class Surface;

	class Pipeline : public spk::GraphicalAPI::AbstractPipeline
	{
		friend class spk::GraphicalAPI::Surface;
	private:

		class VulkanHandle
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

			VulkanHandle(Device& p_device,
				const std::vector<unsigned int>& p_vertexSPVCode,
				const std::vector<unsigned int>& p_fragmentSPVCode,
				const ConfigInfo& p_configInfo);

			~VulkanHandle();

			static void defaultConfigInfo(ConfigInfo& p_configInfo);

			void bind(vk::CommandBuffer p_commandBuffer);

			vk::Pipeline& pipeline() { return _graphicsPipeline; }

		private:

			void _createShaderModule(const std::vector<unsigned int>& p_SPVCode, vk::ShaderModule* p_shaderModule);

			void _createGraphicsPipeline(
				const std::vector<unsigned int>& p_vertexSPVCode,
				const std::vector<unsigned int>& p_fragmentSPVCode,
				const ConfigInfo& p_configInfo);

			Device& _device;
			vk::Pipeline _graphicsPipeline;
			vk::ShaderModule _vertexShaderModule;
			vk::ShaderModule _fragmentShaderModule;
		}; // class VulkanHandle

		class VulkanObject : public spk::GraphicalAPI::AbstractPipeline::Object
		{
		public:
			VulkanObject(spk::GraphicalAPI::AbstractPipeline* p_owner);

			void push();

			void activate();
			void deactivate();

			static std::vector<vk::VertexInputBindingDescription> bindingDescriptions(const spk::GraphicalAPI::AbstractPipeline::Configuration::StorageLayout& p_storageConfiguration);
			static std::vector<vk::VertexInputAttributeDescription> attributeDescriptions(const spk::GraphicalAPI::AbstractPipeline::Configuration::StorageLayout& p_storageConfiguration);

		}; // class VulkanObject

		static inline Device* _linkedDevice = nullptr;
		static inline Renderer* _linkedRenderer = nullptr;
		vk::PipelineLayout _pipelineLayout;
		VulkanHandle::ConfigInfo _configInfo;
		std::unique_ptr<VulkanHandle> _vulkanHandle;
		
		void _createPipelineLayout(/*vk::DescriptorSetLayout p_descriptorSetLayout*/);

		void _loadProgram(
			const std::string& p_vertexName, const std::string& p_vertexCode,
			const std::string& p_fragmentName, const std::string& p_fragmentCode);

	public:
		Pipeline(
			const std::filesystem::path& p_vertexShaderPath,
			const std::filesystem::path& p_fragmentShaderPath);

		~Pipeline();

		void activate();
		void deactivate();

		void launch(const size_t& p_nbIndexes);

		std::shared_ptr<Pipeline::Object> createObject();
	}; // class Pipeline
} // namespace spk::GraphicalAPI