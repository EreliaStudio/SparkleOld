#include "graphics/pipeline/spk_pipeline.hpp"

namespace spk
{
	void Pipeline::OpenGLObject::_pushVerticesData(const uint8_t* p_data, size_t p_dataSize)
	{
		_verticesBuffer.push(p_data, p_dataSize);
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
		_verticesBuffer.activate();
		_indexesBuffer.activate();
	}
			
	static GLenum convertAttributeTypeToGLenum(const ShaderLayout::Data::Type &p_input)
	{
		switch (p_input)
		{
		case (ShaderLayout::Data::Type::Int):
			return (GL_INT);
		case (ShaderLayout::Data::Type::UInt):
			return (GL_UNSIGNED_BYTE);
		case (ShaderLayout::Data::Type::Float):
			return (GL_FLOAT);
		case (ShaderLayout::Data::Type::Structure):
			return (GL_UNSIGNED_BYTE);
		}
		spk::throwException(L"Unexpected data type [" + std::to_wstring(static_cast<size_t>(p_input)) + L"]");
		return (GL_NONE);
	}

	void Pipeline::OpenGLObject::_configureVerticesBuffer(const ShaderLayout::StorageBufferLayout& p_storageBufferLayout)
	{
		_verticesBuffer.activate();
		for (const auto& field : p_storageBufferLayout.fields())
		{
			glEnableVertexAttribArray(field.location);

			glVertexAttribPointer(
				field.location,
				field.data.format,
				convertAttributeTypeToGLenum(field.data.type),
				GL_FALSE,
				p_storageBufferLayout.stride(),
				(void*)(field.offset));
		}
	}
	
	Pipeline::OpenGLObject::OpenGLObject(AbstractPipeline* p_owner, const ShaderLayout::StorageBufferLayout& p_storageBufferLayout, const ShaderLayout::PushConstantsLayout& p_pushConstantsLayout) : 
		Object(p_owner, p_storageBufferLayout, p_pushConstantsLayout),
		_aggregator(),
		_verticesBuffer(GPU::Buffer::Mode::Vertices),
		_indexesBuffer(GPU::Buffer::Mode::Indexes)
	{
		_configureVerticesBuffer(p_storageBufferLayout);
	}

	std::shared_ptr<Pipeline::Object> Pipeline::_loadObject(const ShaderLayout::StorageBufferLayout& p_storageLayout, const ShaderLayout::PushConstantsLayout& p_pushConstantsLayout)
	{
		activate();
		return (std::make_shared<OpenGLObject>(this, p_storageLayout, p_pushConstantsLayout));
	}
}