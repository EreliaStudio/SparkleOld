#pragma once

#include "network/spk_central_node.hpp"
#include "widget/spk_abstract_widget.hpp"

namespace spk
{
	class CentralNodeManager : public AbstractWidget
	{
	private:
		CentralNode* _centralNodeToManage;

		void _onRender();
		void _onGeometryChange();
		bool _onUpdate();

	public:
		CentralNodeManager(const std::wstring& p_name);

		void setCentralNode(CentralNode* p_client);

		CentralNode* centralNode();
		const CentralNode* centralNode() const;
	};
}