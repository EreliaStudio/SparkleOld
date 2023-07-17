#pragma once

#include "network/spk_remote_node.hpp"
#include "widget/spk_abstract_widget.hpp"

namespace spk
{
	class RemoteNodeManager : public AbstractWidget
	{
	private:
		RemoteNode* _remoteNodeToManage;

		void _onRender();
		void _onGeometryChange();
		bool _onUpdate();

	public:
		RemoteNodeManager(const std::wstring& p_name);

		void setRemoteNode(RemoteNode* p_client);

		RemoteNode* remoteNode();
		const RemoteNode* remoteNode() const;
	};
}