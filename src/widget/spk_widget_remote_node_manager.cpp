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

	void RemoteNodeManager::setRemoteNode(spk::RemoteNode* p_client)
	{
		_remoteNodeToManage = p_client;
	}

	spk::RemoteNode* RemoteNodeManager::remoteNode()
	{
		return (_remoteNodeToManage);
	}

	const spk::RemoteNode* RemoteNodeManager::remoteNode() const
	{
		return (_remoteNodeToManage);
	}
}