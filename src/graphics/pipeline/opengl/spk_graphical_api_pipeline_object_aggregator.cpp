#include "graphics/pipeline/opengl/spk_graphical_api_pipeline.hpp"

namespace spk::GraphicalAPI
{
	Pipeline::OpenGLObject::Aggregator::Aggregator(spk::GraphicalAPI::AbstractPipeline* p_owner) :
		_owner(p_owner)
	{
		_owner->activate();
		glGenVertexArrays(1, &_VAO);
		activate();
	}

	void Pipeline::OpenGLObject::Aggregator::activate()
	{
		glBindVertexArray(_VAO);
	}

	void Pipeline::OpenGLObject::Aggregator::deactivate()
	{
		glBindVertexArray(0);
	}
}