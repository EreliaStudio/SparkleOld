#include "widget/spk_remote_node_widget.hpp"

namespace spk::Widget
{
	bool RemoteNode::_onUpdate()
	{
		if (_remoteNodeToManage != nullptr)
			_remoteNodeToManage->treatMessages();
		return (false);
	}

	RemoteNode::RemoteNode(const std::wstring& p_name) :
		NoGraphics(p_name),
		_remoteNodeToManage(nullptr)
	{

	}

	void RemoteNode::setRemoteNode(spk::RemoteNode* p_client)
	{
		_remoteNodeToManage = p_client;
	}

	spk::RemoteNode* RemoteNode::remoteNode()
	{
		return (_remoteNodeToManage);
	}

	const spk::RemoteNode* RemoteNode::remoteNode() const
	{
		return (_remoteNodeToManage);
	}
}