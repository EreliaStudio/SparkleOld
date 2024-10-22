#include "widget/spk_widget_client_manager.hpp"

namespace spk::Widget
{

	bool ClientManager::_onUpdate()
	{
		if (_clientToManage != nullptr)
			_clientToManage->treatMessages();
		return (false);
	}

	ClientManager::ClientManager(const std::wstring &p_name) :
		NoGraphics(p_name),
		_clientToManage(nullptr)
	{
	}

	void ClientManager::setClient(spk::Network::Client *p_client)
	{
		_clientToManage = p_client;
	}

	spk::Network::Client *ClientManager::client()
	{
		return (_clientToManage);
	}

	const spk::Network::Client *ClientManager::client() const
	{
		return (_clientToManage);
	}
}