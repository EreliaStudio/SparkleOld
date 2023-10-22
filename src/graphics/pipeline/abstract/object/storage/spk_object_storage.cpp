#include "graphics/pipeline/spk_abstract_pipeline.hpp"

namespace spk
{
	AbstractPipeline::Object::Storage::Storage(const spk::ShaderLayout::StorageBufferLayout& p_storageBufferLayout) :
		_vertices(L"Vertices", p_storageBufferLayout.stride()),
		_indexes(L"Indexes", sizeof(uint32_t))
	{

	}

	AbstractPipeline::Object::Storage::Buffer &AbstractPipeline::Object::Storage::vertices()
	{
		return (_vertices);
	}

	AbstractPipeline::Object::Storage::Buffer &AbstractPipeline::Object::Storage::indexes()
	{
		return (_indexes);
	}
}