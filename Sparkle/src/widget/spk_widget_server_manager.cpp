#include "widget/spk_widget_server_manager.hpp"

namespace spk
{

	bool ServerManager::_onUpdate()
	{
		if (_serverToManage != nullptr)
			_serverToManage->treatMessages();
		return (false);
	}

	ServerManager::ServerManager(const std::wstring& p_name) :
		NoGraphicsWidget(p_name),
		_serverToManage(nullptr)
	{

	}

	void ServerManager::setServer(spk::Network::Server* p_server)
	{
		_serverToManage = p_server;
	}

	spk::Network::Server* ServerManager::server()
	{
		return (_serverToManage);
	}

	const spk::Network::Server* ServerManager::server() const
	{
		return (_serverToManage);
	}
}