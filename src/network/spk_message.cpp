#include "network/spk_message.hpp"

namespace spk
{
	Message::Header::Header(Type p_id) : _id(p_id),
											 _size(0)
	{
	}

	const Message::Type &Message::Header::id() const
	{
		return (_id);
	}

	const Message::Header::EmiterID& Message::Header::emiterID() const
	{
		return (_emiter);
	}

	void Message::Header::setType(Type p_id)
	{
		_id = p_id;
	}

	void Message::Header::setEmiter(EmiterID p_emiter) const
	{
		_emiter = p_emiter;
	}

	void Message::Header::reset(uint32_t p_size)
	{
		_size = p_size;
	}

	Message::Message(int32_t p_id) : _header(p_id)
	{

	}

	Message::Header &Message::header()
	{
		return (_header);
	}

	const Message::Header &Message::header() const
	{
		return (_header);
	}

	uint8_t *Message::data()
	{
		return (_content.data());
	}

	const uint8_t *Message::data() const
	{
		return (_content.data());
	}

	size_t Message::size() const
	{
		return (_header._size);
	}
}