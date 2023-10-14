#include "graphics/pipeline/spk_abstract_pipeline.hpp"

namespace spk
{
	AbstractPipeline::Object::Object(AbstractPipeline *p_owner, const ShaderLayout::StorageBufferLayout& p_storageLayout, const ShaderLayout::PushConstantLayout& p_pushConstantsLayout) :
		_owner(p_owner),
		_storage(p_storageLayout),
		_pushConstants(p_pushConstantsLayout)
	{
	}

	AbstractPipeline::Object::Object(Object&& p_other) noexcept
		: _owner(p_other._owner), _storage(std::move(p_other._storage)), _pushConstants(std::move(p_other._pushConstants))
	{
		p_other._owner = nullptr;
	}

	AbstractPipeline::Object& AbstractPipeline::Object::operator=(Object&& p_other) noexcept
	{
		if (this != &p_other)
		{
			_owner = p_other._owner;
			_storage = std::move(p_other._storage);
			_pushConstants = std::move(p_other._pushConstants);
			p_other._owner = nullptr;
		}
		return (*this);
	}
	
	void AbstractPipeline::Object::render()
	{
		_onRender();
		_owner->launch(_indexes.size());
	}

	void AbstractPipeline::Object::updateStorage()
	{
		_pushStorageDatas(_storage.data(), _storage.size());
	}

	void AbstractPipeline::Object::updateConstants()
	{
		_pushConstantsDatas(_pushConstants.data(), _pushConstants.size());
	}

	AbstractPipeline::Object::Storage& AbstractPipeline::Object::storage()
	{
		return (_storage);
	}
	
	AbstractPipeline::Object::PushConstants& AbstractPipeline::Object::pushConstants()
	{
		return (_pushConstants);
	}
}
