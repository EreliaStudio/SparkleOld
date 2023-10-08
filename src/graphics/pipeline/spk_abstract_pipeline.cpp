#include "graphics/pipeline/spk_abstract_pipeline.hpp"

#include "iostream/spk_iostream.hpp"

namespace spk
{
	void AbstractPipeline::_loadPipeline(const ShaderModule &p_vertexInput, const ShaderModule &p_fragmentInput)
	{
		Configuration configuration = Configuration(p_vertexInput, p_fragmentInput);

		spk::cout << configuration << std::endl;

		_loadProgram(configuration, p_vertexInput, p_fragmentInput);
	}
}