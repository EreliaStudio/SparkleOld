#pragma once

#include "network/spk_client.hpp"

namespace spk
{
	class RemoteNodeManager : public AbstractWidget
	{
	private:
		RemoteNode* _remoteNodeToManage;

		void _onRender()
		{

		}

		void _onGeometryChange()
		{
			
		}

		bool _onUpdate()
		{
			if (_remoteNodeToManage != nullptr)
				_remoteNodeToManage->treatMessages();
			return (false);
		}

	public:
		RemoteNodeManager(const std::wstring& p_name) :
			AbstractWidget(p_name),
			_remoteNodeToManage(nullptr)
		{

		}

		void setRemoteNode(RemoteNode* p_client)
		{
			_remoteNodeToManage = p_client;
		}

		RemoteNode* remoteNode()
		{
			return (_remoteNodeToManage);
		}

		const RemoteNode* remoteNode() const
		{
			return (_remoteNodeToManage);
		}
	};
}