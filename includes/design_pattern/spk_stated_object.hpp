#pragma once

#include "design_pattern/spk_contract_provider.hpp"
#include <map>

namespace spk
{
    template <typename TState>
    class StatedObject : public ContractProvider
    {
    private:
        TState _state;
        std::map<TState, ContractProvider::CallbackContainer> _callbacks;

    public:
        StatedObject(TState p_defaultState = {}) :
            _state(p_defaultState)
        {

        }

        Contract addStateCallback(TState p_state, Callback p_callback)
        {
            Contract result = ContractProvider::subscribe(_callbacks[p_state], p_callback);

			return (std::move(result));
        }

        void setState(TState p_state)
        {
            _state = p_state;

            CallbackContainer &container = _callbacks[p_state];

			for (size_t i = 0; i < container.size(); i++)
			{
				container[i]();
			}
        }

        const TState& state() const {return (_state);}
    };
}