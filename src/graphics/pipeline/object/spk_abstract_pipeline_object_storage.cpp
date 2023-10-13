#include "graphics/pipeline/spk_abstract_pipeline.hpp"

namespace spk
{
	AbstractPipeline::Object::Storage::Storage(const spk::ShaderLayout::StorageBufferLayout& p_layout) :
		_data(),
		_stride(p_layout.stride())
	{

	}

	void AbstractPipeline::Object::Storage::clear()
	{
		_data.clear();
	}

	const void* AbstractPipeline::Object::Storage::data() const
	{
		return _data.data();
	}

	size_t AbstractPipeline::Object::Storage::size() const
	{
		return _data.size();
	}
}
