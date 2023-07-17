#include "widget/spk_client_manager.hpp"

namespace spk
{

	bool ClientManager::_onUpdate()
	{
		if (_clientToManage != nullptr)
			_clientToManage->treatMessages();
		return (false);
	}

	ClientManager::ClientManager(const std::wstring &p_name) :
		NoGraphicsWidget(p_name),
		_clientToManage(nullptr)
	{
	}

	void ClientManager::setClient(Client *p_client)
	{
		_clientToManage = p_client;
	}

	Client *ClientManager::client()
	{
		return (_clientToManage);
	}

	const Client *ClientManager::client() const
	{
		return (_clientToManage);
	}
}