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

		static void _initializeWinSockData();
		static void _releaseWinSockData();
	public:
		NetworkObject();
		NetworkObject(const NetworkObject& p_other);   
		NetworkObject(NetworkObject&& p_other);

		NetworkObject& operator=(const NetworkObject& p_other);
		NetworkObject& operator=(NetworkObject&& p_other);

		~NetworkObject();
	};
}