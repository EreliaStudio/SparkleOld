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

		CallbackContainer::Contract addStateCallback(TState p_state, CallbackContainer::Callback p_callback)
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