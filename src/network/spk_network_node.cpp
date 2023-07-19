#include "network/spk_network_node.hpp"
#include "network/spk_network_central_node.hpp"

namespace spk::Network
{
	Node::Node() : _centralNode(nullptr)
	{
	}

	void Node::link(CentralNode *p_centralNode)
	{
		_centralNode = p_centralNode;
	}

	void Node::receive(const spk::Network::Message &p_msg)
	{
		_centralNode->returnMessage(p_msg);
	}
}