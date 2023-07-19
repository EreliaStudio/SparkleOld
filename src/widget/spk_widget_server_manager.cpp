#include "widget/spk_widget_server_manager.hpp"

namespace spk::Widget
{

	bool ServerManager::_onUpdate()
	{
		if (_serverToManage != nullptr)
			_serverToManage->treatMessages();
		return (false);
	}

	ServerManager::ServerManager(const std::wstring& p_name) :
		NoGraphics(p_name),
		_serverToManage(nullptr)
	{

	}

	void ServerManager::setServer(spk::Server* p_server)
	{
		_serverToManage = p_server;
	}

	spk::Server* ServerManager::server()
	{
		return (_serverToManage);
	}

	const spk::Server* ServerManager::server() const
	{
		return (_serverToManage);
	}
}