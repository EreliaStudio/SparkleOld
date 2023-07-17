#include "network/spk_network_object.hpp"
#include "spk_basic_functions.hpp"

namespace spk
{
	void NetworkObject::_initializeWinSockData()
	{
		std::lock_guard<std::recursive_mutex> lock(_mutex);
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
		std::lock_guard<std::recursive_mutex> lock(_mutex);
		spk::cout << "Releasing WinSockData" << std::endl;
		
		printCallStack();

		exit(1);
		WSACleanup();
	}
	void NetworkObject::_incrementCount()
	{
		std::lock_guard<std::recursive_mutex> lock(_mutex);
		if (nb_element == 0)
			_initializeWinSockData();
		nb_element++;
	}
		
	void NetworkObject::_decrementCount()
	{
		std::lock_guard<std::recursive_mutex> lock(_mutex);
		nb_element--;
		if (nb_element == 0)
			_releaseWinSockData();
	}

	NetworkObject::NetworkObject()
	{
        _incrementCount();
	}

	NetworkObject::NetworkObject(const NetworkObject &p_other)
	{
        _incrementCount();
	}

	NetworkObject::NetworkObject(NetworkObject&& p_other)
    {
        _incrementCount();
    }

    NetworkObject& NetworkObject::operator=(const NetworkObject& p_other)
    {
        if (this != &p_other)
        {
            _incrementCount();
        }
        return *this;
    }

    NetworkObject& NetworkObject::operator=(NetworkObject&& p_other)
    {
        if (this != &p_other)
        {
            _incrementCount();
        }
        return *this;
    }

	NetworkObject::~NetworkObject()
	{
		_decrementCount();
	}

}