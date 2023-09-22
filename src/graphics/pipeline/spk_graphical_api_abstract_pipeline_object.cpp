#include "graphics/pipeline/spk_graphical_api_abstract_pipeline.hpp"

namespace spk::GraphicalAPI
{
	AbstractPipeline::Object::Object(AbstractPipeline* p_owner, const Storage::Configuration& p_storageConfiguration) :
		_owner(p_owner),
		_storage(p_storageConfiguration)
	{
		
	}

	AbstractPipeline::Object::Storage& AbstractPipeline::Object::storage()
	{
		return (_storage);
	}
}