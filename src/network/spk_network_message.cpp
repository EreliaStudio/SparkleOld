#include "network/spk_network_message.hpp"

namespace spk::Network
{
	const Message::Header::EmiterID& Message::Header::emiterID() const
	{
		return (_emiter);
	}

	void Message::Header::setEmiter(EmiterID p_emiter) const
	{
		_emiter = p_emiter;
	}

	void Message::Header::reset(uint32_t p_size)
	{
		_size = p_size;
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
    
	bool Message::empty() const
	{
		return (_content.empty());
	}

	void Message::resize(const size_t &p_newSize)
	{
		_content.resize(p_newSize);
	}

	void Message::skip(const size_t &p_number)
	{
		_content.skip(p_number);
	}

	void Message::clear()
	{
		_content.clear();
		_header._size = 0;
	}

	void Message::reset()
    {
		_content.reset();
    }
}