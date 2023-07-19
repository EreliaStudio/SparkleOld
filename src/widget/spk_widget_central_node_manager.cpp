#include "widget/spk_widget_central_node_manager.hpp"

namespace spk::Widget
{
	bool CentralNodeManager::_onUpdate()
	{
		if (_centralNodeToManage != nullptr)
			_centralNodeToManage->treatMessages();
		return (false);
	}

	CentralNodeManager::CentralNodeManager(const std::wstring &p_name) : 
		NoGraphics(p_name),
		_centralNodeToManage(nullptr)
	{
	}

	void CentralNodeManager::setCentralNode(spk::CentralNode *p_centralNode)
	{
		_centralNodeToManage = p_centralNode;
	}

	spk::CentralNode *CentralNodeManager::centralNode()
	{
		return (_centralNodeToManage);
	}

	const spk::CentralNode *CentralNodeManager::centralNode() const
	{
		return (_centralNodeToManage);
	}
}