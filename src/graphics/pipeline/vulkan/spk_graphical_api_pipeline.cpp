#include "graphics/pipeline/vulkan/spk_graphical_api_pipeline.hpp"
#include "spk_basic_functions.hpp"
#include "graphics/pipeline/vulkan/spk_graphical_api_pipeline_basic_functions.hpp"

namespace spk::GraphicalAPI
{
	Pipeline::Pipeline(const std::filesystem::path& p_vertexShaderPath, const std::filesystem::path& p_fragmentShaderPath) :
		spk::GraphicalAPI::AbstractPipeline()
	{
		_createPipelineLayout();
		loadFromFile(p_vertexShaderPath, p_fragmentShaderPath);
	}

	Pipeline::~Pipeline()
	{
		_linkedDevice->device().waitIdle();
		_linkedDevice->device().destroyPipelineLayout(_pipelineLayout);
	}

	void Pipeline::activate()
	{
		// _linkedRenderer->currentCommandBuffer().bindPipeline(vk::PipelineBindPoint::eGraphics, _vulkanHandle->pipeline());
	}

	void Pipeline::deactivate()
	{
		// _linkedRenderer->currentCommandBuffer().bindPipeline(vk::PipelineBindPoint::eGraphics, vk::Pipeline(nullptr));
	}

	void Pipeline::launch(const size_t& p_nbIndexes)
	{
		// _linkedRenderer->currentCommandBuffer().drawIndexed(static_cast<uint32_t>(p_nbIndexes), 1, 0, 0, 0);
	}

	std::shared_ptr<Pipeline::Object> Pipeline::createObject()
	{
		return (std::make_shared<VulkanObject>(this, _linkedDevice, _configuration));
	}

	void Pipeline::_createPipelineLayout()
	{
		std::vector<vk::DescriptorSetLayout> descriptorSetLayouts = { };

		vk::PipelineLayoutCreateInfo pipelineLayoutInfo(
			vk::PipelineLayoutCreateFlags(),
			static_cast<uint32_t>(descriptorSetLayouts.size()), descriptorSetLayouts.data(),
			0, nullptr ///< Push constant
		);
		if (_linkedDevice->device().createPipelineLayout(&pipelineLayoutInfo, nullptr, &_pipelineLayout) != vk::Result::eSuccess)
			spk::throwException(L"Failed to create pipeline layout");
	}

	void Pipeline::_loadProgram(
		const std::string& p_vertexName, const std::string& p_vertexCode,
		const std::string& p_fragmentName, const std::string& p_fragmentCode)
	{
		std::vector<unsigned int> vertexSPIRVCode;
		std::vector<unsigned int> fragmentSPIRVCode;

		if (SpirvHelper::GLSLtoSPV(vk::ShaderStageFlagBits::eVertex, p_vertexCode.c_str(), vertexSPIRVCode) == false)
			spk::throwException(L"Failed to convert vertex shader to SPIR-V");
		if (SpirvHelper::GLSLtoSPV(vk::ShaderStageFlagBits::eFragment, p_fragmentCode.c_str(), fragmentSPIRVCode) == false)
			spk::throwException(L"Failed to convert fragment shader to SPIR-V");

		VulkanHandle::defaultConfigInfo(_configInfo);
		_configInfo.bindingDescriptions = VulkanObject::bindingDescriptions(_configuration.storage);
		_configInfo.attributeDescriptions = VulkanObject::attributeDescriptions(_configuration.storage);
		_configInfo.pipelineLayout = _pipelineLayout;
		_configInfo.renderPass = _linkedRenderer->renderPass();

		_vulkanHandle = std::make_unique<VulkanHandle>(*_linkedDevice, vertexSPIRVCode, fragmentSPIRVCode, _configInfo);
	}
}