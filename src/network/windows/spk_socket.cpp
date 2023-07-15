#include "network/windows/spk_socket.hpp"

namespace spk
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

	void Socket::send(const spk::Message &p_msg)
	{
		int sendingMessageHeaderError = ::send(_socket, reinterpret_cast<const char *>(&(p_msg.header())), sizeof(spk::Message::Header), 0);
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

		if (p_messageToFill.size() != 0)
		{
			bytesRead = ::recv(_socket, reinterpret_cast<char *>(p_messageToFill.data()), p_messageToFill.size(), 0);
			if (bytesRead != p_messageToFill.size())
			{
				spk::throwException(L"Error while receiving data: socket error code [" + std::to_wstring(WSAGetLastError()) + L"]");
			}
		}

		return (ReadResult::Success);
	}
}