#include "graphics/pipeline/spk_abstract_pipeline.hpp"

namespace spk
{
	void AbstractPipeline::Object::_updateVertices()
	{
		_pushVerticesData(_storage.vertices().data(), _storage.vertices().size());
		_storage.vertices().clear();
		_storage.vertices().setUpdateStatus(false);
	}

	void AbstractPipeline::Object::_updateIndexes()
	{
		_pushIndexesData(_storage.indexes().data(), _storage.indexes().size());
		_nbIndexesPushed = _storage.indexes().size() / sizeof(uint32_t);
		_storage.indexes().clear();
		_storage.indexes().setUpdateStatus(false);
	}

	void AbstractPipeline::Object::_updatePushConstants()
	{
		_pushPushConstantsData(_pushConstants.data(), _pushConstants.size());
		_pushConstants.setUpdateStatus(false);
	}

	AbstractPipeline::Object::Object(AbstractPipeline *p_owner, const spk::ShaderLayout::StorageBufferLayout &p_storageBufferLayout, const spk::ShaderLayout::PushConstantsLayout &p_pushConstantsLayout) :
		_owner(p_owner),
		_storage(p_storageBufferLayout),
		_pushConstants(p_pushConstantsLayout),
		_nbIndexesPushed(0)
	{
	}

	void AbstractPipeline::Object::render()
	{
		_owner->activate();

		if (_storage.vertices().needUpdate() == true)
			_updateVertices();
		if (_storage.indexes().needUpdate() == true)
			_updateIndexes();
		if (_pushConstants.needUpdate() == true)
			_updatePushConstants();

		_onRender();

		_owner->launch(_nbIndexesPushed);

		_owner->deactivate();
	}

	AbstractPipeline::Object::Storage &AbstractPipeline::Object::storage()
	{
		return (_storage);
	}

	AbstractPipeline::Object::PushConstants &AbstractPipeline::Object::pushConstants()
	{
		return (_pushConstants);
	}

	AbstractPipeline::Object::PushConstants::Field& AbstractPipeline::Object::pushConstants(const std::wstring& p_fieldName)
	{
		return (_pushConstants[p_fieldName]);
	}

	const size_t& AbstractPipeline::Object::nbIndexes() const
	{
		return (_nbIndexesPushed);
	}
}