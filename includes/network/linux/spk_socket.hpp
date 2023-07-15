#pragma once

#include <string>
#include "network/spk_message.hpp"

namespace spk
{
	class Socket
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
		int _socket = -1;
		bool _isConnected = false;

		void connect(int p_socket);
		void connect(const std::wstring& p_serverAddress, const size_t& p_serverPort);

		void close();

	public:
		bool isConnected();

		void send(const spk::Message& p_msg);
		ReadResult receive(spk::Message& p_messageToFill);
	};
}