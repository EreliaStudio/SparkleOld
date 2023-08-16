#pragma once

#include <map>
#include "network/spk_network_message.hpp"
#include "network/spk_network_acceptor.hpp"
#include "network/spk_network_socket.hpp"
#include "threading/spk_persistent_worker.hpp"
#include "threading/spk_thread_safe_queue.hpp"
#include "spk_basic_functions.hpp"

namespace spk::Network
{
    /**
     * @class Client
     * @brief The Client class encapsulates the behavior of a client in a network communication context.
     *
     * The Client class handles connecting to a server, sending messages, receiving messages, and processing received messages. 
     * It also supports setting callbacks for specific types of messages.
     */
	class Client
	{
	public:
		spk::PersistentWorker _socketContextWorker; /**< A worker object that handles tasks related to the socket context */
		spk::ContractProvider::Contract _readingSocketDataContract; /**< A contract object used for reading data from the socket */

		spk::Network::Socket _socket; /**< The socket object used for network communication */
		spk::ThreadSafeQueue<spk::Network::Message> _messagesToTreat; /**< A thread-safe queue that stores messages to be processed */

		std::map<spk::Network::Message::Type, std::function<void(const spk::Network::Message&)>> _onMessageReceptionCallbacks; /**< Map storing callbacks for specific types of messages */
		std::function<void(const spk::Network::Message&)> _onUnknowMessageReception = [&](const spk::Network::Message& p_msg){
			spk::cout << L"Unknow message ID [" << p_msg.header().type() << L"] received" << std::endl;
		}; /**< Callback function for handling unknown message types */

		void _treatMessage(const spk::Network::Message& p_msg); /**< Internal function to handle a specific message */

	public:
        /**
         * @brief Constructor for Client.
         */
		Client();

        /**
         * @brief Destructor for Client.
         */
		~Client();

        /**
         * @brief Connects to a server at the specified address and port.
         *
         * @param p_serverAddress The address of the server to connect to.
         * @param p_serverPort The port of the server to connect to.
         */
		void connect(const std::wstring& p_serverAddress, const size_t& p_serverPort);

        /**
         * @brief Disconnects from the server.
         */
		void disconnect();

        /**
         * @brief Processes all pending messages.
         */
		void treatMessages();

        /**
         * @brief Sets a callback for a specific message type.
         *
         * @param p_messageType The type of the message type.
         * @param p_funct The callback function.
         */
        template <typename TMessageType>
		void setOnMessageReceptionCallback(const TMessageType& p_messageType, std::function<void(const spk::Network::Message&)> p_funct)
        {
            Message::Type messageType = static_cast<Message::Type>(p_messageType);
            if (_onMessageReceptionCallbacks.contains(messageType) == true)
            {
                spk::throwException(L"Callback already define for message type [" + std::to_wstring(messageType) + L"]");
            }
            _onMessageReceptionCallbacks[messageType] = std::bind(p_funct, std::placeholders::_1);
        }

        /**
         * @brief Sets a callback for unknown message types.
         *
         * @param p_funct The callback function.
         */
		void setUnknowMessageReceptionCallback(std::function<void(const spk::Network::Message&)> p_funct);

        /**
         * @brief Sends a message to the server.
         *
         * @param p_msg The message to send.
         */
		void send(const spk::Network::Message& p_msg);
	};
}
