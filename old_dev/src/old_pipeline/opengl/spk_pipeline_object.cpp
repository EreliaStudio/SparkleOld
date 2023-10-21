#include "graphics/pipeline/opengl/spk_pipeline.hpp"
#include "spk_basic_functions.hpp"
#include "graphics/pipeline/opengl/spk_pipeline_basic_functions.hpp"

namespace spk
{
	size_t Pipeline::OpenGLObject::_findFirstBindingAvailible(const spk::ShaderLayout& p_configuration)
	{
		size_t result = 0;
		
		for (;;)
		{
			auto it = std::find_if(
				p_configuration.uniformBlocks.begin(),
				p_configuration.uniformBlocks.end(),
				[result](const auto& uniform)
				{
					return uniform.binding == result;
				}
			);
			
			if (it == p_configuration.uniformBlocks.end())
			{
				return result;
			}
			
			++result;
		}
		return 0;
	}

	Pipeline::OpenGLObject::OpenGLObject(spk::Pipeline* p_owner) :
		spk::AbstractPipeline::Object(p_owner),
		_aggregator(p_owner),
		_modelBuffer(Pipeline::Buffer::Mode::Data, p_owner->configuration().storage),
		_indexesBuffer(Pipeline::Buffer::Mode::Indexes),
		_pushConstantBuffer(p_owner->program(), p_owner->configuration().constants.type, _findFirstBindingAvailible(p_owner->configuration()))
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