#include "graphics/pipeline/vulkan/spk_graphical_api_pipeline.hpp"
#include "graphics/pipeline/vulkan/spk_graphical_api_pipeline_basic_functions.hpp"

namespace spk::GraphicalAPI
{
	Pipeline::VulkanObject::VulkanObject(spk::GraphicalAPI::AbstractPipeline* p_owner,
		Device* p_linkedDevice, const spk::GraphicalAPI::ShaderLayout& p_configuration) :
		spk::GraphicalAPI::AbstractPipeline::Object(p_owner),
		_linkedDevice(p_linkedDevice), _configuration(p_configuration)
	{
	}

	void Pipeline::VulkanObject::push()
	{
		_createVertexBuffers();
		if (indexes().size() > 0)
			_createIndexBuffers();
	}

	void Pipeline::VulkanObject::updateConstants()
	{

	}
	
	void Pipeline::VulkanObject::activate()
	{

	}
	void Pipeline::VulkanObject::deactivate()
	{

	}

	std::vector<vk::VertexInputBindingDescription> Pipeline::VulkanObject::bindingDescriptions(const spk::GraphicalAPI::ShaderLayout::StorageLayout& p_storageConfiguration)
	{
		std::vector<vk::VertexInputBindingDescription> bindingDescriptions;
		bindingDescriptions.reserve(1);

		bindingDescriptions.push_back(vk::VertexInputBindingDescription(
			0, ///< Binding
			p_storageConfiguration.stride, ///< Stride
			vk::VertexInputRate::eVertex
		));

		return (bindingDescriptions);
	}

	std::vector<vk::VertexInputAttributeDescription> Pipeline::VulkanObject::attributeDescriptions(const spk::GraphicalAPI::ShaderLayout::StorageLayout& p_storageConfiguration)
	{
		std::vector<vk::VertexInputAttributeDescription> attributeDescriptions;
		attributeDescriptions.reserve(p_storageConfiguration.fields.size());

		for (const auto& field : p_storageConfiguration.fields)
		{
			attributeDescriptions.push_back(vk::VertexInputAttributeDescription(
				field.location, ///< Location
				0, ///< Binding
				attributeFormat(field.attribute), ///< Format
				field.offset ///< Offset
			));
		}
		return (attributeDescriptions);
	}

	void Pipeline::VulkanObject::_createVertexBuffers()
	{
		_vertexCount = static_cast<uint32_t>(storage().size() / _configuration.storage.stride);
		assert(_vertexCount >= 3 && "Vertex count must be at least 3");

		vk::DeviceSize bufferSize = storage().size();
		uint32_t vertexSize = _configuration.storage.stride;

		Buffer stagingBuffer(
			*_linkedDevice,
			vertexSize,
			_vertexCount,
			vk::BufferUsageFlagBits::eTransferSrc,
			vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent
		);

		stagingBuffer.map();
		stagingBuffer.writeToBuffer(storage().data());

		_vertexBuffer = std::make_unique<Buffer>(
			*_linkedDevice,
			vertexSize,
			_vertexCount,
			vk::BufferUsageFlagBits::eTransferDst | vk::BufferUsageFlagBits::eVertexBuffer,
			vk::MemoryPropertyFlagBits::eDeviceLocal
		);

		_linkedDevice->copyBuffer(stagingBuffer.buffer(), _vertexBuffer->buffer(), bufferSize);
	}

	void Pipeline::VulkanObject::_createIndexBuffers()
	{
		_indexCount = static_cast<uint32_t>(indexes().nbIndexes());
		assert(_indexCount >= 3 && "Index count must be at least 3");

		vk::DeviceSize bufferSize = indexes().size();
		uint32_t indexSize = bufferSize / _indexCount;

		Buffer stagingBuffer(
			*_linkedDevice,
			indexSize,
			_indexCount,
			vk::BufferUsageFlagBits::eTransferSrc,
			vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent
		);

		stagingBuffer.map();
		stagingBuffer.writeToBuffer(indexes().data());

		_indexBuffer = std::make_unique<Buffer>(
			*_linkedDevice,
			indexSize,
			_indexCount,
			vk::BufferUsageFlagBits::eTransferDst | vk::BufferUsageFlagBits::eIndexBuffer,
			vk::MemoryPropertyFlagBits::eDeviceLocal
		);

		_linkedDevice->copyBuffer(stagingBuffer.buffer(), _indexBuffer->buffer(), bufferSize);
	}
}