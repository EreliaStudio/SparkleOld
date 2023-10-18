#include "graphics/pipeline/spk_pipeline.hpp"

namespace spk
{
	void Pipeline::OpenGLObject::_pushVerticesData(const uint8_t* p_data, size_t p_dataSize)
	{

	}

	void Pipeline::OpenGLObject::_pushIndexesData(const uint8_t* p_data, size_t p_dataSize)
	{

	}

	void Pipeline::OpenGLObject::_pushPushConstantsData(const uint8_t* p_data, size_t p_dataSize)
	{

	}

	void Pipeline::OpenGLObject::_onRender()
	{

	}
	
	Pipeline::OpenGLObject::OpenGLObject(AbstractPipeline* p_owner, const spk::ShaderLayout::StorageBufferLayout& p_storageBufferLayout, const spk::ShaderLayout::PushConstantsLayout& p_pushConstantsLayout) : 
		Object(p_owner, p_storageBufferLayout, p_pushConstantsLayout)
	{

	}

	std::shared_ptr<Pipeline::Object> Pipeline::_loadObject(const ShaderLayout::StorageBufferLayout& p_storageLayout, const ShaderLayout::PushConstantsLayout& p_pushConstantsLayout)
	{
		return (std::make_shared<OpenGLObject>(this, p_storageLayout, p_pushConstantsLayout));
	}
}