#pragma once

#include "network/spk_message.hpp"
#include "network/spk_node.hpp"
#include "network/spk_client.hpp"

namespace spk
{
	class RemoteNode : public spk::Node
	{
	private:
		spk::Client _client;
		CentralNode* _centralNode;

	public:
		RemoteNode();

		void connect(const std::wstring& p_serverAddress, const size_t& p_serverPort);

		void send(const spk::Message& p_msg);

		void treatMessages();
	};
}