#include "widget/spk_central_node_widget.hpp"

namespace spk
{
	bool CentralNodeWidget::_onUpdate()
	{
		if (_centralNodeToManage != nullptr)
			_centralNodeToManage->treatMessages();
		return (false);
	}

	CentralNodeWidget::CentralNodeWidget(const std::wstring &p_name) : 
		NoGraphicsWidget(p_name),
		_centralNodeToManage(nullptr)
	{
	}

	void CentralNodeWidget::setCentralNode(CentralNode *p_centralNode)
	{
		_centralNodeToManage = p_centralNode;
	}

	CentralNode *CentralNodeWidget::centralNode()
	{
		return (_centralNodeToManage);
	}

	const CentralNode *CentralNodeWidget::centralNode() const
	{
		return (_centralNodeToManage);
	}
}