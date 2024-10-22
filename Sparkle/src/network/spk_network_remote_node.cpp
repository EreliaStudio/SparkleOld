#include "network/spk_network_remote_node.hpp"

namespace spk::Network
{
	RemoteNode::RemoteNode()
	{
		_client.setUnknowMessageReceptionCallback([&](const spk::Network::Message &p_msg)
												  { receive(p_msg); });
	}

	void RemoteNode::connect(const std::wstring &p_serverAddress, const size_t &p_serverPort)
	{
		_client.connect(p_serverAddress, p_serverPort);
	}

	void RemoteNode::send(const spk::Network::Message &p_msg)
	{
		_client.send(p_msg);
	}

	void RemoteNode::treatMessages()
	{
		_client.treatMessages();
	}
}