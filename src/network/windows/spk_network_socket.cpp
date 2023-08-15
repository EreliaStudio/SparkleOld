#include "network/windows/spk_network_socket.hpp"

namespace spk::Network
{
	void Socket::connect(SOCKET p_socket)
	{
		_socket = p_socket;
		_isConnected = true;
	}

	void Socket::connect(const std::wstring &p_serverAddress, const size_t &p_serverPort)
	{
		struct addrinfo *result = NULL;
		struct addrinfo *ptr = NULL;
		struct addrinfo hints;

		ZeroMemory(&hints, sizeof(hints));
		hints.ai_family = AF_UNSPEC;
		hints.ai_socktype = SOCK_STREAM;
		hints.ai_protocol = IPPROTO_TCP;

		int sourceAddressResolutionError = ::getaddrinfo(spk::wstringToString(p_serverAddress).c_str(), std::to_string(p_serverPort).c_str(), &hints, &result);
		if (sourceAddressResolutionError != 0)
		{
			spk::throwException(L"Source creation failed : getaddrinfo error code [" + std::to_wstring(sourceAddressResolutionError) + L"]");
		}

		_socket = INVALID_SOCKET;

		for (ptr = result; ptr != NULL; ptr = ptr->ai_next)
		{
			_socket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);

			int socketConnectionError = ::connect(_socket, result->ai_addr, (int)result->ai_addrlen);
			if (socketConnectionError == SOCKET_ERROR)
			{
				spk::throwException(L"Unable to connect to server [" + p_serverAddress + L"] at port [" + std::to_wstring(p_serverPort) + L"][" + std::to_wstring(WSAGetLastError()) + L"]");
			}
		}

		int settingSocketBlockingModeError = ioctlsocket(_socket, FIONBIO, &NON_BLOCKING_SOCKET);
		if (settingSocketBlockingModeError != NO_ERROR)
		{
			spk::throwException(L"ioctlsocket failed with error: " + std::to_wstring(settingSocketBlockingModeError));
		}

		_isConnected = true;

		freeaddrinfo(result);
	}

	void Socket::close()
	{

		int shutdowningSocketError = shutdown(_socket, SD_SEND);
		if (shutdowningSocketError == SOCKET_ERROR)
		{
			spk::throwException(L"ioctlsocket failed with error: " + std::to_wstring(WSAGetLastError()));
		}
		closesocket(_socket);
		_isConnected = false;
	}

	bool Socket::isConnected()
	{
		return (_isConnected);
	}

	void Socket::send(const spk::Network::Message &p_msg)
	{
		int sendingMessageHeaderError = ::send(_socket, reinterpret_cast<const char *>(&(p_msg.header())), sizeof(spk::Network::Message::Header), 0);
		if (sendingMessageHeaderError == SOCKET_ERROR)
		{
			spk::throwException(L"Error while sending header: socket error code [" + std::to_wstring(WSAGetLastError()) + L"]");
		}

		if (p_msg.size() != 0)
		{
			int sendingMessageDataError = ::send(_socket, reinterpret_cast<const char *>(p_msg.data()), p_msg.size(), 0);

			if (sendingMessageHeaderError == SOCKET_ERROR)
			{
				spk::throwException(L"Error while sending data: socket error code [" + std::to_wstring(WSAGetLastError()) + L"]");
			}
		}
	}

	Socket::ReadResult Socket::receive(spk::Network::Message &p_messageToFill)
	{
		int bytesRead = ::recv(_socket, reinterpret_cast<char *>(&(p_messageToFill.header())), sizeof(spk::Network::Message::Header), 0);
		spk::cout << "Bytes read : " << bytesRead  << " vs header size : " << sizeof(spk::Network::Message::Header) << std::endl;
		spk::cout << "Last error : " << WSAGetLastError() << " vs " << WSAEWOULDBLOCK << std::endl;

		if (bytesRead == 0)
		{
			return ReadResult::Closed;
		}
		else if (bytesRead == SOCKET_ERROR)
		{
			int error = WSAGetLastError();
			if (error != WSAEWOULDBLOCK)
			{
				spk::throwException(L"Error while receiving header: socket error code [" + std::to_wstring(error) + L"]");
			}
			else
			{
				return ReadResult::NothingToRead;
			}
		}

		DEBUG_LINE();
		size_t totalReadedSize = 0;
		while (totalReadedSize < p_messageToFill.size())
		{
		DEBUG_LINE();
			bytesRead = ::recv(_socket, reinterpret_cast<char *>(p_messageToFill.data()), p_messageToFill.size(), 0);
		DEBUG_LINE();

			spk::cout << "Bytes read : " << bytesRead << " -> " << totalReadedSize << " vs size : " << p_messageToFill.size() << std::endl;
			spk::cout << "Last error : " << WSAGetLastError() << " vs " << WSAEWOULDBLOCK << std::endl;

			if (bytesRead == SOCKET_ERROR)
			{
				int error = WSAGetLastError();
				if (error != WSAEWOULDBLOCK)
				{
					spk::throwException(L"Error while receiving data: socket error code [" + std::to_wstring(error) + L"]");
				}
				DEBUG_LINE();
			}
			else if (bytesRead == 0)
			{
				DEBUG_LINE();
				return ReadResult::Closed;
			}
			else
			{
				totalReadedSize += bytesRead;
			}
		}
		DEBUG_LINE();

		return ReadResult::Success;
	}
}