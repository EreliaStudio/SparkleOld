#include "widget/spk_central_node_manager.hpp"

namespace spk
{
	bool CentralNodeManager::_onUpdate()
	{
		if (_centralNodeToManage != nullptr)
			_centralNodeToManage->treatMessages();
		return (false);
	}

	CentralNodeManager::CentralNodeManager(const std::wstring &p_name) : 
		NoGraphicsWidget(p_name),
		_centralNodeToManage(nullptr)
	{
	}

	void CentralNodeManager::setCentralNode(CentralNode *p_centralNode)
	{
		_centralNodeToManage = p_centralNode;
	}

	CentralNode *CentralNodeManager::centralNode()
	{
		return (_centralNodeToManage);
	}

	const CentralNode *CentralNodeManager::centralNode() const
	{
		return (_centralNodeToManage);
	}
}