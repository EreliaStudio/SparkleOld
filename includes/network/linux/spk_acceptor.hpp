#pragma once

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include "network/spk_socket.hpp"

namespace spk
{ 
	class Acceptor
	{
	private:
		static constexpr int INVALID_SOCKET = -1;
		
		int _socket;

	public:
		Acceptor();
		~Acceptor();

		void start(const size_t& p_port);

		void stop();

		bool accept(Socket& p_socket);
	};
}