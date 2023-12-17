#pragma once
#include "design_pattern/spk_callback_container.hpp"
#include <map>

namespace spk
{
    /**
     * @brief A StateMachine class that handles different state transitions.
     * It allows setting callbacks for state transitions and state executions.
     *
     * @tparam TState The type of state this machine handles.
     */
    template <typename TState>
    class StateMachine
    {
    public:
        /**
         * @brief Alias for CallbackContainer::Callback.
         *
         * Callback is used to define the function signature for callback functions within the state machine.
         * It encapsulates a function that will be called during state transitions or state executions.
         */
        using Callback = CallbackContainer::Callback;

        /**
         * @brief Alias for CallbackContainer::Contract.
         *
         * Contract represents a handle or token that manages the lifecycle of a callback subscription
         * within the state machine. It allows for efficient management and potential removal of callbacks.
         */
        using Contract = CallbackContainer::Contract;

    private:
        /**
         * @brief Struct representing a key for the transitionCallbacks map.
         *
         * This struct is used as a key in the map that stores callbacks for specific state transitions.
         */
        struct Key
        {

            TState source;      /**< The source state of the transition. */
            TState destination; /**< The destination state of the transition. */
            bool operator<(const Key &p_other) const
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

        TState _state;                                                             /**< The current state of the state machine. */
        std::map<TState, CallbackContainer> _executionCallbacks; /**< Map storing callbacks for state executions. */
        std::map<Key, CallbackContainer> _transitionCallbacks;   /**< Map storing callbacks for state transitions. */
        CallbackContainer _unknownExecutionCallback;             /**< Callback container for unknown state executions. */
        CallbackContainer _unknownTransitionCallback;            /**< Callback container for unknown state transition. */

    public:
        /**
         * @brief Constructs a new StateMachine object.
         *
         * @param p_state Initial state of the state machine.
         */
        StateMachine(const TState &p_state) : _state(p_state)
        {
        }

        /**
         * @brief Execute the execution callbacks for the current state. The behavior of this method depends on the specific state machine implementation.
         */
        void update()
        {
            if (_executionCallbacks[_state].size() != 0)
                _executionCallbacks[_state].notify();
            else
                _unknownExecutionCallback.notify();
        }

        /**
         * @brief Changes the current state of the state machine and call the transition callback.
         *
         * @param p_state The new state.
         */
        void setState(const TState &p_state)
        {
            if (_transitionCallbacks[{_state, p_state}].size() != 0)
                _transitionCallbacks[{_state, p_state}].notify();
            else
                _unknownTransitionCallback.notify();

            _state = p_state;
        }

        /**
         * @brief Subscribes a callback for unknown state transitions.
         *
         * Registers a callback that is called when a state transition occurs to an undefined or unknown state.
         * This can be used to handle unexpected or erroneous state transitions.
         *
         * @param p_callback The callback function to be executed.
         * @return Contract A contract object managing the subscription of the callback.
         */
        Contract setOnUnknowStateTransition(const Callback &p_callback)
        {
            return (_unknownTransitionCallback.subscribe(p_callback));
        }

        /**
         * @brief Subscribes a callback for unknown state executions.
         *
         * Registers a callback that is called when the state machine attempts to execute an unknown state.
         * This is useful for handling or logging undefined state executions.
         *
         * @param p_callback The callback function to be executed.
         * @return Contract A contract object managing the subscription of the callback.
         */
        Contract setOnUnknowStateExecution(const Callback &p_callback)
        {
            return (_unknownExecutionCallback.subscribe(p_callback));
        }

        /**
         * @brief Sets a callback for a specific state transition.
         *
         * Registers a callback to be called when the state machine transitions from a specific initial state
         * to a target state. This allows for defining behavior or actions during certain state changes.
         *
         * @param p_initialState The initial state of the transition.
         * @param p_targetState The target state of the transition.
         * @param p_callback The callback function to be executed during the transition.
         * @return Contract A contract object managing the subscription of the callback.
         */
        Contract setStateTransitionCallback(const TState &p_initialState, const TState &p_targetState, const Callback &p_callback)
        {
            return (subscribe(_transitionCallbacks[{p_initialState, p_targetState}], p_callback));
        }

        /**
         * @brief Sets a callback for the execution of a specific state.
         *
         * Registers a callback to be called when a specific state is executed in the state machine.
         * This is useful for defining actions or behaviors that should occur when the state machine
         * is in a particular state.
         *
         * @param p_state The state for which the callback is to be executed.
         * @param p_callback The callback function to be executed.
         * @return Contract A contract object managing the subscription of the callback.
         */
        Contract setStateExecutionCallback(const TState &p_state, const Callback &p_callback)
        {
            return (subscribe(_executionCallbacks[p_state], p_callback));
        }
    };
}
