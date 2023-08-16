#pragma once

#include "spk_system_define.hpp"
#include "spk_basic_functions.hpp"

namespace spk::Network
{
    /**
     * @class Object
     * @brief Base class for objects that utilize network resources.
     *
     * The Object class encapsulates the basic behaviors common to network objects, such as socket management.
     * This class is intended to be a base class for other network-related classes, such as sockets, acceptors, etc.
     */
	class Object
	{
	private:
		static inline std::recursive_mutex _mutex; /**< Mutex for thread safety */
		static inline size_t _nbElement = 0; /**< Counter for the number of Object instances */
		static inline WSADATA _wsaData; /**< Windows Sockets API data */

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
         * @brief Increments the count of Object instances.
         *
         * This function increments the count of Object instances, typically when a new instance is created.
         */
		static void _incrementCount();

		/**
         * @brief Decrements the count of Object instances.
         *
         * This function decrements the count of Object instances, typically when an instance is destroyed.
         */
		static void _decrementCount();

	public:
		/**
         * @brief Default constructor for Object.
         *
         * This constructor initializes a new instance of a Object.
         */
		Object();

		/**
         * @brief Copy constructor for Object.
         *
         * This constructor initializes a new instance of a Object, copying the data from another instance.
         * @param p_other The other instance to copy data from.
         */
		Object(const Object& p_other);

		/**
         * @brief Move constructor for Object.
         *
         * This constructor initializes a new instance of a Object, moving the data from another instance.
         * @param p_other The other instance to move data from.
         */
		Object(Object&& p_other);

		/**
         * @brief Copy assignment operator for Object.
         *
         * This operator is used to copy the data from another Object into this instance.
         * @param p_other The other instance to copy data from.
         * @return A reference to this instance.
         */
		Object& operator=(const Object& p_other);

		/**
         * @brief Move assignment operator for Object.
         *
         * This operator is used to move the data from another Object into this instance.
         * @param p_other The other instance to move data from.
         * @return A reference to this instance.
         */
		Object& operator=(Object&& p_other);

		/**
         * @brief Destructor for Object.
         *
         * This destructor is responsible for releasing any resources the Object may hold.
         */
		~Object();
	};
}
