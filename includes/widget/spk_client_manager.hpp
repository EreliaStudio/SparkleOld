#pragma once

#include "network/spk_client.hpp"

namespace spk
{
	class ClientManager : public AbstractWidget
	{
	private:
		Client* _clientToManage;

		void _onRender()
		{

		}

		void _onGeometryChange()
		{
			
		}

		bool _onUpdate()
		{
			if (_clientToManage != nullptr)
				_clientToManage->treatMessages();
			return (false);
		}

	public:
		ClientManager(const std::wstring& p_name) :
			AbstractWidget(p_name),
			_clientToManage(nullptr)
		{

		}

		void setClient(Client* p_client)
		{
			_clientToManage = p_client;
		}

		Client* client()
		{
			return (_clientToManage);
		}

		const Client* client() const
		{
			return (_clientToManage);
		}
	};
}