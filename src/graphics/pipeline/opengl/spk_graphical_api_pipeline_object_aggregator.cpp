#include "graphics/pipeline/opengl/spk_graphical_api_pipeline.hpp"

namespace spk::GraphicalAPI
{
	Pipeline::Aggregator::Aggregator(spk::GraphicalAPI::AbstractPipeline* p_owner) :
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