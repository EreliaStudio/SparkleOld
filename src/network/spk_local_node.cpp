#include "network/spk_local_node.hpp"

namespace spk
{
	LocalNode::LocalNode()
	{
	}

	void LocalNode::send(const spk::Message &p_msg)
	{
		_onMessageReceptionCallbacks[p_msg.header().id()](p_msg);
	}
}