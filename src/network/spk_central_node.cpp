#include "network/spk_central_node.hpp"

namespace spk
{
	CentralNode::CentralNode()
	{
		_server.setUnknowMessageReceptionCallback([&](const spk::Server::EmiterID &p_emiterID, const spk::Message &p_msg)
		{
			spk::cout << "Receive message type [" << p_msg.header().id() << L"] from emiter [" << p_emiterID << L"]" << std::endl;
			if (_messagesRedirection.contains(p_msg.header().id()) == false)
			{
				spk::throwException(L"Message type [" + std::to_wstring(p_msg.header().id()) + L"] isn't connected to any node");
			}
			p_msg.header().setEmiter(p_emiterID);
			_messagesRedirection[p_msg.header().id()]->send(p_msg);
		});
	}

	void CentralNode::start(size_t p_serverPort)
	{
		_server.start(p_serverPort);
	}

	void CentralNode::returnMessage(const spk::Message &p_msg)
	{
		_server.sendTo(p_msg.header().emiterID(), p_msg);
	}

	void CentralNode::redirectMessage(spk::Message::Type p_type, Node *p_destinationNode)
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