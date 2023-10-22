#include "graphics/pipeline/spk_pipeline.hpp"

namespace spk
{
	Pipeline::Pipeline(const spk::ShaderModule &p_vertexInput, const spk::ShaderModule &p_fragmentInput) :
		AbstractPipeline(p_vertexInput, p_fragmentInput)
	{
		_loadPipeline();
	}

	void Pipeline::launch(const size_t &p_nbIndexes)
	{
		glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(p_nbIndexes), GL_UNSIGNED_INT, nullptr);
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