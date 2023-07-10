#pragma once
#include "design_pattern/spk_contract_provider.hpp"
#include <map>
#include <vector>

/**
 * @brief A StateMachine class that handles different state transitions.
 * It allows setting callbacks for state transitions and state executions.
 * 
 * @tparam TState The type of state this machine handles.
 */
template <typename TState>
class StateMachine : public spk::ContractProvider
{
private:


    struct Key {
        TState source;       
        TState destination;       
        bool operator<(const Key& p_other) const
        {
            if (source < p_other.source)
            {
                return (true);
            }
            if (source == p_other.source && destination < p_other.destination)
            {
                return (true);
            }
            return (false);
        }
    };

    TState _state;
    std::map<TState, ContractProvider::CallbackContainer> _executionCallbacks;
    std::map<Key , ContractProvider::CallbackContainer> _transitionCallbacks;
    ContractProvider::CallbackContainer _unknowExecutionCallback;
    ContractProvider::CallbackContainer _unknowTransitionCallback;


    void callCallbacks(ContractProvider::CallbackContainer p_know, ContractProvider::CallbackContainer p_unknow)
    {
        CallbackContainer *container = &p_know;
        if (container->size() == 0)
        { 
            if (p_unknow.size() == 0)
            {
				throw std::runtime_error("Unknow Callback is undefined and Callback is undefined");
            }
            container = &p_unknow;
        }
        for (size_t i = 0; i < container->size(); i++)
        {
                (*container)[i]();
        }
    }

public:
    /**
     * @brief Constructs a new StateMachine object.
     *
     * @param p_state Initial state of the state machine.
     */
    StateMachine(const TState& p_state) : _state(p_state)
    {}

    /**
     * @brief Update the state machine. The behavior of this method depends on the specific state machine implementation.
     */
    void update()
    {
        // execution
        callCallbacks(_executionCallbacks[_state], _unknowExecutionCallback);
    }
    
    /**
     * @brief Changes the current state of the state machine.
     *
     * @param p_state The new state.
     */
    void setState(const TState& p_state)
    {
        callCallbacks(_transitionCallbacks[{_state, p_state}], _unknowTransitionCallback);
        _state = p_state;
    }

    /**
     * @brief Sets a callback to be called when an unknown state transition occurs.
     *
     * @param p_callback The callback to be set.
     * @return Contract The Contract object representing the ownership of the callback.
     */
    Contract setOnUnknowStateTransition(const Callback& p_callback)
    {
        return(std::move(ContractProvider::subscribe(_unknowTransitionCallback, p_callback)));
    }

    /**
     * @brief Sets a callback to be called when an unknown state execution occurs.
     *
     * @param p_callback The callback to be set.
     * @return Contract The Contract object representing the ownership of the callback.
     */
    Contract setOnUnknowStateExecution(const Callback& p_callback)
    {
        return(std::move(ContractProvider::subscribe(_unknowExecutionCallback, p_callback)));
    }

    /**
     * @brief Sets a callback to be called when a specific state transition occurs.
     *
     * @param p_initialState The initial state.
     * @param p_targetState The target state.
     * @param p_callback The callback to be set.
     * @return Contract The Contract object representing the ownership of the callback.
     */
    Contract setStateTransitionCallback(const TState& p_initialState, const TState& p_targetState, const Callback& p_callback)
    {
        return(std::move(ContractProvider::subscribe(_transitionCallbacks[{p_initialState, p_targetState}], p_callback)));
    }

    /**
     * @brief Sets a callback to be called when a specific state is executed.
     *
     * @param p_state The specific state.
     * @param p_callback The callback to be set.
     * @return Contract The Contract object representing the ownership of the callback.
     */
    Contract setStateExecutionCallback(const TState& p_state, const Callback& p_callback)
    {
        return(std::move(ContractProvider::subscribe(_executionCallbacks[p_state], p_callback)));
    }
    
};