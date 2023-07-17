#include "network/spk_node.hpp"
#include "network/spk_central_node.hpp"

namespace spk
{
	Node::Node() : _centralNode(nullptr)
	{
	}

	void Node::link(CentralNode *p_centralNode)
	{
		_centralNode = p_centralNode;
	}

	void Node::receive(const spk::Message &p_msg)
	{
		_centralNode->returnMessage(p_msg);
	}
}