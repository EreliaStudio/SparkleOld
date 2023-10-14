#include "graphics/pipeline/spk_pipeline.hpp"

namespace spk
{
	void Pipeline::OpenGLUniformBlock::_updateData(const void* p_data, size_t p_dataSize)
	{

	}

	Pipeline::OpenGLUniformBlock::OpenGLUniformBlock(const spk::ShaderLayout::UniformBlockLayout& p_uniformBlockLayout) :
		spk::AbstractPipeline::UniformBlock(p_uniformBlockLayout)
	{

	}

	void Pipeline::OpenGLUniformBlock::activate()
	{

	}

	void Pipeline::OpenGLUniformBlock::deactivate()
	{

	}
}