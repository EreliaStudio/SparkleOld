#include "widget/spk_server_widget.hpp"

namespace spk
{

	bool ServerWidget::_onUpdate()
	{
		if (_serverToManage != nullptr)
			_serverToManage->treatMessages();
		return (false);
	}

	ServerWidget::ServerWidget(const std::wstring& p_name) :
		NoGraphicsWidget(p_name),
		_serverToManage(nullptr)
	{

	}

	void ServerWidget::setServer(Server* p_server)
	{
		_serverToManage = p_server;
	}

	Server* ServerWidget::server()
	{
		return (_serverToManage);
	}

	const Server* ServerWidget::server() const
	{
		return (_serverToManage);
	}
}