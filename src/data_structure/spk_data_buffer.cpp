#include "data_structure/spk_data_buffer.hpp"

namespace spk
{
	DataBuffer::DataBuffer() :
		_data(),
		_bookmark(0)
	{
	}
	
	void DataBuffer::resize(const size_t &p_newSize)
	{
		_data.resize(p_newSize);
	}

	void DataBuffer::skip(const size_t& p_number)
	{
		if (leftover() < p_number)
			throw std::runtime_error(std::string("Unable to skip ") + std::to_string(p_number) + " bytes.");
		_bookmark += p_number;
	}

	void DataBuffer::clear()
	{
		_data.clear();
		_bookmark = 0;
	}

	void DataBuffer::reset()
	{
		_bookmark = 0;
	}
}
