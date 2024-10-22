#include "network/spk_network_central_node.hpp"

namespace spk::Network
{
	CentralNode::CentralNode()
	{
		_server.setUnknowMessageReceptionCallback([&](const spk::Network::Server::EmiterID &p_emiterID, const spk::Network::Message &p_msg)
		{
			if (_messagesRedirection.contains(p_msg.header().type()) == false)
			{
				spk::throwException(L"Message type [" + std::to_wstring(p_msg.header().type()) + L"] isn't connected to any node");
			}
			p_msg.header().setEmiter(p_emiterID);
			_messagesRedirection[p_msg.header().type()]->send(p_msg);
		});
	}

	void CentralNode::start(size_t p_serverPort)
	{
		_server.start(p_serverPort);
	}

	void CentralNode::returnMessage(const spk::Network::Message &p_msg)
	{
		_server.sendTo(p_msg.header().emiterID(), p_msg);
	}

	void CentralNode::redirectMessage(spk::Network::Message::Type p_type, Node *p_destinationNode)
	{
		if (_messagesRedirection.contains(p_type) == true)
			spk::throwException(L"CentralNode has already a defined Node to message [" + std::to_wstring(p_type) + L"]");
		_messagesRedirection[p_type] = p_destinationNode;
		p_destinationNode->link(this);
	}

	void CentralNode::treatMessages()
	{
		_server.treatMessages();
	}
}