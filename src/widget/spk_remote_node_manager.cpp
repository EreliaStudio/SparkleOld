#include "widget/spk_remote_node_manager.hpp"

namespace spk
{
	void RemoteNodeManager::_onRender()
	{

	}

	void RemoteNodeManager::_onGeometryChange()
	{
		
	}

	bool RemoteNodeManager::_onUpdate()
	{
		if (_remoteNodeToManage != nullptr)
			_remoteNodeToManage->treatMessages();
		return (false);
	}

	RemoteNodeManager::RemoteNodeManager(const std::wstring& p_name) :
		AbstractWidget(p_name),
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