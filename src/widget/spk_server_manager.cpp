#include "widget/spk_server_manager.hpp"

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

	void ServerManager::setServer(Server* p_server)
	{
		_serverToManage = p_server;
	}

	Server* ServerManager::server()
	{
		return (_serverToManage);
	}

	const Server* ServerManager::server() const
	{
		return (_serverToManage);
	}
}