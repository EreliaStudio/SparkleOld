#pragma once

#include "network/spk_network_object.hpp"
#include "network/spk_message.hpp"
#include "threading/spk_thread_safe_queue.hpp"

namespace spk
{
	class Socket : spk::NetworkObject
	{
		friend class Server;
		friend class Client;
		friend class Acceptor;

	public:
		enum class ReadResult
		{
			Success,
			NothingToRead,
			Closed
		};

	private:
		static inline u_long BLOCKING_SOCKET = 0;
		static inline u_long NON_BLOCKING_SOCKET = 1;

		SOCKET _socket = NULL;
		bool _isConnected = false;

		void connect(SOCKET p_socket);
		void connect(const std::wstring& p_serverAddress, const size_t& p_serverPort);

		void close();

	public:
		bool isConnected();

		void send(const spk::Message& p_msg);
		ReadResult receive(spk::Message& p_messageToFill);
	};
}