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
         */
		enum class ReadResult
		{
            Success,       /**< Indicates the read was successful. */
            NothingToRead, /**< Indicates there was nothing to read from the socket. */
            Closed,        /**< Indicates the socket was closed. */
            Timeout        /**< Indicates the socket was timeout during reception of incoming message. */
		};

        
        using FileDescriptor = SOCKET; /**< A definition for socket's file descriptor*/

	private:
		static inline u_long BLOCKING_SOCKET = 0; /**< Constant representing a blocking socket */
		static inline u_long NON_BLOCKING_SOCKET = 1; /**< Constant representing a non-blocking socket */

		FileDescriptor _socket = SOCKET(); /**< The socket file descriptor */
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


        /**
         * @brief Submethod used by the receive method to read the header
         * @param p_messageToFill The message holding the header to read.
         * @return The status of the reading process.
         * If the header have been successfully readed, return Success.
         * If the header haven't been readed correctly, return NothingToRead.
         * If the connection have been closed by the socket or by the server, return Closed.
        */
        Socket::ReadResult _receiveHeader(spk::Network::Message &p_messageToFill);

        /**
         * @brief Submethod used by the receive method, to wait until the content of the message is arrived.
         * @return The status of the selection process.
         * If datas are availible, return Success.
         * If no data are availible before the end of the timeout, return Timeout.
        */
        Socket::ReadResult _waitForSelection();

        /**
         * @brief Submethod used by the receive method to read the content
         * @param p_messageToFill The message holding the content to read.
         * @return The status of the reading process.
         * If the content have been successfully readed, return Success.
         * If the connection have been closed by the socket or by the server, return Closed.
        */
        Socket::ReadResult _receiveContent(spk::Network::Message &p_messageToFill);

	public:
		/**
         * @brief Checks whether the socket is currently connected.
         *
         * @return True if the socket is connected, false otherwise.
         */
		bool isConnected();

        /**
         * @brief Return the holded file descriptor from the socket.
        */
        const SOCKET& fileDescriptor() const;

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
