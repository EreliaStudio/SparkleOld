#pragma once

#include "design_pattern/spk_callback_container.hpp"
#include <map>

namespace spk
{
	/**
	 * @brief A class who manage state switching.
	 * This class representing an object who can be in different state.
	 * the class additionnaly store lambdas to call uppon specific state.
	 * Each lambda is provided to the user as Contract object
	 *
	 * @tparam TState The status class/structure/enum/type in which the object can be set
	 */
	template <typename TState>
	class StatedObject
	{
	public:
		/**
         * @brief Alias for CallbackContainer::Contract.
         *
         * Contract is used as a handle for managing the lifecycle of a callback subscription.
         * When a callback is added to this object, a Contract instance is returned, which can be
         * used to modify or remove the callback later. This simplifies the management of callback
         * subscriptions and enhances code readability.
         */
        using Contract = CallbackContainer::Contract;

        /**
         * @brief Alias for CallbackContainer::Callback.
         *
         * Callback is used to define the function signature for callback functions. When a callback is
         * registered with this object, it must conform to this signature. This ensures consistency and
         * allows for easy integration of callbacks into the object's event handling system. Using an alias
         * enhances readability and makes it easier to change the callback signature in the future if needed.
         */
        using Callback = CallbackContainer::Callback;

	private:
		TState _state;
		std::map<TState, CallbackContainer> _callbacks;

	public:
		/**
		 * @brief Construct a new Stated Object
		 * 
		 * @param p_defaultState Represent the default state of the object.
		 */
		StatedObject(TState p_defaultState) :
			_state(p_defaultState)
		{

		}

	    /**
		 * @brief Adds a callback to be invoked when the object enters a specific state.
		 *
		 * This method allows for the registration of a callback function that will be called whenever
		 * the object transitions to a specific state. This functionality is crucial for handling state-dependent
		 * actions or behaviors. When the object's state changes to the specified state, the callback is executed.
		 *
		 * @param p_state The state upon entering which the callback should be invoked. This parameter should
		 *                be of the type TState, which represents the various states the object can be in.
		 * @param p_callback A function to be called when the object enters the specified state. The function
		 *                   should match the signature defined in CallbackContainer::Callback.
		 * @return Contract A contract object that represents the registration of the callback. This contract
		 *         can be used for later management of the callback, such as removing or modifying it.
		 */
		Contract addStateCallback(TState p_state, Callback p_callback)
		{
			return (std::move(_callbacks[p_state].subscribe(p_callback)));
		}

		/**
		 * @brief Set the object's State and call the binded function if have one.
		 * @param p_state The new state.
		 */
		void setState(TState p_state)
		{
			_state = p_state;
	
			_callbacks[p_state].notify();
		}
		
		/**
		 * @brief get the current object's state
		 * @return const TState& 
		 */
		const TState& state() const {return (_state);}
	};
}