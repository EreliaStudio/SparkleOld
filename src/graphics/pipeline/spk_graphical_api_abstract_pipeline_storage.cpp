#include "graphics/pipeline/spk_graphical_api_abstract_pipeline.hpp"

namespace spk::GraphicalAPI
{
	AbstractPipeline::Object::Storage::Storage(const Storage::Configuration& p_storageConfiguration) :
		_configuration(p_storageConfiguration)
	{
		
	}

	void AbstractPipeline::Object::Storage::clear()
	{
		_content.clear();
	}
}