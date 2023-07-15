#include "network/linux/spk_socket.hpp"
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/ioctl.h>
#include <unistd.h>

namespace spk
{
	void Socket::connect(int p_socket)
	{
		_socket = p_socket;
		_isConnected = true;
	}

	void Socket::connect(const std::string &p_serverAddress, const size_t &p_serverPort)
	{
		struct addrinfo *result = NULL;
		struct addrinfo *ptr = NULL;
		struct addrinfo hints;

		memset(&hints, 0, sizeof(hints));
		hints.ai_family = AF_UNSPEC;
		hints.ai_socktype = SOCK_STREAM;
		hints.ai_protocol = IPPROTO_TCP;

		int sourceAddressResolutionError = getaddrinfo(p_serverAddress.c_str(), std::to_string(p_serverPort).c_str(), &hints, &result);
		if (sourceAddressResolutionError != 0)
		{
			spk::throwException("Source creation failed : getaddrinfo error code [" + std::to_string(sourceAddressResolutionError) + "]");
		}

		_socket = -1;

		for (ptr = result; ptr != NULL; ptr = ptr->ai_next)
		{
			_socket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);

			int socketConnectionError = ::connect(_socket, result->ai_addr, result->ai_addrlen);
			if (socketConnectionError == -1)
			{
				spk::throwException("Unable to connect to server [" + p_serverAddress + "] at port [" + std::to_string(p_serverPort) + "][" + std::to_string(errno) + "]");
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
			spk::throwException("shutdown failed with error: " + std::to_string(errno));
		}
		::close(_socket);
		_isConnected = false;
	}

	bool Socket::isConnected()
	{
		return (_isConnected);
	}

	void Socket::send(const spk::Message &p_msg)
	{
		int sendingMessageHeaderError = ::send(_socket, reinterpret_cast<const char *>(&(p_msg.header())), sizeof(spk::Message::Header), 0);
		if (sendingMessageHeaderError == -1)
		{
			spk::throwException("Error while sending header: socket error code [" + std::to_string(errno) + "]");
		}

		if (p_msg.size() != 0)
		{
			int sendingMessageDataError = ::send(_socket, reinterpret_cast<const char *>(p_msg.data()), p_msg.size(), 0);

			if (sendingMessageDataError == -1)
			{
				spk::throwException("Error while sending data: socket error code [" + std::to_string(errno) + "]");
			}
		}
	}

	Socket::ReadResult Socket::receive(spk::Message &p_messageToFill)
	{
		spk::Message newMessage;
		int bytesRead = ::recv(_socket, reinterpret_cast<char *>(&(p_messageToFill.header())), sizeof(spk::Message::Header), 0);

		if (bytesRead == 0)
		{
			close();
			return ReadResult::Closed;
		}
		else if (bytesRead < 0)
		{
			if (errno != EAGAIN)
			{
				spk::throwException("Error while receiving header: socket error code [" + std::to_string(errno) + "]");
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
				spk::throwException("Error while receiving data: socket error code [" + std::to_string(errno) + "]");
			}
		}

		return (ReadResult::Success);
	}
}