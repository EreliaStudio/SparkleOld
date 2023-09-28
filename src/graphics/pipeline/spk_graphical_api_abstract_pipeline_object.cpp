#include "graphics/pipeline/spk_graphical_api_abstract_pipeline.hpp"

namespace spk::GraphicalAPI
{
	AbstractPipeline::Object::Object(AbstractPipeline* p_owner) :
		_owner(p_owner),
		_storage(p_owner->configuration().storage),
		_indexes()
	{
		
	}

	void AbstractPipeline::Object::render()
	{

	}

	AbstractPipeline::Object::Storage& AbstractPipeline::Object::storage()
	{
		return (_storage);
	}
	
	AbstractPipeline::Object::Indexes& AbstractPipeline::Object::indexes()
	{
		return (_indexes);
	}
}