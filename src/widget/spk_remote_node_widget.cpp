#include "widget/spk_remote_node_widget.hpp"

namespace spk
{

	bool RemoteNodeManager::_onUpdate()
	{
		if (_remoteNodeToManage != nullptr)
			_remoteNodeToManage->treatMessages();
		return (false);
	}

	RemoteNodeManager::RemoteNodeManager(const std::wstring& p_name) :
		NoGraphicsWidget(p_name),
		_remoteNodeToManage(nullptr)
	{

	}

	void RemoteNodeManager::setRemoteNode(RemoteNode* p_client)
	{
		_remoteNodeToManage = p_client;
	}

	RemoteNode* RemoteNodeManager::remoteNode()
	{
		return (_remoteNodeToManage);
	}

	const RemoteNode* RemoteNodeManager::remoteNode() const
	{
		return (_remoteNodeToManage);
	}
}