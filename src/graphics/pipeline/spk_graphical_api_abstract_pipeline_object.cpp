#include "graphics/pipeline/spk_graphical_api_abstract_pipeline.hpp"

namespace spk::GraphicalAPI
{
	AbstractPipeline::Object::Object(AbstractPipeline* p_owner, const Storage::Configuration& p_storageConfiguration) :
		_owner(p_owner),
		_storage(p_storageConfiguration)
	{
	}

	void AbstractPipeline::Object::activate()
	{
		_owner->_activateObject(this);
	}

	void AbstractPipeline::Object::deactivate()
	{
		_owner->_activateObject(this);
	}

	void AbstractPipeline::Object::push()
	{
		_owner->_pushStorageData(_storage.data(), _storage.size());
	}

	void AbstractPipeline::Object::render()
	{
		_owner->_renderObject(this);
	}

	AbstractPipeline::Object::Storage& AbstractPipeline::Object::storage()
	{
		return (_storage);
	}
}