#pragma once

#include <functional>
#include <deque>
#include <cstddef>
#include <stdexcept>
#include <memory>

/**
 * @namespace spk
 * @brief Namespace for the Sparkle library components.
 */
namespace spk
{
    /**
     * @class CallbackContainer
     * @brief Manages a collection of callback functions.
     *
     * This class allows for the subscription of callback functions which can be triggered collectively. 
     * Callbacks are managed via Contracts, which control the lifecycle and amendments of the callbacks.
     */
    class CallbackContainer
    {
    public:
        /**
         * @brief Alias for a callback function.
         *
         * Callback is defined as a std::function<void()> type. It represents a function that takes no parameters
         * and returns void. This type of function is commonly used for event handling and notifications,
         * where a specific action is triggered without the need for additional information.
         */
        using Callback = std::function<void()>;

        /**
         * @brief Alias for the container that stores callback functions.
         *
         * Container is defined as a std::list of Callback types. This list is used to store multiple
         * callbacks that have been subscribed to a specific event or trigger. std::list is chosen for 
         * efficient insertion and removal of callbacks, which is a common operation for callback containers.
         */
        using Container = std::list<Callback>;

    private:
        Container _callbacks; /**< Container storing the callbacks. */

    public:
        /**
		 * @class Contract
		 * @brief Represents a subscription contract for a callback within the CallbackContainer.
		 *
		 * This class manages the lifecycle of a callback within the CallbackContainer. It allows for
		 * amendments to the callback and ensures proper cleanup upon destruction. This class follows
		 * the rule of five, disabling copy semantics and providing move semantics for efficient 
		 * resource management.
		 */
		class Contract {
			friend class CallbackContainer;

		private:
			Container::iterator _iterator; /**< Iterator pointing to the specific callback in the container. */
			CallbackContainer::Container* _container; /**< Pointer to the container holding the callback. */
			bool _isOwner; /**< Flag indicating if this contract is the owner (responsible for) the callback. */

			/**
			 * @brief Private constructor used by CallbackContainer to create a Contract.
			 * @param p_iterator Iterator to the callback in the container.
			 * @param p_container Pointer to the container holding the callback.
			 */
			Contract(Container::iterator p_iterator, CallbackContainer::Container* p_container);

		public:
			/**
			 * @brief Default constructor. Constructs an empty Contract.
			 */
			Contract();

			Contract(const Contract&) = delete;
			Contract& operator=(const Contract&) = delete;

			/**
			 * @brief Move constructor. Takes over management of another Contract's callback.
			 * @param other Contract to move from.
			 */
			Contract(Contract&& other) noexcept;

			/**
			 * @brief Move assignment operator. Takes over management of another Contract's callback.
			 * @param other Contract to move from.
			 * @return Reference to this Contract.
			 */
			Contract& operator=(Contract&& other) noexcept;

			/**
			 * @brief Destructor. Unsubscribes the callback from the container if this contract is the owner.
			 */
			~Contract();

			/**
			 * @brief Amends the callback associated with this contract.
			 * @param p_newCallback New callback function to replace the current one.
			 */
			void amend(std::function<void()> p_newCallback);

			/**
			 * @brief Resigns the contract, removing the callback from the container if this contract is the owner.
			 */
			void resign();

			/**
			 * @brief Force the execution of the contract
			*/
			void trigger();
		};

        /**
         * @brief Returns the number of callbacks currently subscribed.
         * @return Size of the callback container.
         */
        size_t size() const;

        /**
         * @brief Subscribes a new callback to the container.
         * @param p_callback The callback function to subscribe.
         * @return A contract object that manages the subscribed callback.
         */
        Contract subscribe(std::function<void()> p_callback);

        /**
         * @brief Notifies (executes) all subscribed callbacks.
         */
        void notify() const;
    };
}
