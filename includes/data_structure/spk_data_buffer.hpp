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
		mutable size_t _bookmark;

	template <typename StringType>
    DataBuffer& _serializeString(const StringType& str)
    {
        *this << str.size();
        for (size_t i = 0; i < str.size(); i++)
        {
            *this << str[i];
        }
        return *this;
    }

    template <typename StringType>
    const DataBuffer& _deserializeString(StringType& str) const
    {
        str.resize(this->get<size_t>());
        for (size_t i = 0; i < str.size(); i++)
        {
            *this >> str[i];
        }
        return *this;
    }

	public:
		DataBuffer();

		inline size_t size() const { return _data.size(); }
		inline size_t bookmark() const { return _bookmark; }
		inline size_t leftover() const { return size() - bookmark(); }
		inline bool empty() const { return leftover() == 0; }

		void skip(const size_t& p_number);
		void clear();
		void reset();

		template <typename OutputType>
		OutputType get() const
		{
			OutputType result;

			*this >> result;

			return (result);
		}

		template <typename InputType>
		void edit(const size_t& p_offset, const InputType& p_input)
		{
			static_assert(std::is_standard_layout<InputType>().value, "Unable to handle this type.");
			if (p_offset + sizeof(InputType) > size())
				throw std::runtime_error("Unable to edit, offset is out of bound.");
			memcpy(_data.data() + p_offset, &p_input, sizeof(InputType));
		}

		template <typename InputType>
		DataBuffer &operator<<(const InputType& p_input)
		{
			// TODO: thread safety
			static_assert(std::is_standard_layout<InputType>().value, "Unable to handle this type.");
			try
			{
				size_t previous_size(_data.size());

				_data.resize(_data.size() + sizeof(InputType));
				std::memcpy(_data.data() + previous_size, &p_input, sizeof(InputType));
			}
			catch (...)
			{
				throw;
			}
			return *this;
		}

		template <typename OutputType>
		const DataBuffer &operator>>(OutputType& p_output) const
		{
			static_assert(std::is_standard_layout<OutputType>().value, "Unable to handle this type.");
			if (leftover() < sizeof(OutputType))
				throw std::runtime_error("Unable to retrieve data buffer content.");
			std::memcpy(&p_output, _data.data() + bookmark(), sizeof(OutputType));
			_bookmark += sizeof(OutputType);
			return *this;
		}

		template<>
		DataBuffer &operator<< <std::string>(const std::string& p_input)
		{
			return _serializeString(p_input);
		}

		template<>
		const DataBuffer &operator>> <std::string>(std::string& p_output) const
		{
			return _deserializeString(p_output);
		}

		template<>
		DataBuffer &operator<< <std::wstring>(const std::wstring& p_input)
		{
			return _serializeString(p_input);
		}

		template<>
		const DataBuffer &operator>> <std::wstring>(std::wstring& p_output) const
		{
			return _deserializeString(p_output);
		}
	};
}
