#pragma once

#include "network/spk_network_object.hpp"
#include "network/spk_message.hpp"
#include "threading/spk_thread_safe_queue.hpp"
#include "network/spk_socket.hpp"

namespace spk
{ 
	class Sink : spk::NetworkObject
	{
	private:
		static inline u_long BLOCKING_SOCKET = 0;
		static inline u_long NON_BLOCKING_SOCKET = 1;

		SOCKET _socket;

	public:
		Sink() :
			spk::NetworkObject(),
			_socket(NULL)
		{

		}

		~Sink()
		{
			
		}

		void start(const size_t& p_port)
		{
			spk::cout << L"Starting server on port " << p_port << std::endl;

			struct addrinfo *result = NULL, hints;

			ZeroMemory(&hints, sizeof (hints));
			hints.ai_family = AF_INET;
			hints.ai_socktype = SOCK_STREAM;
			hints.ai_protocol = IPPROTO_TCP;
			hints.ai_flags = AI_PASSIVE;

			int sinkAddressResolutionError = ::getaddrinfo(NULL, std::to_string(p_port).c_str(), &hints, &result);
			if (sinkAddressResolutionError != 0) {
				spk::throwException(L"Sink creation failed : getaddrinfo error code [" + std::to_wstring(sinkAddressResolutionError) + L"]");
			}

			_socket = ::socket(result->ai_family, result->ai_socktype, result->ai_protocol);
			if (_socket == INVALID_SOCKET) {
				spk::throwException(L"Listening socket creation failed : socket error code [" + std::to_wstring(WSAGetLastError()) + L"]");
			}
			
			int settingSocketBlockingModeError = ioctlsocket(_socket, FIONBIO, &NON_BLOCKING_SOCKET);
			if (settingSocketBlockingModeError != NO_ERROR) {
				spk::throwException(L"ioctlsocket failed with error: " + std::to_wstring(settingSocketBlockingModeError));
			}

			int bindingListeningSocketError = ::bind( _socket, result->ai_addr, (int)result->ai_addrlen);
			if (bindingListeningSocketError == SOCKET_ERROR) {
				spk::throwException(L"Binding listening socket creation failed : socket error code [" + std::to_wstring(WSAGetLastError()) + L"]");
			}
			freeaddrinfo(result);

			if ( ::listen( _socket, SOMAXCONN ) == SOCKET_ERROR ) {
				spk::throwException(L"Error while listening : socket error code [" + std::to_wstring(WSAGetLastError()) + L"]");
			}
		}

		~Sink()
		{
			
		}

		void start(const size_t& p_port)
		{
			spk::cout << L"Starting server on port " << p_port << std::endl;

			struct addrinfo *result = NULL, hints;

			ZeroMemory(&hints, sizeof (hints));
			hints.ai_family = AF_INET;
			hints.ai_socktype = SOCK_STREAM;
			hints.ai_protocol = IPPROTO_TCP;
			hints.ai_flags = AI_PASSIVE;

			int sinkAddressResolutionError = ::getaddrinfo(NULL, std::to_string(p_port).c_str(), &hints, &result);
			if (sinkAddressResolutionError != 0) {
				spk::throwException(L"Sink creation failed : getaddrinfo error code [" + std::to_wstring(sinkAddressResolutionError) + L"]");
			}

			_socket = ::socket(result->ai_family, result->ai_socktype, result->ai_protocol);
			if (_socket == INVALID_SOCKET) {
				spk::throwException(L"Listening socket creation failed : socket error code [" + std::to_wstring(WSAGetLastError()) + L"]");
			}
			
			int settingSocketBlockingModeError = ioctlsocket(_socket, FIONBIO, &NON_BLOCKING_SOCKET);
			if (settingSocketBlockingModeError != NO_ERROR) {
				spk::throwException(L"ioctlsocket failed with error: " + std::to_wstring(settingSocketBlockingModeError));
			}

			int bindingListeningSocketError = ::bind( _socket, result->ai_addr, (int)result->ai_addrlen);
			if (bindingListeningSocketError == SOCKET_ERROR) {
				spk::throwException(L"Binding listening socket creation failed : socket error code [" + std::to_wstring(WSAGetLastError()) + L"]");
			}
			freeaddrinfo(result);

			if ( ::listen( _socket, SOMAXCONN ) == SOCKET_ERROR ) {
				spk::throwException(L"Error while listening : socket error code [" + std::to_wstring(WSAGetLastError()) + L"]");
			}
		}

		void stop()
		{
			closesocket(_socket);
		}

		bool accept(Socket& p_socket)
		{
			SOCKET newConnectionSocket = ::accept(_socket, NULL, NULL);

			if (newConnectionSocket == INVALID_SOCKET)
    		{
				int error = WSAGetLastError();
				if (error != WSAEWOULDBLOCK)
				{
					spk::throwException(L"Error while accepting a new connection: socket error code [" + std::to_wstring(error) + L"]");
				}
				else
				{
					return false;
				}
			}
			
			p_socket.connect(newConnectionSocket);
			return (true);
		}
	};
}