#pragma once

#include <map>
#include "network/spk_message.hpp"
#include "network/spk_node.hpp"
#include "network/spk_server.hpp"

namespace spk
{
	class CentralNode
	{
	private:
		spk::Server _server;
		std::map<spk::Message::Type, Node*> _messagesRedirection;

	public:
		CentralNode();

		void start(size_t p_serverPort);

		void returnMessage(const spk::Message& p_msg);

		void redirectMessage(spk::Message::Type p_type, Node* p_destinationNode);

		void treatMessages();
	};
}