#pragma once

#include <vector>
#include <cstdint>
#include <string>
#include <cstring>
#include <stdexcept>
#include <mutex>

#include "miscellaneous/spk_is_container.hpp"

namespace spk
{/**
	 * \class DataBuffer
	 * \brief Class for managing data buffers.
	 *
	 * This class is designed to provide convenient and efficient storage and retrieval of data.
	 * It supports various data types including standard and container types, provided they are
	 * standard layout types.
	 *
	 * The class provides a set of utility methods and operator overloads for reading from and
   
	 * writing to the buffer, as well as for managing the internal state of the buffer.
	 */
	class DataBuffer
	{
	private:
		std::vector<uint8_t> _data; ///< The buffer's data, stored as bytes.
		mutable size_t _bookmark; ///< Bookmark to keep track of the current position in the buffer.

	public:
		/**
		 * @brief Default constructor for the DataBuffer class.
		 */
		DataBuffer();

		/**
		 * @brief Constructor with a pre-defined sized data buffer
		 */
		DataBuffer(size_t p_dataSize);

		/**
		 * @brief Return the content of the DataBuffer as a raw bytes array
		 * 
		 * @return The content of the DataBuffer as raw bytes
		*/
		uint8_t* data()
		{
			return (_data.data());
		}

		/**
		 * @brief Return the content of the DataBuffer as a const raw bytes array
		 * 
		 * @return The content of the DataBuffer as const raw bytes
		*/
		const uint8_t* data() const
		{
			return (_data.data());
		}

		/**
		 * @brief Get the size of the buffer.
		 *
		 * @return The size of the buffer.
		 */
		inline size_t size() const { return _data.size(); }
		/**
		 * @brief Get the bookmark of the buffer.
		 *
		 * @return The position of the bookmark.
		 */
		inline size_t bookmark() const { return _bookmark; }

		/**
		 * @brief Get the size of the remaining buffer.
		 *
		 * @return The size from the bookmark to the end of the buffer.
		 */
		inline size_t leftover() const { return size() - bookmark(); }

		/**
		 * @brief Check whether the buffer is empty.
		 *
		 * @return True if there's no leftover data in the buffer, false otherwise.
		 */
		inline bool empty() const { return leftover() == 0; }

		/**
		 * @brief Resize the amount of data requested.
		 *
		 * @param p_newSize The number of bytes required inside the content.
		 */
		void resize(const size_t &p_newSize);

		/**
		 * @brief Move the bookmark ahead by the specified number.
		 *
		 * @param p_number The number of bytes to skip.
		 */
		void skip(const size_t &p_number);

		/**
		 * @brief Clear the buffer and reset the bookmark.
		 */
		void clear();

		/**
		 * @brief Reset the bookmark to the start of the buffer.
		 */
		void reset();

		/**
		 * @brief Retrieve data of a specific type from the buffer.
		 *
		 * @tparam OutputType The type of data to retrieve.
		 * @return The retrieved data.
		 */
		template <typename OutputType>
		OutputType get() const
		{
			OutputType result;

			*this >> result;

			return (result);
		}

		/**
		 * @brief Edit the buffer at a specific offset with input data.
		 *
		 * @tparam InputType The type of input data.
		 * @param p_offset Offset in the buffer where the data should be written.
		 * @param p_input The data to write into the buffer.
		 */
		template <typename InputType>
		void edit(const size_t& p_offset, const InputType& p_input)
		{
			static_assert(std::is_standard_layout<InputType>().value, "Unable to handle this type.");
			if (p_offset + sizeof(InputType) > size())
				throw std::runtime_error("Unable to edit, offset is out of bound.");
			memcpy(_data.data() + p_offset, &p_input, sizeof(InputType));
		}

		void edit(const size_t& p_offset, const void* p_data, const size_t& p_dataSize)
		{
			if (p_offset + p_dataSize > size())
				throw std::runtime_error("Unable to edit, offset is out of bound.");
			memcpy(_data.data() + p_offset, p_data, p_dataSize);
		}

		/**
		 * @brief Insert data into the buffer.
		 *
		 * This template function handles all non-container types.
		 *
		 * @tparam InputType The type of input data.
		 * @param p_input The data to insert into the buffer.
		 * @return Reference to the current DataBuffer object.
		 */
		template <typename InputType, typename std::enable_if_t<!spk::IsContainer<InputType>::value>* = nullptr>
		DataBuffer& operator<<(const InputType& p_input)
		{
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

		/**
		 * @brief Extract data from the buffer.
		 *
		 * This template function handles all non-container types.
		 *
		 * @tparam OutputType The type of data to extract.
		 * @param p_output Reference to a variable where the extracted data should be stored.
		 * @return Const reference to the current DataBuffer object.
		 */
		template <typename OutputType, typename std::enable_if_t<!spk::IsContainer<OutputType>::value>* = nullptr>
		const DataBuffer& operator>>(OutputType& p_output) const
		{
			static_assert(std::is_standard_layout<OutputType>().value, "Unable to handle this type.");
			if (leftover() < sizeof(OutputType))
				throw std::runtime_error("Unable to retrieve data buffer content.");
			std::memcpy(&p_output, _data.data() + bookmark(), sizeof(OutputType));
			_bookmark += sizeof(OutputType);
			return *this;
		}

		/**
		 * @brief Insert container data into the buffer.
		 *
		 * This template function handles all container types.
		 *
		 * @tparam InputType The type of input data (must be a container type).
		 * @param p_input The container data to insert into the buffer.
		 * @return Reference to the current DataBuffer object.
		 */
		template <typename InputType, typename std::enable_if_t<spk::IsContainer<InputType>::value>* = nullptr>
		DataBuffer& operator<<(const InputType& p_input)
		{
			*this << p_input.size();
			for (auto it = p_input.begin(); it != p_input.end(); ++it)
			{
				*this << *it;
			}
			return *this;
		}

		/**
		 * @brief Extract container data from the buffer.
		 *
		 * This template function handles all container types.
		 *
		 * @tparam OutputType The type of data to extract (must be a container type).
		 * @param p_output Reference to a variable where the extracted data should be stored.
		 * @return Const reference to the current DataBuffer object.
		 */
		template <typename OutputType, typename std::enable_if_t<spk::IsContainer<OutputType>::value>* = nullptr>
		const DataBuffer& operator>>(OutputType& p_output) const
		{
			p_output.resize(this->get<size_t>());
			for (auto it = p_output.begin(); it != p_output.end(); ++it)
			{
				*this >> *it;
			}
			return *this;
		}

		void append(const void* p_data, const size_t& p_dataSize)
		{
			size_t oldSize = size();
			resize(size() + p_dataSize);
			edit(oldSize, p_data, p_dataSize);
		}
	};
}
