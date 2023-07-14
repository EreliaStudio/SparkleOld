#pragma once

#include "spk_system_define.hpp"
#include "spk_basic_functions.hpp"

namespace spk
{
	class NetworkObject
	{
	private:
		static inline size_t nb_element = 0;
		static inline WSADATA wsaData;

		static void _initializeWinSockData()
		{
			spk::cout << "Requesting initialization of WinSockData" << std::endl;
			int initializationResult;

			initializationResult = WSAStartup(MAKEWORD(2,2), &wsaData);
			if (initializationResult != 0) {
				spk::throwException(L"Initialization of network failed : WSAStartup error code [" + std::to_wstring(initializationResult) + L"]");
			}
		}

		static void _releaseWinSockData()
		{
			spk::cout << "Releasing WinSockData" << std::endl;
			WSACleanup();
		}
	public:
		NetworkObject()
		{
			if (nb_element == 0)
				_initializeWinSockData();
			nb_element++;	
		}

		NetworkObject(const NetworkObject& p_other)
		{
			if (nb_element == 0)
				_initializeWinSockData();
			nb_element++;	
		}

		~NetworkObject()
		{
			nb_element--;	
			if (nb_element == 0)
				_releaseWinSockData();	
		}
	};
}