#pragma once

#include <unordered_map>
#include "network/spk_network_message.hpp"
#include "network/spk_network_acceptor.hpp"
#include "network/spk_network_socket.hpp"
#include "threading/spk_persistent_worker.hpp"
#include "threading/spk_thread_safe_queue.hpp"
#include "spk_basic_functions.hpp"

namespace spk::Network
{
    /**
     * @class Server
     * @brief Class that manages network communications for a server.
     *
     * The Server class provides functionality for starting and stopping the server, handling messages, and managing callbacks for events like new connections and disconnections.
     */
	class Server
	{
	public:
		using EmiterID = spk::Network::Message::Header::EmiterID; /**< Type alias for emitter ID */

	private:
		static const EmiterID MINIMAL_ID_VALUE = 10000; /**< Minimal valid emitter ID value */

		spk::PersistentWorker _socketContextWorker; /**< Persistent worker for socket context */
		spk::ContractProvider::Contract _socketListeningContract; /**< Contract for socket listening */
		spk::ContractProvider::Contract _readingIncomingMessageContract; /**< Contract for reading incoming messages */

		Acceptor _Acceptor; /**< The server socket acceptor */
        spk::Network::Socket::FileDescriptor _maxFDs = SOCKET_ERROR;
        fd_set _readingFDs;

		spk::ThreadSafeQueue<std::pair<EmiterID, spk::Network::Message>> _messagesToTreat; /**< Queue for storing incoming messages */

		std::unordered_map<EmiterID, Socket> _clients; /**< A map to store client connections */
		std::unordered_map<spk::Network::Message::Type, std::function<void(const EmiterID&, const spk::Network::Message&)>> _onMessageReceptionCallbacks; /**< Map for callbacks that are executed upon message reception */

		std::function<void(const EmiterID&)> _onNewConnectionCallback = nullptr; /**< Callback function for new connection */
		std::function<void(const EmiterID&)> _onConnectionDisconnectionCallback = nullptr; /**< Callback function for connection disconnection */
		std::function<void(const EmiterID&, const spk::Network::Message&)> _onUnknownMessageTypeCallback = [&](const EmiterID& p_id, const spk::Network::Message& p_msg){
			spk::cout << L"Callback not defined for message id [" << std::to_wstring(p_msg.header().type()) << L"]" << std::endl;
		};

		void _treatMessage(const EmiterID& p_emiterID, const spk::Network::Message& p_msg); /**< Function to process a received message */
		long long _findValidID(); /**< Function to find a valid ID for a new client */

	public:
        /**
         * @brief Constructs a new Server.
         */
		Server();

        /**
         * @brief Destructs the Server.
         */
		~Server();

        /**
         * @brief Starts the server.
         *
         * @param p_serverPort Port number the server will listen to.
         */
		void start(const size_t& p_serverPort);

        /**
         * @brief Stops the server.
         */
		void stop();

        /**
         * @brief Processes all incoming messages.
         */
		void treatMessages();

        /**
         * @brief Sets the callback function for a specific message type.
         *
         * @param p_messageType The type of the message.
         * @param p_funct The callback function.
         */
        template <typename TMessageType>
		void setOnMessageReceptionCallback(const TMessageType& p_messageType, std::function<void(const EmiterID&, const spk::Network::Message&)> p_funct)
        {
            Message::Type messageType = static_cast<Message::Type>(p_messageType);
            if (_onMessageReceptionCallbacks.contains(messageType) == true)
                spk::throwException(L"Callback already define for message type [" + std::to_wstring(messageType) + L"]");
            _onMessageReceptionCallbacks[messageType] = std::bind(p_funct, std::placeholders::_1, std::placeholders::_2);
        }

        /**
         * @brief Sets the callback function for a new connection event.
         *
         * @param p_funct The callback function.
         */
		void setNewConnectionCallback(std::function<void(const EmiterID&)> p_funct);

        /**
         * @brief Sets the callback function for a connection disconnection event.
         *
         * @param p_funct The callback function.
         */
		void setConnectionDisconnectionCallback(std::function<void(const EmiterID&)> p_funct);

        /**
         * @brief Sets the callback function for an unknown message type.
         *
         * @param p_funct The callback function.
         */
		void setUnknowMessageReceptionCallback(std::function<void(const EmiterID&, const spk::Network::Message&)> p_funct);

        /**
         * @brief Sends a message to a specific client.
         *
         * @param p_emiterID The ID of the client to send the message to.
         * @param p_msg The message to send.
         */
		void sendTo(const Server::EmiterID& p_emiterID, const spk::Network::Message& p_msg);
	};
}
