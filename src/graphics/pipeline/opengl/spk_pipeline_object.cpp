#include "graphics/pipeline/spk_pipeline.hpp"

namespace spk
{
	void Pipeline::OpenGLObject::_pushVerticesData(const uint8_t* p_data, size_t p_dataSize)
	{
		_storageBuffer.push(p_data, p_dataSize);
	}

	void Pipeline::OpenGLObject::_pushIndexesData(const uint8_t* p_data, size_t p_dataSize)
	{
		_indexesBuffer.push(p_data, p_dataSize);
	}

	void Pipeline::OpenGLObject::_pushPushConstantsData(const uint8_t* p_data, size_t p_dataSize)
	{

	}

	void Pipeline::OpenGLObject::_onRender()
	{
		_aggregator.activate();
		_storageBuffer.activate();
		_indexesBuffer.activate();
	}
	
	Pipeline::OpenGLObject::OpenGLObject(AbstractPipeline* p_owner, const spk::ShaderLayout::StorageBufferLayout& p_storageBufferLayout, const spk::ShaderLayout::PushConstantsLayout& p_pushConstantsLayout) : 
		Object(p_owner, p_storageBufferLayout, p_pushConstantsLayout),
		_aggregator(p_owner),
		_storageBuffer(Pipeline::Buffer::Mode::Data, p_storageBufferLayout),
		_indexesBuffer(Pipeline::Buffer::Mode::Indexes)
	{

	}

	std::shared_ptr<Pipeline::Object> Pipeline::_loadObject(const ShaderLayout::StorageBufferLayout& p_storageLayout, const ShaderLayout::PushConstantsLayout& p_pushConstantsLayout)
	{
		return (std::make_shared<OpenGLObject>(this, p_storageLayout, p_pushConstantsLayout));
	}
}