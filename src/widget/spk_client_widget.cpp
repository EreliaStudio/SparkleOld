#include "widget/spk_client_widget.hpp"

namespace spk::Widget
{

	bool Client::_onUpdate()
	{
		if (_clientToManage != nullptr)
			_clientToManage->treatMessages();
		return (false);
	}

	Client::Client(const std::wstring &p_name) :
		NoGraphics(p_name),
		_clientToManage(nullptr)
	{
	}

	void Client::setClient(spk::Client *p_client)
	{
		_clientToManage = p_client;
	}

	spk::Client *Client::client()
	{
		return (_clientToManage);
	}

	const spk::Client *Client::client() const
	{
		return (_clientToManage);
	}
}