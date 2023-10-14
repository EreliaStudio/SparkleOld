#include "graphics/pipeline/spk_pipeline.hpp"

namespace spk
{
	std::shared_ptr<Pipeline::UniformBlock> Pipeline::_loadUniformBlock(const spk::ShaderLayout::UniformBlockLayout& p_uniformBlockLayout)
	{
		return (std::make_shared<OpenGLUniformBlock>(p_uniformBlockLayout));
	}
	
	std::shared_ptr<Pipeline::SamplerUniform> Pipeline::_loadSamplerUniform(const spk::ShaderLayout::UniformBlockLayout& p_uniformBlockLayout)
	{
		return (std::make_shared<OpenGLSamplerUniform>(p_uniformBlockLayout));
	}

	std::shared_ptr<Pipeline::Object> Pipeline::_loadObject(const spk::ShaderLayout::StorageBufferLayout& p_storageLayout, const spk::ShaderLayout::PushConstantLayout& p_pushConstantsLayout)
	{
		return (std::make_shared<OpenGLObject>(this, p_storageLayout, p_pushConstantsLayout));
	}

	Pipeline::Pipeline(const spk::ShaderModule &p_vertexInput, const spk::ShaderModule &p_fragmentInput) :
		AbstractPipeline(p_vertexInput, p_fragmentInput)
	{
		_loadPipeline();
	}

	void Pipeline::launch(const size_t &p_nbVertex)
	{

	}

	void Pipeline::activate()
	{
		glUseProgram(_program);
	}

	void Pipeline::deactivate()
	{
		glUseProgram(0);
	}
}