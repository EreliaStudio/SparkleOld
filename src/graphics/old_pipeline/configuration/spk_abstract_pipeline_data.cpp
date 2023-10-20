#include "graphics/pipeline/spk_abstract_pipeline.hpp"

namespace spk
{
	ShaderLayout::Data::Data() :
		type(Type::Error),
		size(0),
		format(0)
	{

	}
	
	ShaderLayout::Data::Data(const size_t& p_format, const Type& p_type) :
		type(p_type),
		format(p_format)
	{
		switch (type)
		{
			case Type::Int:
			{
				size = sizeof(int);
				break;
			}
			case Type::UInt:
			{
				size = sizeof(unsigned int);
				break;
			}
			case Type::Float:
			{
				size = sizeof(float);
				break;
			}
			default:
				spk::throwException(L"Invalid Data constructor called");
		}
	}

	ShaderLayout::Data::Data(const size_t& p_size) :
		type(Type::Complex),
		size(p_size),
		format(1)
	{

	}
}