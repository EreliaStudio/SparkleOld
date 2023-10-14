#include "graphics/pipeline/spk_pipeline.hpp"

namespace spk
{
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