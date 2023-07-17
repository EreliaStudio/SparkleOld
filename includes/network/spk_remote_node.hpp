#pragma once

#include "network/spk_message.hpp"
#include "network/spk_node.hpp"
#include "network/spk_client.hpp"

namespace spk
{
    /**
     * @class RemoteNode
     * @brief Class representing a remote network node.
     *
     * The RemoteNode class extends the Node class, providing additional functionality for managing a remote network connection.
     */
	class RemoteNode : public spk::Node
	{
	private:
		spk::Client _client; /**< The client used for network communication */
		CentralNode* _centralNode; /**< Pointer to the central node */

	public:
        /**
         * @brief Constructs a new RemoteNode.
         */
		RemoteNode();

        /**
         * @brief Establish a connection to a remote server.
         *
         * @param p_serverAddress Address of the server.
         * @param p_serverPort Port of the server.
         */
		void connect(const std::wstring& p_serverAddress, const size_t& p_serverPort);

        /**
         * @brief Sends a message to the remote node.
         *
         * @param p_msg Message to be sent.
         */
		void send(const spk::Message& p_msg);

        /**
         * @brief Process incoming messages from the remote node.
         */
		void treatMessages();
	};
}
