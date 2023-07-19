#include "widget/spk_central_node_widget.hpp"

namespace spk::Widget
{
	bool CentralNode::_onUpdate()
	{
		if (_centralNodeToManage != nullptr)
			_centralNodeToManage->treatMessages();
		return (false);
	}

	CentralNode::CentralNode(const std::wstring &p_name) : 
		NoGraphics(p_name),
		_centralNodeToManage(nullptr)
	{
	}

	void CentralNode::setCentralNode(spk::CentralNode *p_centralNode)
	{
		_centralNodeToManage = p_centralNode;
	}

	spk::CentralNode *CentralNode::centralNode()
	{
		return (_centralNodeToManage);
	}

	const spk::CentralNode *CentralNode::centralNode() const
	{
		return (_centralNodeToManage);
	}
}