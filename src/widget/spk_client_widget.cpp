#include "widget/spk_client_manager.hpp"

namespace spk
{

	bool ClientWidget::_onUpdate()
	{
		if (_clientToManage != nullptr)
			_clientToManage->treatMessages();
		return (false);
	}

	ClientWidget::ClientWidget(const std::wstring &p_name) :
		NoGraphicsWidget(p_name),
		_clientToManage(nullptr)
	{
	}

	void ClientWidget::setClient(Client *p_client)
	{
		_clientToManage = p_client;
	}

	Client *ClientWidget::client()
	{
		return (_clientToManage);
	}

	const Client *ClientWidget::client() const
	{
		return (_clientToManage);
	}
}