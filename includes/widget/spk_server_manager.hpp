#pragma once

#include "network/spk_server.hpp"

namespace spk
{
	class ServerManager : public AbstractWidget
	{
	private:
		Server* _serverToManage;

		void _onRender()
		{

		}

		void _onGeometryChange()
		{

		}

		bool _onUpdate()
		{
			if (_serverToManage != nullptr)
				_serverToManage->treatMessages();
			return (false);
		}
	public:
		ServerManager(const std::wstring& p_name) :
			AbstractWidget(p_name),
			_serverToManage(nullptr)
		{

		}

		void setServer(Server* p_server)
		{
			_serverToManage = p_server;
		}

		Server* server()
		{
			return (_serverToManage);
		}

		const Server* server() const
		{
			return (_serverToManage);
		}
	};
}