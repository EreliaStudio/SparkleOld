#pragma once
#include "design_pattern/spk_contract_provider.hpp"
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
    class StateMachine : public spk::ContractProvider
    {
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
        std::map<TState, ContractProvider::CallbackContainer> _executionCallbacks; /**< Map storing callbacks for state executions. */
        std::map<Key, ContractProvider::CallbackContainer> _transitionCallbacks;   /**< Map storing callbacks for state transitions. */
        ContractProvider::CallbackContainer _unknownExecutionCallback;             /**< Callback container for unknown state executions. */
        ContractProvider::CallbackContainer _unknownTransitionCallback;            /**< Callback container for unknown state transition. */

        /**
         * @brief Calls the callbacks stored in the provided callback containers.
         *
         * @details If the known callback container is empty, the unknown callback container is used.
         * If both containers are empty, a runtime_error is thrown.
         *
         * @param p_known The callback container for known callbacks.
         * @param p_unknown The callback container for unknown callbacks.
         */
        void _callCallbacks(const ContractProvider::CallbackContainer &p_known, const ContractProvider::CallbackContainer &p_unknown)
        {
            const CallbackContainer *container = &p_known;
            if (container->size() == 0)
            {
                if (p_unknown.size() == 0)
                {
                    throw std::runtime_error("Unknown Callback is undefined and Callback is undefined");
                }
                container = &p_unknown;
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
        StateMachine(const TState &p_state) : _state(p_state)
        {
        }

        /**
         * @brief Execute the execution callbacks for the current state. The behavior of this method depends on the specific state machine implementation.
         */
        void update()
        {
            _callCallbacks(_executionCallbacks[_state], _unknownExecutionCallback);
        }

        /**
         * @brief Changes the current state of the state machine and call the transition callback.
         *
         * @param p_state The new state.
         */
        void setState(const TState &p_state)
        {
            _callCallbacks(_transitionCallbacks[{_state, p_state}], _unknownTransitionCallback);
            _state = p_state;
        }

        /**
         * @brief Sets a callback to be called when an unknown state transition occurs.
         *
         * @param p_callback The callback to be set.
         * @return Contract The Contract object representing the ownership of the callback.
         */
        std::shared_ptr<Contract> setOnUnknowStateTransition(const Callback &p_callback)
        {
            return (ContractProvider::subscribe(_unknownTransitionCallback, p_callback));
        }

        /**
         * @brief Sets a callback to be called when an unknown state execution occurs.
         *
         * @param p_callback The callback to be set.
         * @return Contract The Contract object representing the ownership of the callback.
         */
        std::shared_ptr<Contract> setOnUnknowStateExecution(const Callback &p_callback)
        {
            return (ContractProvider::subscribe(_unknownExecutionCallback, p_callback));
        }

        /**
         * @brief Sets a callback to be called when a specific state transition occurs.
         *
         * @param p_initialState The initial state.
         * @param p_targetState The target state.
         * @param p_callback The callback to be set.
         * @return Contract The Contract object representing the ownership of the callback.
         */
        std::shared_ptr<Contract> setStateTransitionCallback(const TState &p_initialState, const TState &p_targetState, const Callback &p_callback)
        {
            return (ContractProvider::subscribe(_transitionCallbacks[{p_initialState, p_targetState}], p_callback));
        }

        /**
         * @brief Sets a callback to be called when a specific state is executed.
         *
         * @param p_state The specific state.
         * @param p_callback The callback to be set.
         * @return Contract The Contract object representing the ownership of the callback.
         */
        std::shared_ptr<Contract> setStateExecutionCallback(const TState &p_state, const Callback &p_callback)
        {
            return (ContractProvider::subscribe(_executionCallbacks[p_state], p_callback));
        }
    };
}
