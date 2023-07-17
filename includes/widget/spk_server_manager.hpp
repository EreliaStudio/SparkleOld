#pragma once

#include "network/spk_server.hpp"
#include "widget/spk_abstract_widget.hpp"

namespace spk
{
	class ServerManager : public AbstractWidget
	{
	private:
		Server* _serverToManage;

		void _onRender();
		void _onGeometryChange();
		bool _onUpdate();
	public:
		ServerManager(const std::wstring& p_name);

		void setServer(Server* p_server);

		Server* server();
		const Server* server() const;
	};
}