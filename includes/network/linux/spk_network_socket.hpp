#pragma once

#include <string>
#include "network/spk_network_message.hpp"

namespace spk::Network
{
    /**
     * @class Socket
     * @brief The Socket class is a wrapper around a network socket.
     *
     * The Socket class is a higher-level representation of a network socket, which 
     * includes methods for establishing connections, sending messages, and receiving messages.
     */
	class Socket
	{
		friend class Server;
		friend class Client;
		friend class Acceptor;

	public:
        /**
         * @enum ReadResult
         * @brief The result of a read operation.
         *
         * Success - The read was successful.
         * NothingToRead - There was nothing to read from the socket.
         * Closed - The socket was closed.
         */
		enum class ReadResult
		{
			Success,
			NothingToRead,
			Closed
		};

        using FileDescriptor = int;

	private:
		int _socket = -1;  /**< The socket file descriptor */
		bool _isConnected = false; /**< Indicates if the socket is currently connected */

        /**
         * @brief Connects the socket to another already-existing socket.
         * @param p_socket The socket file descriptor of the socket to connect to.
         */
		void connect(int p_socket);

        /**
         * @brief Connects the socket to a server.
         * @param p_serverAddress The address of the server to connect to.
         * @param p_serverPort The port of the server to connect to.
         */
		void connect(const std::wstring& p_serverAddress, const size_t& p_serverPort);

        /**
         * @brief Closes the socket.
         *
         * Closes the socket and prevents any further send or receive operations.
         */
		void close();

	public:
        /**
         * @brief Checks if the socket is currently connected.
         * @return True if the socket is connected, false otherwise.
         */
		bool isConnected();

        const FileDescriptor& fileDescriptor() const;

        /**
         * @brief Sends a message over the socket.
         * @param p_msg The message to send.
         */
		void send(const spk::Network::Message& p_msg);

        /**
         * @brief Receives a message from the socket.
         * @param p_messageToFill The message object to fill with the received data.
         * @return The result of the receive operation (see ReadResult).
         */
		ReadResult receive(spk::Network::Message& p_messageToFill);
	};
}
