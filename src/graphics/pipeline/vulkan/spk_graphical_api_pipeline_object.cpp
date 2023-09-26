#include "graphics/pipeline/vulkan/spk_graphical_api_pipeline.hpp"
#include "graphics/pipeline/vulkan/spk_graphical_api_pipeline_basic_functions.hpp"

namespace spk::GraphicalAPI
{
	Pipeline::VulkanObject::VulkanObject(spk::GraphicalAPI::AbstractPipeline* p_owner,
		const spk::GraphicalAPI::AbstractPipeline::Object::Storage::Configuration& p_storageConfiguration)
		: spk::GraphicalAPI::AbstractPipeline::Object(p_owner, p_storageConfiguration)
	{
	}

	void Pipeline::VulkanObject::push()
	{

	}

	void Pipeline::VulkanObject::activate()
	{

	}
	void Pipeline::VulkanObject::deactivate()
	{

	}

	std::vector<vk::VertexInputBindingDescription> Pipeline::VulkanObject::bindingDescriptions(const spk::GraphicalAPI::AbstractPipeline::Object::Storage::Configuration& p_storageConfiguration)
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

	std::vector<vk::VertexInputAttributeDescription> Pipeline::VulkanObject::attributeDescriptions(const spk::GraphicalAPI::AbstractPipeline::Object::Storage::Configuration& p_storageConfiguration)
	{
		std::vector<vk::VertexInputAttributeDescription> attributeDescriptions;
		attributeDescriptions.reserve(p_storageConfiguration.attributes.size());

		for (const auto &attribute: p_storageConfiguration.attributes)
		{
			attributeDescriptions.push_back(vk::VertexInputAttributeDescription(
				attribute.second.location, ///< Location
				0, ///< Binding
				attributeFormat(attribute.second), ///< Format
				attribute.second.offset ///< Offset
			));
		}
		return (attributeDescriptions);
	}
}