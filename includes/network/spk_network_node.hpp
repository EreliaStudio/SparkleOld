#pragma once

#include "network/spk_network_message.hpp"

namespace spk::Network
{
	class CentralNode;

    /**
     * @class Node
     * @brief Class encapsulating a node, remote or local.
     *
     * The Node class provides a generic abstraction for a network node that can send and receive messages.
     */
	class Node
	{
	private:
		spk::Network::CentralNode* _centralNode; /**< Pointer to the central node */

	public:
        /**
         * @brief Constructs a new Node.
         */
		Node();

        /**
         * @brief Links the node with a CentralNode.
         *
         * @param p_centralNode Pointer to the CentralNode to link with.
         */
		void link(spk::Network::CentralNode* p_centralNode);

        /**
         * @brief Sends a message. Must be implemented by derived classes.
         *
         * @param p_msg Message to send.
         */
		virtual void send(const spk::Network::Message& p_msg) = 0;

        /**
         * @brief Receives a message. Can be overridden by derived classes.
         *
         * @param p_msg Message to receive.
         */
		virtual void receive(const spk::Network::Message& p_msg);
	};
}
