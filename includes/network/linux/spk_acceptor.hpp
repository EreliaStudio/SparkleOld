#pragma once

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include "network/spk_socket.hpp"

namespace spk
{ 
    /**
     * @class Acceptor
     * @brief The Acceptor class is a wrapper around a socket, intended to accept connections from clients.
     *
     * An Acceptor object represents a server-side, listening socket. This class
     * simplifies the task of starting a server and accepting client connections.
     */
	class Acceptor
	{
	private:
		static constexpr int INVALID_SOCKET = -1;
		int _socket; /**< The socket file descriptor */

	public:
        /**
         * @brief Constructs a new Acceptor
         *
         * This constructor initializes the internal socket to a safe state.
         */
		Acceptor();

        /**
         * @brief Destructs the Acceptor
         *
         * This destructor ensures the Acceptor is properly closed upon destruction.
         */
		~Acceptor();

        /**
         * @brief Starts the Acceptor listening on a specified port.
         *
         * This function starts the Acceptor, allowing it to listen for client connections on
         * the specified port.
         *
         * @param p_port The port to listen on.
         */
		void start(const size_t& p_port);

        /**
         * @brief Stops the Acceptor
         *
         * This function stops the Acceptor, closing its socket and stopping it from
         * accepting any further connections.
         */
		void stop();

        /**
         * @brief Accepts a new client connection
         *
         * This function accepts a new client connection, creating a new Socket for 
         * that connection and returning true if the operation was successful.
         *
         * @param p_socket The Socket object to be filled with the new client connection.
         * @return True if a new client was successfully accepted, false otherwise.
         */
		bool accept(Socket& p_socket);
	};
}
