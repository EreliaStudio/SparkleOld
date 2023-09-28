#include "graphics/pipeline/spk_graphical_api_abstract_pipeline.hpp"

namespace spk::GraphicalAPI
{
	AbstractPipeline::Configuration::Data::Data()
	{

	}

	AbstractPipeline::Configuration::Data::Data(const size_t& p_size)
	{
		size = p_size;
	}
}