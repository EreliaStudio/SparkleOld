#pragma once

#include "network/spk_client.hpp"

namespace spk
{
	class CentralNodeManager : public AbstractWidget
	{
	private:
		CentralNode* _centralNodeToManage;

		void _onRender()
		{

		}

		void _onGeometryChange()
		{
			
		}

		bool _onUpdate()
		{
			if (_centralNodeToManage != nullptr)
				_centralNodeToManage->treatMessages();
			return (false);
		}

	public:
		CentralNodeManager(const std::wstring& p_name) :
			AbstractWidget(p_name),
			_centralNodeToManage(nullptr)
		{

		}

		void setCentralNode(CentralNode* p_client)
		{
			_centralNodeToManage = p_client;
		}

		CentralNode* centralNode()
		{
			return (_centralNodeToManage);
		}

		const CentralNode* centralNode() const
		{
			return (_centralNodeToManage);
		}
	};
}