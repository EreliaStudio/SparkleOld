#include "graphics/pipeline/spk_abstract_pipeline.hpp"

namespace spk
{
    AbstractPipeline::Object::Indexes::Indexes()
    {
        _data.clear();
    }

    void AbstractPipeline::Object::Indexes::clear()
    {
        _data.clear();
    }

    AbstractPipeline::Object::Indexes& AbstractPipeline::Object::Indexes::operator << (const std::vector<size_t>& p_indexes)
    {
        for (const auto& index : p_indexes)
        {
            _data.push_back(index);
        }

        return *this;
    }

    const void* AbstractPipeline::Object::Indexes::data() const
    {
        return (_data.data());
    }

    size_t AbstractPipeline::Object::Indexes::size() const
    {
        return _data.size();
    }
}
