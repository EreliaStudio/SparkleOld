#pragma once

#include "network/spk_network_network_object.hpp"
#include "network/spk_network_message.hpp"
#include "threading/spk_thread_safe_queue.hpp"

namespace spk::Network
{
    /**
     * @class Socket
     * @brief The Socket class encapsulates a network socket, and includes methods for establishing connections, sending messages, and receiving messages.
     *
     * The Socket class extends from the Object class. The Server, Client, and Acceptor classes are friends of Socket, giving them access to its private members and methods.
     */
	class Socket : spk::Network::Object
	{
		friend class Server;
		friend class Client;
		friend class Acceptor;

	public:
        /**
         * @enum ReadResult
         * @brief Enum representing the result of a read operation.
         *
         * Success - The read operation was successful.
         * NothingToRead - There was nothing to read from the socket.
         * Closed - The socket was closed.
         */
		enum class ReadResult
		{
			Success,
			NothingToRead,
			Closed,
            Timeout
		};

	private:
		static inline u_long BLOCKING_SOCKET = 0; /**< Constant representing a blocking socket */
		static inline u_long NON_BLOCKING_SOCKET = 1; /**< Constant representing a non-blocking socket */

		SOCKET _socket = SOCKET(); /**< The socket file descriptor */
		bool _isConnected = false; /**< Boolean representing whether the socket is currently connected */

		/**
         * @brief Connects this socket to another existing socket.
         *
         * @param p_socket The file descriptor of the socket to connect to.
         */
		void connect(SOCKET p_socket);

		/**
         * @brief Connects this socket to a server.
         *
         * @param p_serverAddress The address of the server to connect to.
         * @param p_serverPort The port of the server to connect to.
         */
		void connect(const std::wstring& p_serverAddress, const size_t& p_serverPort);

		/**
         * @brief Closes the socket, stopping any further send and receive operations.
         */
		void close();

        Socket::ReadResult _receiveHeader(spk::Network::Message &p_messageToFill);
        Socket::ReadResult _waitForSelection();
        Socket::ReadResult _receiveContent(spk::Network::Message &p_messageToFill);

	public:
		/**
         * @brief Checks whether the socket is currently connected.
         *
         * @return True if the socket is connected, false otherwise.
         */
		bool isConnected();

        const SOCKET& socket() const;

		/**
         * @brief Sends a message over the socket.
         *
         * @param p_msg The message to send.
         */
		void send(const spk::Network::Message& p_msg);

		/**
         * @brief Receives a message from the socket.
         *
         * @param p_messageToFill The message object to fill with the received data.
         * @return The result of the read operation (see ReadResult).
         */
		ReadResult receive(spk::Network::Message& p_messageToFill);
	};
}
