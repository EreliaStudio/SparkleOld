#include "graphics/pipeline/vulkan/spk_pipeline.hpp"
#include "spk_basic_functions.hpp"

#include <cassert>

namespace spk
{
	Pipeline::VulkanHandle::VulkanHandle(Device& p_device,
		const std::vector<unsigned int>& p_vertexSPVCode,
		const std::vector<unsigned int>& p_fragmentSPVCode,
		const ConfigInfo& p_configInfo) :
		_device(p_device)
	{
		_createGraphicsPipeline(p_vertexSPVCode, p_fragmentSPVCode, p_configInfo);
	}

	Pipeline::VulkanHandle::~VulkanHandle()
	{
		_device.device().destroyShaderModule(_vertexShaderModule);
		_device.device().destroyShaderModule(_fragmentShaderModule);

		_device.device().destroyPipeline(_graphicsPipeline);
	}

	void Pipeline::VulkanHandle::defaultConfigInfo(ConfigInfo& p_configInfo)
	{
		p_configInfo.inputAssemblyInfo = vk::PipelineInputAssemblyStateCreateInfo({},
			vk::PrimitiveTopology::eTriangleList,
			false ///< Primitive restart
		);

		p_configInfo.viewportInfo = vk::PipelineViewportStateCreateInfo(
			vk::PipelineViewportStateCreateFlags(),
			1, nullptr,
			1, nullptr
		);

		p_configInfo.rasterizationInfo = vk::PipelineRasterizationStateCreateInfo({},
			false, ///< Depth clamp
			false, ///< Rasterizer discard
			vk::PolygonMode::eFill,
			vk::CullModeFlagBits::eNone,
			vk::FrontFace::eCounterClockwise,
			false, 0.0f, 0.0f, 0.0f, ///< Depth bias
			1.0f ///< Line width
		);

		p_configInfo.multisampleInfo = vk::PipelineMultisampleStateCreateInfo({},
			vk::SampleCountFlagBits::e1,
			false, ///< Sample shading
			1.0f, ///< Min sample shading
			nullptr, ///< Sample mask
			false, ///< Alpha to coverage
			false ///< Alpha to one
		);

		p_configInfo.colorBlendAttachment = vk::PipelineColorBlendAttachmentState(
			false, ///< Blend
			vk::BlendFactor::eOne, ///< Src color blend factor
			vk::BlendFactor::eZero, ///< Dst color blend factor
			vk::BlendOp::eAdd, ///< Color blend op
			vk::BlendFactor::eOne, ///< Src alpha blend factor
			vk::BlendFactor::eZero, ///< Dst alpha blend factor
			vk::BlendOp::eAdd, ///< Alpha blend op
			vk::ColorComponentFlagBits::eR | ///< Color write mask
			vk::ColorComponentFlagBits::eG |
			vk::ColorComponentFlagBits::eB |
			vk::ColorComponentFlagBits::eA
		);

		p_configInfo.colorBlendInfo = vk::PipelineColorBlendStateCreateInfo({},
			false, ///< Logic op
			vk::LogicOp::eCopy,
			1, &p_configInfo.colorBlendAttachment,
			{ 0.0f, 0.0f, 0.0f, 0.0f } ///< Blend constants
		);

		p_configInfo.depthStencilInfo = vk::PipelineDepthStencilStateCreateInfo(
			vk::PipelineDepthStencilStateCreateFlags(),
			true, ///< Depth test
			true, ///< Depth write
			vk::CompareOp::eLess,
			false, ///< Depth bounds
			false, ///< Stencil test
			{}, ///< Front
			{}, ///< Back
			0.0f, ///< Min depth bounds
			1.0f ///< Max depth bounds
		);

		p_configInfo.dynamicStateEnables = { vk::DynamicState::eViewport, vk::DynamicState::eScissor };

		p_configInfo.dynamicStateInfo = vk::PipelineDynamicStateCreateInfo({},
			static_cast<uint32_t>(p_configInfo.dynamicStateEnables.size()),
			p_configInfo.dynamicStateEnables.data()
		);
	}

	void Pipeline::VulkanHandle::bind(vk::CommandBuffer p_commandBuffer)
	{
		p_commandBuffer.bindPipeline(vk::PipelineBindPoint::eGraphics, _graphicsPipeline);
	}

	void Pipeline::VulkanHandle::_createShaderModule(const std::vector<unsigned int>& p_SPVCode, vk::ShaderModule* p_shaderModule)
	{
		vk::ShaderModuleCreateInfo createInfo(
			vk::ShaderModuleCreateFlags(),
			p_SPVCode.size() * sizeof(unsigned int),
			p_SPVCode.data()
		);

		if (_device.device().createShaderModule(&createInfo, nullptr, p_shaderModule) != vk::Result::eSuccess)
			spk::throwException(L"Failed to create shader module");
	}

	void Pipeline::VulkanHandle::_createGraphicsPipeline(
		const std::vector<unsigned int>& p_vertexSPVCode,
		const std::vector<unsigned int>& p_fragmentSPVCode,
		const ConfigInfo& p_configInfo)
	{
		assert(p_configInfo.bindingDescriptions.size() > 0 &&
			"Cannot create graphics pipeline: no bindingDescriptions provided in configInfo");
		assert(p_configInfo.attributeDescriptions.size() > 0 &&
			"Cannot create graphics pipeline: no attributeDescriptions provided in configInfo");
		assert(p_configInfo.pipelineLayout != vk::PipelineLayout() &&
			"Cannot create graphics pipeline: no pipelineLayout provided in configInfo");
		assert(p_configInfo.renderPass != vk::RenderPass() &&
			"Cannot create graphics pipeline: no renderPass provided in configInfo");

		_createShaderModule(p_vertexSPVCode, &_vertexShaderModule);
		_createShaderModule(p_fragmentSPVCode, &_fragmentShaderModule);

		vk::PipelineShaderStageCreateInfo shaderStages[] = {
			vk::PipelineShaderStageCreateInfo({},
				vk::ShaderStageFlagBits::eVertex,
				_vertexShaderModule,
				"main"
			),
			vk::PipelineShaderStageCreateInfo({},
				vk::ShaderStageFlagBits::eFragment,
				_fragmentShaderModule,
				"main"
			)
		};

		auto& bindingDescriptions = p_configInfo.bindingDescriptions;
		auto& attributeDescriptions = p_configInfo.attributeDescriptions;

		vk::PipelineVertexInputStateCreateInfo vertexInputInfo({},
			static_cast<uint32_t>(bindingDescriptions.size()), bindingDescriptions.data(),
			static_cast<uint32_t>(attributeDescriptions.size()), attributeDescriptions.data()
		);

		vk::GraphicsPipelineCreateInfo pipelineInfo({}, ///< Flags
			2, shaderStages,
			&vertexInputInfo,
			&p_configInfo.inputAssemblyInfo,
			nullptr, ///< Tessellation state
			&p_configInfo.viewportInfo,
			&p_configInfo.rasterizationInfo,
			&p_configInfo.multisampleInfo,
			&p_configInfo.depthStencilInfo,
			&p_configInfo.colorBlendInfo,
			&p_configInfo.dynamicStateInfo,
			p_configInfo.pipelineLayout,
			p_configInfo.renderPass,
			p_configInfo.subpass,
			nullptr, ///< Base pipeline handle
			-1 ///< Base pipeline index
		);

		if (_device.device().createGraphicsPipelines(nullptr, 1, &pipelineInfo, nullptr, &_graphicsPipeline) != vk::Result::eSuccess)
			spk::throwException(L"Failed to create graphics pipeline");
	}
}