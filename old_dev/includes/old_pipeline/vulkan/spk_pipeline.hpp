#pragma once

#include <vulkan/vulkan.hpp>
#include "graphics/surface/vulkan/spk_device.hpp"
#include "graphics/surface/vulkan/spk_renderer.hpp"
#include <filesystem>
#include "graphics/pipeline/spk_abstract_pipeline.hpp"
#include "graphics/surface/vulkan/spk_buffer.hpp"

namespace spk
{
	class Surface;

	class Pipeline : public spk::AbstractPipeline
	{
		friend class spk::Surface;
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

		class VulkanObject : public spk::AbstractPipeline::Object
		{
		public:
			VulkanObject(spk::AbstractPipeline* p_owner,
				Device* p_linkedDevice, const spk::ShaderLayout& p_configuration);

			void push();
			void updateConstants();

			void activate();
			void deactivate();

			static std::vector<vk::VertexInputBindingDescription> bindingDescriptions(const spk::ShaderLayout::StorageLayout& p_storageConfiguration);
			static std::vector<vk::VertexInputAttributeDescription> attributeDescriptions(const spk::ShaderLayout::StorageLayout& p_storageConfiguration);

		private:

			void _createVertexBuffers();
			void _createIndexBuffers();

			Device* _linkedDevice;
			ShaderLayout _configuration;

			std::unique_ptr<Buffer> _vertexBuffer;
			uint32_t _vertexCount;

			bool _hasIndexBuffer;
			std::unique_ptr<Buffer> _indexBuffer;
			uint32_t _indexCount;
		}; // class VulkanObject

		static inline Device* _linkedDevice = nullptr;
		static inline Renderer* _linkedRenderer = nullptr;
		vk::PipelineLayout _pipelineLayout;
		VulkanHandle::ConfigInfo _configInfo;
		std::unique_ptr<VulkanHandle> _vulkanHandle;

		void _createPipelineLayout(/*vk::DescriptorSetLayout p_descriptorSetLayout*/);
		std::unique_ptr<UniformBlock> _createUniformBlock(const Configuration::UniformBlockLayout& p_layout);

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

		std::unique_ptr<Pipeline::Object> createObject();
	}; // class Pipeline
} // namespace spk