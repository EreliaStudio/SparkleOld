#include "widget/spk_central_node_manager.hpp"

namespace spk
{
	void CentralNodeManager::_onRender()
	{
	}

	void CentralNodeManager::_onGeometryChange()
	{
	}

	bool CentralNodeManager::_onUpdate()
	{
		if (_centralNodeToManage != nullptr)
			_centralNodeToManage->treatMessages();
		return (false);
	}

	CentralNodeManager::CentralNodeManager(const std::wstring &p_name) : 
		AbstractWidget(p_name),
		_centralNodeToManage(nullptr)
	{
	}

	void CentralNodeManager::setCentralNode(CentralNode *p_client)
	{
		_centralNodeToManage = p_client;
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