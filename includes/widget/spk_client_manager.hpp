#pragma once

#include "network/spk_client.hpp"
#include "widget/spk_abstract_widget.hpp"

namespace spk
{
	class ClientManager : public AbstractWidget
	{
	private:
		Client* _clientToManage;

		void _onRender();
		void _onGeometryChange();
		bool _onUpdate();

	public:
		ClientManager(const std::wstring& p_name);

		void setClient(Client* p_client);

		Client* client();
		const Client* client() const;
	};
}