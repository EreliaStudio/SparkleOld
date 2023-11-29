#pragma once

#include "design_pattern/spk_contract_provider.hpp"
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
	class StatedObject : public ContractProvider
	{
	private:
		TState _state;
		std::map<TState, ContractProvider::CallbackContainer> _callbacks;

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
		 * @brief Setup callback for specific state.
		 * 
		 * This method will set a specific callback to a desired state.
		 * This method will provide a Contract to the user.
		 *
		 * @param p_state The state binded to the callback.
		 * @param p_callback The callback binded to the state.
		 * @return Contract A Contract representing the bind between state and callback, allowing him to edit/resign the callback added by this method.
		 */
		std::shared_ptr<Contract> addStateCallback(TState p_state, Callback p_callback)
		{
			return (ContractProvider::subscribe(_callbacks[p_state], p_callback));
		}

		/**
		 * @brief Set the object's State and call the binded function if have one.
		 * @param p_state The new state.
		 */
		void setState(TState p_state)
		{
			_state = p_state;
	
			CallbackContainer &container = _callbacks[p_state];

			for (size_t i = 0; i < container.size(); i++)
			{
					container[i]();
			}
		}
		
		/**
		 * @brief get the current object's state
		 * @return const TState& 
		 */
		const TState& state() const {return (_state);}
	};
}