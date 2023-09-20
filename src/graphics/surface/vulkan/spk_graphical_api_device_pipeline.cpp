#include "graphics/surface/vulkan/spk_graphical_api_device.hpp"

#include <fstream>
#include <stdexcept>
#include <cassert>

namespace spk::GraphicalAPI
{
	Device::Pipeline::Pipeline(Device& p_device,
		const std::filesystem::path& p_vertexShaderPath,
		const std::filesystem::path& p_fragmentShaderPath,
		const ConfigInfo& p_configInfo)
		: _device(p_device)
	{
		_createGraphicsPipeline(p_vertexShaderPath, p_fragmentShaderPath, p_configInfo);
	}

	Device::Pipeline::~Pipeline()
	{
		_device.device().destroyShaderModule(_vertexShaderModule);
		_device.device().destroyShaderModule(_fragmentShaderModule);
		_device.device().destroyPipeline(_graphicsPipeline);
	}

	void Device::Pipeline::bind(vk::CommandBuffer p_commandBuffer)
	{
		p_commandBuffer.bindPipeline(vk::PipelineBindPoint::eGraphics, _graphicsPipeline);
	}

	void Device::Pipeline::defaultPipelineConfigInfo(ConfigInfo& p_configInfo)
	{
		//! p_configInfo.bindingDescriptions = Model::Vertex::bindingDescriptions();
		//! p_configInfo.attributeDescriptions = Model::Vertex::attributeDescriptions();

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


	std::vector<char> Device::Pipeline::_readFile(const std::filesystem::path& p_filePath)
	{
		std::ifstream file(p_filePath, std::ios::binary);
		if (file.is_open() == false)
		{
			throw std::runtime_error("Failed to open file: " + p_filePath.string());
		}

		std::vector<char> buffer = std::vector<char>(std::istreambuf_iterator<char>(file),
			std::istreambuf_iterator<char>());

		file.close();

		return buffer;
	}

	void Device::Pipeline::_createGraphicsPipeline(const std::filesystem::path& p_vertexShaderPath,
		const std::filesystem::path& p_fragmentShaderPath, const ConfigInfo& p_configInfo)
	{
		assert(p_configInfo.pipelineLayout != vk::PipelineLayout() &&
			"Cannot create graphics pipeline: no pipelineLayout provided in configInfo");
		assert(p_configInfo.renderPass != vk::RenderPass() &&
			"Cannot create graphics pipeline: no renderPass provided in configInfo");

		_createShaderModule(_readFile(p_vertexShaderPath), &_vertexShaderModule);
		_createShaderModule(_readFile(p_fragmentShaderPath), &_fragmentShaderModule);

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

	void Device::Pipeline::_createShaderModule(const std::vector<char>& p_code, vk::ShaderModule* p_shaderModule)
	{
		vk::ShaderModuleCreateInfo createInfo(
			vk::ShaderModuleCreateFlags(),
			p_code.size(),
			reinterpret_cast<const uint32_t*>(p_code.data())
		);
		if (_device.device().createShaderModule(&createInfo, nullptr, p_shaderModule) != vk::Result::eSuccess)
			spk::throwException(L"Failed to create shader module");
	}
}