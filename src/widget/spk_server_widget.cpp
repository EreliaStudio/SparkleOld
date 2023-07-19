#include "widget/spk_server_widget.hpp"

namespace spk::Widget
{

	bool Server::_onUpdate()
	{
		if (_serverToManage != nullptr)
			_serverToManage->treatMessages();
		return (false);
	}

	Server::Server(const std::wstring& p_name) :
		NoGraphics(p_name),
		_serverToManage(nullptr)
	{

	}

	void Server::setServer(spk::Server* p_server)
	{
		_serverToManage = p_server;
	}

	spk::Server* Server::server()
	{
		return (_serverToManage);
	}

	const spk::Server* Server::server() const
	{
		return (_serverToManage);
	}
}