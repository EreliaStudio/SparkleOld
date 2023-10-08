#include "graphics/pipeline/spk_graphical_api_abstract_pipeline.hpp"

#include "graphics/pipeline/spk_graphical_api_pipeline_basic_functions.hpp"

namespace spk::GraphicalAPI
{
	AbstractPipeline::Object::Object(AbstractPipeline* p_owner) :
		_owner(p_owner),
		_storage(p_owner->configuration().storage),
		_indexes(),
		_pushConstants(p_owner->configuration().constants)
	{

	}

	void AbstractPipeline::Object::render()
	{
		_owner->activate();
		activate();
		_owner->launch(_indexes.nbIndexes());
		deactivate();
		_owner->deactivate();
	}

	AbstractPipeline::Object::Storage& AbstractPipeline::Object::storage()
	{
		return (_storage);
	}
	
	AbstractPipeline::Object::Indexes& AbstractPipeline::Object::indexes()
	{
		return (_indexes);
	}

	AbstractPipeline::Object::PushConstants &AbstractPipeline::Object::pushConstants()
	{
		return (_pushConstants);
	}
}