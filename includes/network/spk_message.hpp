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
			using Type = int32_t;
			using EmiterID = long long;
			friend class Message;

		private:
			Type _id;
			uint32_t _size;
			mutable EmiterID _emiter;

		public:
			Header(Type p_id = 0);

			const EmiterID& emiterID() const;
			const Type& id() const;
			void setType(Type p_id);
			void setEmiter(EmiterID p_emiterID) const;
			void reset(uint32_t p_size);
		};
		using Type = Header::Type;

	private:
		Header _header;
		DataBuffer _content;
	
	public:
		Message(int32_t p_id = 0);

		Header& header();
		const Header& header() const;

		uint8_t* data();
		const uint8_t* data() const;

		size_t size() const;

		template <typename InputType>
		Message& operator<<(const InputType& p_input)
		{
			_content << p_input;
			_header._size = _content.size();
			return *this;
		}

		template <typename OutputType>
		const Message& operator>>(OutputType& p_output) const
		{
			_content >> p_output;
			return *this;
		}
	};
}