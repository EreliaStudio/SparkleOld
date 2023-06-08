#include "data_structure/spk_data_buffer.hpp"

namespace spk
{
	DataBuffer::DataBuffer() : _data(),
					_bookmark(0)
	{
	
	}

	void DataBuffer::skip(size_t number)
	{
		if (leftover() < number)
			throw std::runtime_error(std::string("Unable to skip ") + std::to_string(number) + " bytes.");
		_bookmark += number;
	}

	void DataBuffer::clear(void)
	{
		_data.clear();
		_bookmark = 0;
	}

	void DataBuffer::reset(void)
	{
		_bookmark = 0;
	}
}
