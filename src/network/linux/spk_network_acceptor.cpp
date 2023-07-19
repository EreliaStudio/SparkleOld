#include "network/spk_network_acceptor.hpp"
#include <fcntl.h>
#include "iostream/spk_iostream.hpp"
#include "spk_basic_functions.hpp"

namespace spk::Network
{
	Acceptor::Acceptor() :
		_socket(INVALID_SOCKET)
	{
	}

	Acceptor::~Acceptor()
	{
		close(_socket);
	}

	void Acceptor::start(const size_t& p_port)
	{
		spk::cout << L"Starting server on port " << p_port << std::endl;

		struct addrinfo* result = NULL, hints = {};

		hints.ai_family = AF_INET;
		hints.ai_socktype = SOCK_STREAM;
		hints.ai_protocol = IPPROTO_TCP;
		hints.ai_flags = AI_PASSIVE;

		int AcceptorAddressResolutionError = ::getaddrinfo(NULL, std::to_string(p_port).c_str(), &hints, &result);
		if (AcceptorAddressResolutionError != 0)
		{
			spk::throwException(L"Acceptor creation failed : getaddrinfo error code [" + std::to_wstring(AcceptorAddressResolutionError) + L"]");
		}

		_socket = ::socket(result->ai_family, result->ai_socktype, result->ai_protocol);
		if (_socket == INVALID_SOCKET)
		{
			spk::throwException(L"Listening socket creation failed : socket error code [" + std::to_wstring(errno) + L"]");
		}

		int flags = fcntl(_socket, F_GETFL, 0);
		if (flags == -1)
		{
			spk::throwException(L"Error getting socket flags: error code [" + std::to_wstring(errno) + L"]");
		}

		flags |= O_NONBLOCK;
		int res = fcntl(_socket, F_SETFL, flags);
		if (res == -1)
		{
			spk::throwException(L"Error setting socket to non-blocking mode: error code [" + std::to_wstring(errno) + L"]");
		}

		int bindingListeningSocketError = ::bind(_socket, result->ai_addr, (int)result->ai_addrlen);
		if (bindingListeningSocketError == -1)
		{
			spk::throwException(L"Binding listening socket creation failed : socket error code [" + std::to_wstring(errno) + L"]");
		}
		freeaddrinfo(result);

		if (::listen(_socket, SOMAXCONN) == -1)
		{
			spk::throwException(L"Error while listening : socket error code [" + std::to_wstring(errno) + L"]");
		}
	}

	void Acceptor::stop()
	{
		close(_socket);
	}

	bool Acceptor::accept(Socket& p_socket)
	{
		int newConnectionSocket = ::accept(_socket, NULL, NULL);

		if (newConnectionSocket == INVALID_SOCKET)
		{
			if (errno == EAGAIN || errno == EWOULDBLOCK)
			{
				return false;
			}
			else
			{
				spk::throwException(L"Error while accepting a new connection: socket error code [" + std::to_wstring(errno) + L"]");
			}
		}

		p_socket.connect(newConnectionSocket);
		return (true);
	}
}