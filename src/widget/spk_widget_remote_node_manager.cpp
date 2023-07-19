#include "widget/spk_widget_remote_node_manager.hpp"

namespace spk::Widget
{
	bool RemoteNodeManager::_onUpdate()
	{
		if (_remoteNodeToManage != nullptr)
			_remoteNodeToManage->treatMessages();
		return (false);
	}

	RemoteNodeManager::RemoteNodeManager(const std::wstring& p_name) :
		NoGraphics(p_name),
		_remoteNodeToManage(nullptr)
	{

	}

	void RemoteNodeManager::setRemoteNode(spk::Network::RemoteNode* p_client)
	{
		_remoteNodeToManage = p_client;
	}

	spk::Network::RemoteNode* RemoteNodeManager::remoteNode()
	{
		return (_remoteNodeToManage);
	}

	const spk::Network::RemoteNode* RemoteNodeManager::remoteNode() const
	{
		return (_remoteNodeToManage);
	}
}