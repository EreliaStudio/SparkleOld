#include "graphics/pipeline/opengl/spk_graphical_api_pipeline.hpp"
#include "spk_basic_functions.hpp"
#include "graphics/pipeline/opengl/spk_graphical_api_pipeline_basic_functions.hpp"

namespace spk::GraphicalAPI
{
	Pipeline::OpenGLObject::OpenGLObject(spk::GraphicalAPI::AbstractPipeline* p_owner) :
		spk::GraphicalAPI::AbstractPipeline::Object(p_owner),
		_aggregator(p_owner),
		_modelBuffer(Pipeline::OpenGLObject::Buffer::Mode::Data, p_owner->configuration().storage),
		_indexesBuffer(Pipeline::OpenGLObject::Buffer::Mode::Indexes),
		_pushConstantBuffer(static_cast<Pipeline*>(p_owner)->program(), p_owner->configuration().constants.type, p_owner->configuration().constants.binding)
	{
		
	}

	void Pipeline::OpenGLObject::push()
	{
		_aggregator.activate();
		_modelBuffer.push(storage().data(), storage().size());
		_indexesBuffer.push(indexes().data(), indexes().size());
		_aggregator.deactivate();
	}

	void Pipeline::OpenGLObject::updateConstants()
	{
		_aggregator.activate();
		_pushConstantBuffer.push(pushConstants().data(), pushConstants().size());
		_aggregator.deactivate();
	}

	void Pipeline::OpenGLObject::activate()
	{
		_aggregator.activate();
		_modelBuffer.activate();
		_indexesBuffer.activate();
	}

	void Pipeline::OpenGLObject::deactivate()
	{
		_aggregator.deactivate();
		_modelBuffer.deactivate();
		_indexesBuffer.deactivate();
	}
}