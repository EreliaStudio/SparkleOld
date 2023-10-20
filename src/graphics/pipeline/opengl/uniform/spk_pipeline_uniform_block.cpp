#include "graphics/pipeline/spk_pipeline.hpp"

namespace spk
{
	std::shared_ptr<Pipeline::UniformBlock> Pipeline::_loadUniformBlock(const ShaderLayout::UniformBlockLayout& p_uniformBlockLayout)
	{
		return (std::make_shared<OpenGLUniformBlock>(p_uniformBlockLayout));
	}
}