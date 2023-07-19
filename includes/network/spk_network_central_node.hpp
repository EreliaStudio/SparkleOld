#pragma once

#include <map>
#include "network/spk_network_message.hpp"
#include "network/spk_network_node.hpp"
#include "network/spk_network_server.hpp"

namespace spk::Network
{
    /**
     * @class CentralNode
     * @brief The CentralNode class encapsulates the behavior of a central communication node.
     *
     * The CentralNode class is responsible for handling and redirecting incoming messages to the appropriate nodes, as well as starting and managing a server for communication.
     */
	class CentralNode
	{
	private:
		spk::Network::Server _server; /**< Server object for network communication */
		std::map<spk::Network::Message::Type, Node*> _messagesRedirection; /**< Map to store the mapping of message types to corresponding nodes */

	public:
        /**
         * @brief Constructor for CentralNode.
         *
         * The constructor initializes a CentralNode instance and sets up message handling.
         */
		CentralNode();

        /**
         * @brief Start the server with the given port.
         *
         * @param p_serverPort The port number for the server to use for communication.
         */
		void start(size_t p_serverPort);

        /**
         * @brief Send a response message back to the message emitter.
         *
         * @param p_msg The message to send back.
         */
		void returnMessage(const spk::Network::Message& p_msg);

        /**
         * @brief Setup the redirection of a specific message type to a node.
         *
         * @param p_type The type of message to redirect.
         * @param p_destinationNode The node to redirect the message to.
         */
		void redirectMessage(spk::Network::Message::Type p_type, Node* p_destinationNode);

        /**
         * @brief Process all pending messages on the server.
         */
		void treatMessages();
	};
}
