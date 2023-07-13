#pragma once

#include <stdint.h>
#include "data_structure/spk_data_buffer.hpp"

namespace spk
{
	class Message
	{
		friend class Socket;
	public:
		class Header
		{
		public:
			using ID = int32_t;
			friend class Message;

		private:
			ID _id;
			uint32_t _size;

		public:
			Header(ID p_id = 0):
				_id(p_id),
				_size(0)
			{

			}

			const ID& id() const
			{
				return (_id);
			}

			void setID(ID p_id)
			{
				_id = p_id;
			}

			void reset(uint32_t p_size)
			{
				_size = p_size;
			}
		};
		using ID = Header::ID;

	private:
		Header _header;
		DataBuffer _content;
	
	public:
		Message(int32_t p_id = 0) : 
			_header(p_id)
		{

		}

		Header& header()
		{
			return (_header);
		}

		const Header& header() const
		{
			return (_header);
		}

		uint8_t* data()
		{
			return (_content.data());
		}

		const uint8_t* data() const
		{
			return (_content.data());
		}

		size_t size() const
		{
			return (_header._size);
		}

		template <typename InputType>
		Message& operator<<(const InputType& p_input)
		{
			_content << p_input;
			_header._size = _content.size();
		}

		template <typename OutputType>
		const Message& operator>>(OutputType& p_output) const
		{
			_content >> p_output;
		}
	};
}