#include "data_structure/spk_data_buffer.hpp"

namespace spk
{
	DataBuffer::DataBuffer() :
		_data(),
		_bookmark(0)
	{
	}

	void DataBuffer::skip(const size_t& p_number)
	{
		std::lock_guard<std::recursive_mutex> lock(_accessMutex);

		if (leftover() < p_number)
			throw std::runtime_error(std::string("Unable to skip ") + std::to_string(p_number) + " bytes.");
		_bookmark += p_number;
	}

	void DataBuffer::clear()
	{
		std::lock_guard<std::recursive_mutex> lock(_accessMutex);
		
		_data.clear();
		_bookmark = 0;
	}

	void DataBuffer::reset()
	{
		std::lock_guard<std::recursive_mutex> lock(_accessMutex);
		
		_bookmark = 0;
	}
}
