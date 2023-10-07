#include "pipeline.hpp"

void AbstractPipeline::_loadPipeline(const ShaderModule &p_vertexInput, const ShaderModule &p_fragmentInput)
{
	Configuration configuration = Configuration(p_vertexInput, p_fragmentInput);
	_loadProgram(configuration, p_vertexInput, p_fragmentInput);
}
