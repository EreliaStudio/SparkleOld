#include "pipeline/pipeline.hpp"

AbstractPipeline::Configuration::Data::Data(const Type &p_type, const size_t &p_format, const size_t &p_size) : type(p_type),
																									 format(p_format),
																									 size(p_size)
{
}

AbstractPipeline::Configuration::Data::Data(const size_t &p_size) : type(Type::Structure),
								   format(1),
								   size(p_size)
{
}
