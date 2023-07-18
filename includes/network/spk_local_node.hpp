#pragma once

#include <map>
#include <functional>
#include "network/spk_message.hpp"
#include "network/spk_node.hpp"
#include "spk_basic_functions.hpp"

namespace spk
{
    /**
     * @class LocalNode
     * @brief The LocalNode class extends the Node class, encapsulating the behavior of a local node in a network.
     *
     * The LocalNode class provides support for receiving and sending messages. It also allows setting callbacks 
     * for specific message types and handling of unknown message types.
     */
	class LocalNode : public spk::Node
	{
	private:
		std::map<spk::Message::Type, std::function<void(const spk::Message&)>> _onMessageReceptionCallbacks; /**< Map storing callbacks for specific types of messages */
		std::function<void(const spk::Message&)> _onUnknowMessageReception = [&](const spk::Message& p_msg){
			spk::throwException(L"Message [" + std::to_wstring(p_msg.header().id()) + L"] not defined in local node");
		}; /**< Callback function for handling unknown message types */

	public:
        /**
         * @brief Constructor for LocalNode.
         */
		LocalNode();

        /**
         * @brief Sets a callback for a specific message type.
         *
         * @param p_id The ID of the message type.
         * @param p_funct The callback function.
         */
		void setOnMessageReceptionCallback(const spk::Message::Type& p_id, std::function<void(const spk::Message&)> p_funct);

        /**
         * @brief Sets a callback for unknown message types.
         *
         * @param p_funct The callback function.
         */
		void setUnknowMessageReceptionCallback(std::function<void(const spk::Message&)> p_funct);

        /**
         * @brief Sends a message to the network.
         *
         * @param p_msg The message to send.
         */
		void send(const spk::Message& p_msg);
	};
}
