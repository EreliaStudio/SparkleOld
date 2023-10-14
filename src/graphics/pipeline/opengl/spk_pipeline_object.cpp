#include "graphics/pipeline/spk_pipeline.hpp"

namespace spk
{
	void Pipeline::OpenGLObject::_pushStorageDatas(const void *p_data, size_t p_size)
	{

	}

	void Pipeline::OpenGLObject::_pushConstantsDatas(const void *p_data, size_t p_size)
	{

	}
	
	void Pipeline::OpenGLObject::_pushIndexesDatas(const void* p_data, size_t p_size)
	{

	}

	
	void Pipeline::OpenGLObject::_onRender()
	{
		
	}
	
	Pipeline::OpenGLObject::OpenGLObject(Pipeline* p_owner, const spk::ShaderLayout::StorageBufferLayout& p_storageLayout, const spk::ShaderLayout::PushConstantLayout& p_pushConstantsLayout) : 
		spk::AbstractPipeline::Object(p_owner, p_storageLayout, p_pushConstantsLayout)
	{

	}
}