#include "graphics/pipeline/spk_pipeline.hpp"

namespace spk
{
	void Pipeline::OpenGLUniformBlock::_pushData()
	{
		_buffer.push(_data.data(), _data.size());
	}

	Pipeline::OpenGLUniformBlock::OpenGLUniformBlock(Pipeline* p_pipeline, const ShaderLayout::UniformBlockLayout& p_uniformBlockLayout) :
		UniformBlock(p_uniformBlockLayout),
		_buffer(p_pipeline->_program, p_uniformBlockLayout.type(), p_uniformBlockLayout.key().binding)
	{

	}

	std::shared_ptr<Pipeline::UniformBlock> Pipeline::_loadUniformBlock(const ShaderLayout::UniformBlockLayout& p_uniformBlockLayout)
	{
		return (std::make_shared<OpenGLUniformBlock>(this, p_uniformBlockLayout));
	}
}