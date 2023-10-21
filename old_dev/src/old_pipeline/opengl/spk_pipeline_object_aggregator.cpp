#include "graphics/pipeline/opengl/spk_pipeline.hpp"

namespace spk
{
	Pipeline::Aggregator::Aggregator(spk::AbstractPipeline* p_owner) :
		_owner(p_owner)
	{
		_owner->activate();
		glGenVertexArrays(1, &_VAO);
		activate();
	}

	void Pipeline::Aggregator::activate()
	{
		glBindVertexArray(_VAO);
	}

	void Pipeline::Aggregator::deactivate()
	{
		glBindVertexArray(0);
	}
}