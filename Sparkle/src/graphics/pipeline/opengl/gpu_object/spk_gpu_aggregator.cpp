#include "graphics/pipeline/spk_pipeline.hpp"

namespace spk::GPU
{
	Aggregator::Aggregator()
	{
		glGenVertexArrays(1, &_VAO);
		activate();
	}

	void Aggregator::activate()
	{
		glBindVertexArray(_VAO);
	}

	void Aggregator::deactivate()
	{
		glBindVertexArray(0);
	}

    GLuint Aggregator::vao() const
	{
		return (_VAO);
	}
}