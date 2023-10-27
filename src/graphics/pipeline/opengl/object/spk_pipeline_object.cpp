#include "graphics/pipeline/spk_pipeline.hpp"
#include <functional>

namespace spk
{
	void Pipeline::OpenGLObject::_pushVerticesData(const uint8_t* p_data, size_t p_dataSize)
	{
		_aggregator.activate();
		_storageBuffer.pushVerticesData(p_data, p_dataSize);
	}

	void Pipeline::OpenGLObject::_pushIndexesData(const uint8_t* p_data, size_t p_dataSize)
	{
		_aggregator.activate();
		_storageBuffer.pushIndexesData(p_data, p_dataSize);
	}

	void Pipeline::OpenGLObject::_pushPushConstantsData(const uint8_t* p_data, size_t p_dataSize)
	{
		_aggregator.activate();
		_pushConstantBuffer.push(p_data, p_dataSize);
	}

	void Pipeline::OpenGLObject::_onRender()
	{
		_aggregator.activate();
		_storageBuffer.activate();

		if (_pushConstantBuffer.isValid() == true)
			_pushConstantBuffer.activate();
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

	void Pipeline::OpenGLObject::_configureStorageBuffer(const ShaderLayout::StorageBufferLayout& p_storageBufferLayout)
	{
		_storageBuffer.activate();
		for (const auto& field : p_storageBufferLayout.fields())
		{
			_storageBuffer.addStorageAttribute(
					field.location,
					field.data.format,
					convertAttributeTypeToGLenum(field.data.type),
					field.offset,
					p_storageBufferLayout.stride()
				);
		}
		_storageBuffer.deactivate();
	}
	
	/**
	 * @brief Finds the first available binding slot for a UniformBlock in a given ShaderLayout.
	 * 
	 * @param p_shaderLayout The ShaderLayout to search within.
	 * @return The first available binding slot.
	 */
	size_t findFirstBindingAvailable(const spk::ShaderLayout& p_shaderLayout)
	{
		size_t result = 0;
		
		auto searchLambda = [](size_t p_target, const std::vector<spk::ShaderLayout::UniformBlockLayout>& p_uniformBlockVector) -> bool
		{
			for (const auto& uniformBlock : p_uniformBlockVector)
			{
				if (uniformBlock.key().binding == p_target)
					return (false);
			}
			return (true);
		};

		while (searchLambda(result, p_shaderLayout.uniformBlockLayouts()) == false)
		{
			result++;
		}

		return result;
	}

	Pipeline::OpenGLObject::OpenGLObject(AbstractPipeline* p_owner, const ShaderLayout::StorageBufferLayout& p_storageBufferLayout, const ShaderLayout::PushConstantsLayout& p_pushConstantsLayout) : 
		Object(p_owner, p_storageBufferLayout, p_pushConstantsLayout),
		_aggregator(),
		_storageBuffer(),
		_pushConstantBuffer()
	{
		if (p_pushConstantsLayout.type() != L"")
		{
			_pushConstantBuffer = GPU::UniformBlockBuffer(static_cast<Pipeline*>(p_owner)->_program, p_pushConstantsLayout.type(), findFirstBindingAvailable(static_cast<Pipeline*>(p_owner)->_shaderLayout));
		}
		_configureStorageBuffer(p_storageBufferLayout);

		_aggregator.deactivate();
	}

	std::shared_ptr<Pipeline::Object> Pipeline::_loadObject(const ShaderLayout::StorageBufferLayout& p_storageLayout, const ShaderLayout::PushConstantsLayout& p_pushConstantsLayout)
	{
		activate();
		return (std::make_shared<OpenGLObject>(this, p_storageLayout, p_pushConstantsLayout));
	}
}