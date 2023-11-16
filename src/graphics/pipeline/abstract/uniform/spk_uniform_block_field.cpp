#include "graphics/pipeline/spk_abstract_pipeline.hpp"

namespace spk
{
	AbstractPipeline::UniformBlock::Field::Field() :
		_owner(nullptr),
		_name(L"Unnamed"),
		_data(nullptr),
		_offset(0),
		_size(0)
	{
	}

	AbstractPipeline::UniformBlock::Field::Field(const std::wstring& p_name, UniformBlock *p_owner, uint8_t *p_data, size_t p_offset, size_t p_size) :
		_name(p_name),
		_owner(p_owner),
		_data(p_data),
		_offset(p_offset),
		_size(p_size)
	{
	}
}