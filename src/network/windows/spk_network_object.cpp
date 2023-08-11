#include "network/spk_network_network_object.hpp"
#include "spk_basic_functions.hpp"

namespace spk::Network
{
	void Object::_initializeWinSockData()
	{
		std::lock_guard<std::recursive_mutex> lock(_mutex);

		int initializationResult;

		initializationResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
		if (initializationResult != 0)
		{
			spk::throwException(L"Initialization of network failed : WSAStartup error code [" + std::to_wstring(initializationResult) + L"]");
		}
	}

	void Object::_releaseWinSockData()
	{
		std::lock_guard<std::recursive_mutex> lock(_mutex);
		
		WSACleanup();
	}
	void Object::_incrementCount()
	{
		std::lock_guard<std::recursive_mutex> lock(_mutex);
		if (nb_element == 0)
			_initializeWinSockData();
		nb_element++;
	}
		
	void Object::_decrementCount()
	{
		std::lock_guard<std::recursive_mutex> lock(_mutex);
		nb_element--;
		if (nb_element == 0)
			_releaseWinSockData();
	}

	Object::Object()
	{
        _incrementCount();
	}

	Object::Object(const Object &p_other)
	{
        _incrementCount();
	}

	Object::Object(Object&& p_other)
    {
        _incrementCount();
    }

    Object& Object::operator=(const Object& p_other)
    {
        if (this != &p_other)
        {
            _incrementCount();
        }
        return *this;
    }

    Object& Object::operator=(Object&& p_other)
    {
        if (this != &p_other)
        {
            _incrementCount();
        }
        return *this;
    }

	Object::~Object()
	{
		_decrementCount();
	}

}