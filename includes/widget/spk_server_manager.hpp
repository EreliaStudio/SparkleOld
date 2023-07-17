#pragma once

#include "network/spk_server.hpp"
#include "widget/spk_abstract_widget.hpp"

namespace spk
{
	class ServerManager : public NoGraphicsWidget
	{
	private:
		Server* _serverToManage;

		bool _onUpdate();
	public:
		ServerManager(const std::wstring& p_name);

		void setServer(Server* p_server);

		Server* server();
		const Server* server() const;
	};
}