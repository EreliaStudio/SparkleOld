#include "network/spk_network_object.hpp"
#include "spk_basic_functions.hpp"

namespace spk
{
	void NetworkObject::_initializeWinSockData()
	{
		spk::cout << "Requesting initialization of WinSockData" << std::endl;
		int initializationResult;

		initializationResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
		if (initializationResult != 0)
		{
			spk::throwException(L"Initialization of network failed : WSAStartup error code [" + std::to_wstring(initializationResult) + L"]");
		}
	}

	void NetworkObject::_releaseWinSockData()
	{
		spk::cout << "Releasing WinSockData" << std::endl;
		
		printCallStack();

		exit(1);
		WSACleanup();
	}

	NetworkObject::NetworkObject()
	{
		if (nb_element == 0)
			_initializeWinSockData();
		nb_element++;
	}

	NetworkObject::NetworkObject(const NetworkObject &p_other)
	{
		if (nb_element == 0)
			_initializeWinSockData();
		nb_element++;
	}

	NetworkObject::NetworkObject(NetworkObject&& p_other)
    {
        if (nb_element == 0)
            _initializeWinSockData();
        nb_element++;
    }

    NetworkObject& NetworkObject::operator=(const NetworkObject& p_other)
    {
        if (this != &p_other)
        {
            nb_element++;
        }
        return *this;
    }

    NetworkObject& NetworkObject::operator=(NetworkObject&& p_other)
    {
        if (this != &p_other)
        {
            nb_element++;
        }
        return *this;
    }

	NetworkObject::~NetworkObject()
	{
		nb_element--;
		if (nb_element == 0)
			_releaseWinSockData();
	}

}