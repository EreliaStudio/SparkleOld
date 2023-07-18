#pragma once

#include "spk_system_define.hpp"
#include "spk_basic_functions.hpp"

namespace spk
{
    /**
     * @class NetworkObject
     * @brief Base class for objects that utilize network resources.
     *
     * The NetworkObject class encapsulates the basic behaviors common to network objects, such as socket management.
     * This class is intended to be a base class for other network-related classes, such as sockets, acceptors, etc.
     */
	class NetworkObject
	{
	private:
		static inline std::recursive_mutex _mutex; /**< Mutex for thread safety */
		static inline size_t nb_element = 0; /**< Counter for the number of NetworkObject instances */
		static inline WSADATA wsaData; /**< Windows Sockets API data */

		/**
         * @brief Initializes Windows Sockets API (WinSock) data.
         *
         * This function is responsible for initializing the Windows Sockets API.
         * This should be done before any socket operations are performed.
         */
		static void _initializeWinSockData();

		/**
         * @brief Releases Windows Sockets API (WinSock) data.
         *
         * This function is responsible for releasing the Windows Sockets API resources.
         */
		static void _releaseWinSockData();

		/**
         * @brief Increments the count of NetworkObject instances.
         *
         * This function increments the count of NetworkObject instances, typically when a new instance is created.
         */
		static void _incrementCount();

		/**
         * @brief Decrements the count of NetworkObject instances.
         *
         * This function decrements the count of NetworkObject instances, typically when an instance is destroyed.
         */
		static void _decrementCount();

	public:
		/**
         * @brief Default constructor for NetworkObject.
         *
         * This constructor initializes a new instance of a NetworkObject.
         */
		NetworkObject();

		/**
         * @brief Copy constructor for NetworkObject.
         *
         * This constructor initializes a new instance of a NetworkObject, copying the data from another instance.
         * @param p_other The other instance to copy data from.
         */
		NetworkObject(const NetworkObject& p_other);

		/**
         * @brief Move constructor for NetworkObject.
         *
         * This constructor initializes a new instance of a NetworkObject, moving the data from another instance.
         * @param p_other The other instance to move data from.
         */
		NetworkObject(NetworkObject&& p_other);

		/**
         * @brief Copy assignment operator for NetworkObject.
         *
         * This operator is used to copy the data from another NetworkObject into this instance.
         * @param p_other The other instance to copy data from.
         * @return A reference to this instance.
         */
		NetworkObject& operator=(const NetworkObject& p_other);

		/**
         * @brief Move assignment operator for NetworkObject.
         *
         * This operator is used to move the data from another NetworkObject into this instance.
         * @param p_other The other instance to move data from.
         * @return A reference to this instance.
         */
		NetworkObject& operator=(NetworkObject&& p_other);

		/**
         * @brief Destructor for NetworkObject.
         *
         * This destructor is responsible for releasing any resources the NetworkObject may hold.
         */
		~NetworkObject();
	};
}
