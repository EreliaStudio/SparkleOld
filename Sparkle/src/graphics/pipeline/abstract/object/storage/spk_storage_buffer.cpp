#include "graphics/pipeline/spk_abstract_pipeline.hpp"

namespace spk
{
	AbstractPipeline::Object::Storage::Buffer::Buffer(const std::wstring &p_name, size_t p_unitSize) :
		_unitSize(p_unitSize),
		_name(p_name),
		_needUpdate(false)
	{
		
	}

	void AbstractPipeline::Object::Storage::Buffer::clear()
	{
		_data.clear();
	}

	AbstractPipeline::Object::Storage::Buffer& AbstractPipeline::Object::Storage::Buffer::operator<<(wostream& (*func)(wostream&)) {
		_needUpdate = true;
		return *this;
	}

	void AbstractPipeline::Object::Storage::Buffer::setUpdateStatus(bool p_state)
	{
		_needUpdate = p_state;
	}

	bool AbstractPipeline::Object::Storage::Buffer::needUpdate() const
	{
		return (_needUpdate);
	}

	const uint8_t *AbstractPipeline::Object::Storage::Buffer::data() const
	{
		return (_data.data());
	}

	size_t AbstractPipeline::Object::Storage::Buffer::size() const
	{
		return (_data.size());
	}
}