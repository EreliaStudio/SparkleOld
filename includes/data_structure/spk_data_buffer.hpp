#pragma once 

#include <vector>
#include <cstdint>
#include <string>
#include <cstring>
#include <stdexcept>

namespace spk
{
	class DataBuffer
	{
	private:
		std::vector<uint8_t> _data;
		size_t _bookmark;

	public:
		DataBuffer();

		inline size_t size() const { return _data.size(); }
		inline size_t bookmark() const { return _bookmark; }
		inline size_t leftover() const { return size() - bookmark(); }
		inline bool empty() const { return leftover() == 0; }

		void skip(size_t number);
		void clear(void);
		void reset(void);

		template <typename InputType>
		void edit(size_t offset, const InputType &input)
		{
			static_assert(std::is_standard_layout<InputType>().value, "Unable to handle this type.");
			if (offset + sizeof(InputType) > size())
				throw std::runtime_error("Unable to edit, offset is out of bound.");
			memcpy(_data.data() + offset, &input, sizeof(InputType));
		}

		template <typename InputType>
		DataBuffer &operator<<(const InputType &input)
		{
			// TODO: thread safety
			static_assert(std::is_standard_layout<InputType>().value, "Unable to handle this type.");
			try
			{
				size_t previous_size(_data.size());

				_data.resize(_data.size() + sizeof(InputType));
				std::memcpy(_data.data() + previous_size, &input, sizeof(InputType));
			}
			catch (...)
			{
				throw;
			}
			return *this;
		}

		template <typename InputType>
		DataBuffer &operator>>(InputType &output)
		{
			static_assert(std::is_standard_layout<InputType>().value, "Unable to handle this type.");
			if (leftover() < sizeof(InputType))
				throw std::runtime_error("Unable to retrieve data buffer content.");
			std::memcpy(&output, _data.data() + bookmark(), sizeof(InputType));
			_bookmark += sizeof(InputType);
			return *this;
		}
	};
}
