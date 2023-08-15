#include "network/linux/spk_network_socket.hpp"
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <fcntl.h>
#include "spk_basic_functions.hpp"

namespace spk::Network
{
	void Socket::connect(int p_socket)
	{
		_socket = p_socket;
		_isConnected = true;
	}

	void Socket::connect(const std::wstring &p_serverAddress, const size_t &p_serverPort)
	{
		struct addrinfo *result = NULL;
		struct addrinfo *ptr = NULL;
		struct addrinfo hints;

		memset(&hints, 0, sizeof(hints));
		hints.ai_family = AF_UNSPEC;
		hints.ai_socktype = SOCK_STREAM;
		hints.ai_protocol = IPPROTO_TCP;

		int sourceAddressResolutionError = getaddrinfo(spk::wstringToString(p_serverAddress).c_str(), std::to_string(p_serverPort).c_str(), &hints, &result);
		if (sourceAddressResolutionError != 0)
		{
			spk::throwException(L"Source creation failed : getaddrinfo error code [" + std::to_wstring(sourceAddressResolutionError) + L"]");
		}

		_socket = -1;

		for (ptr = result; ptr != NULL; ptr = ptr->ai_next)
		{
			_socket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);

			int socketConnectionError = ::connect(_socket, result->ai_addr, result->ai_addrlen);
			if (socketConnectionError == -1)
			{
				spk::throwException(L"Unable to connect to server [" + p_serverAddress + L"] at port [" + std::to_wstring(p_serverPort) + L"][" + std::to_wstring(errno) + L"]");
			}
		}

		int flags = fcntl(_socket, F_GETFL, 0);
		fcntl(_socket, F_SETFL, flags | O_NONBLOCK);

		_isConnected = true;

		freeaddrinfo(result);
	}

	void Socket::close()
	{
		int shutdowningSocketError = shutdown(_socket, SHUT_WR);
		if (shutdowningSocketError == -1)
		{
			spk::throwException(L"shutdown failed with error: " + std::to_wstring(errno));
		}
		::close(_socket);
		_isConnected = false;
	}

	bool Socket::isConnected()
	{
		return (_isConnected);
	}

	void Socket::send(const spk::Network::Message &p_msg)
	{
		int sendingMessageHeaderError = ::send(_socket, reinterpret_cast<const char *>(&(p_msg.header())), sizeof(spk::Network::Message::Header), 0);
		if (sendingMessageHeaderError == -1)
		{
			spk::throwException(L"Error while sending header: socket error code [" + std::to_wstring(errno) + L"]");
		}

		if (p_msg.size() != 0)
		{
			int sendingMessageDataError = ::send(_socket, reinterpret_cast<const char *>(p_msg.data()), p_msg.size(), 0);

			if (sendingMessageDataError == -1)
			{
				spk::throwException(L"Error while sending data: socket error code [" + std::to_wstring(errno) + L"]");
			}
		}
	}

	Socket::ReadResult Socket::receive(spk::Network::Message &p_messageToFill)
	{
		spk::Network::Message newMessage;
		int bytesRead = ::recv(_socket, reinterpret_cast<char *>(&(p_messageToFill.header())), sizeof(spk::Network::Message::Header), 0);

		if (bytesRead == 0)
		{
			close();
			return ReadResult::Closed;
		}
		else if (bytesRead < 0)
		{
			if (errno != EAGAIN)
			{
				spk::throwException(L"Error while receiving header: socket error code [" + std::to_wstring(errno) + L"]");
			}
			else
			{
				return ReadResult::NothingToRead;
			}
		}

		if (p_messageToFill.size() != 0)
		{
			bytesRead = ::recv(_socket, reinterpret_cast<char *>(p_messageToFill.data()), p_messageToFill.size(), 0);

			if (bytesRead != p_messageToFill.size())
			{
				spk::throwException(L"Error while receiving data: socket error code [" + std::to_wstring(errno) + L"]");
			}
		}

		return (ReadResult::Success);
	}
}