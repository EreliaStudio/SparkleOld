#include "network/spk_network_message.hpp"

namespace spk::Network
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

	Message::Message(Header::Type p_type) : _header(p_type)
	{

	}
	
	Message Message::createAwnser(Header::Type p_type) const
	{
		Message result;

		result._header = _header;
		result._header._size = 0;
		result._header.setType(p_type);

		return (result);
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