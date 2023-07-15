#pragma once

#include "network/spk_network_object.hpp"
#include "network/spk_message.hpp"
#include "threading/spk_thread_safe_queue.hpp"
#include "network/spk_socket.hpp"

namespace spk
{ 
	class Acceptor : spk::NetworkObject
	{
	private:
		static inline u_long BLOCKING_SOCKET = 0;
		static inline u_long NON_BLOCKING_SOCKET = 1;

		SOCKET _socket;

	public:
		Acceptor();
		~Acceptor();

		void start(const size_t& p_port);

		void stop();

		bool accept(Socket& p_socket);
	};
}