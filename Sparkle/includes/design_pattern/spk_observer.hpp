#pragma once

#include <map>
#include <vector>
#include "design_pattern/spk_callback_container.hpp"

namespace spk
{
	/**
	 * @brief Observer class template.
	 *
	 * This class template represents an observer that can subscribe to events and notify registered callbacks when the events occur.
	 * @tparam TEvent The type of the event.
	 */
	template <typename TEvent>
	class Observer
	{
	public:
		using Contract = CallbackContainer::Contract;
		using Callback = CallbackContainer::Callback;

	private:
		std::map<TEvent, CallbackContainer> _callbacks; /**< The map of keys/values events/callbacks. */

	public:
		/**
		 * @brief Default constructor for Observer.
		 *
		 * This constructor initializes the observer.
		 */
		Observer()
		{
		}

		CallbackContainer::Contract subscribe(TEvent p_event, const Callback& p_callback)
		{
			return (_callbacks[p_event].subscribe(p_callback));
		}

		/**
		 * @brief Notify the observers of an event.
		 *
		 * This function notifies all registered callbacks for the specified event.
		 * It retrieves the callback container for the event and calls each callback function in the container.
		 *
		 * @param p_event The event to notify.
		 */
		void notify(TEvent p_event)
		{
			_callbacks[p_event].notify();
		}
	};
}
