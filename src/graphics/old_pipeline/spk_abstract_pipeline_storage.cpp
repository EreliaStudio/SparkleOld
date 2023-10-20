#include "graphics/pipeline/spk_abstract_pipeline.hpp"

namespace spk
{
	AbstractPipeline::Object::Storage::Storage(const ShaderLayout::StorageLayout& p_configuration) :
		_configuration(p_configuration)
	{
		
	}

	void AbstractPipeline::Object::Storage::clear()
	{
		_content.clear();
	}
	
	const void* AbstractPipeline::Object::Storage::data() const
	{
		return (_content.data());
	}
	
	const size_t AbstractPipeline::Object::Storage::size() const
	{
		return (_content.size());
	}
}