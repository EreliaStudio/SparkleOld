#include "graphics/pipeline/spk_graphical_api_abstract_pipeline.hpp"

namespace spk::GraphicalAPI
{
	AbstractPipeline::Object::Indexes::Indexes()
	{

	}

	void AbstractPipeline::Object::Indexes::clear()
	{
		_content.clear();
	}

	AbstractPipeline::Object::Indexes &AbstractPipeline::Object::Indexes::operator<<(const Index &p_index)
	{
		_content.push_back(p_index);
		return *this;
	}

	AbstractPipeline::Object::Indexes &AbstractPipeline::Object::Indexes::operator<<(const std::vector<Index> &p_indexes)
	{
		_content.insert(_content.end(), p_indexes.begin(), p_indexes.end());
		return *this;
	}

	void AbstractPipeline::Object::Indexes::insert(const Index* p_indexes, size_t p_size)
	{
		_content.insert(_content.end(), p_indexes, p_indexes + p_size);
	}

	const void* AbstractPipeline::Object::Indexes::data() const
	{
		return (_content.data());
	}

	const size_t AbstractPipeline::Object::Indexes::nbIndexes() const
	{
		return (_content.size());
	}
	
	const size_t AbstractPipeline::Object::Indexes::size() const
	{
		return (_content.size() * sizeof(unsigned int));
	}
}