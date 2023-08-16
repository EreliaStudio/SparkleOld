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
         * @brief The result of a read operation.
         */
		enum class ReadResult
		{
            Success,       //!< Indicates the read was successful. */
            NothingToRead, //!< Indicates there was nothing to read from the socket. */
            Closed,        //!< Indicates the socket was closed. */
            Timeout        //!< Indicates the socket was timeout during reception of incoming message. */
		};

        using FileDescriptor = int; /**< A definition for socket's file descriptor*/

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
         * @brief Checks if the socket is currently connected.
         * @return True if the socket is connected, false otherwise.
         */
		bool isConnected();

        /**
         * @brief Return the holded file descriptor from the socket.
        */
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
