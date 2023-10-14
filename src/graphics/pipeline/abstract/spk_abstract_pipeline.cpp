#include "graphics/pipeline/spk_abstract_pipeline.hpp"

#include "iostream/spk_iostream.hpp"

namespace spk
{
	void AbstractPipeline::_loadPipeline()
	{
		_loadProgram(_shaderLayout);
	}

	AbstractPipeline::AbstractPipeline(const ShaderModule &p_vertexInput, const ShaderModule &p_fragmentInput) :
		_shaderLayout(p_vertexInput, p_fragmentInput)
	{

	}

}