#pragma once

#include <map>
#include <vector>
#include "design_pattern/spk_contract_provider.hpp"

namespace spk
{
    /**
     * @brief Observer class template.
     *
     * This class template represents an observer that can subscribe to events and notify registered callbacks when the events occur.
     * @tparam TEvent The type of the event.
     */
    template <typename TEvent>
    class Observer : public ContractProvider
    {
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

        /**
         * @brief Subscribe to an event with a callback.
         *
         * This function subscribes to an event with the specified callback.
         * It adds the callback to the corresponding event in the callback map and returns a contract representing the subscription.
         *
         * @param p_event The event to subscribe to.
         * @param p_callback The callback function to be registered.
         * @return A contract representing the subscription.
         */
        Contract subscribe(TEvent p_event, const Callback& p_callback)
        {
            Contract result = ContractProvider::subscribe(_callbacks[p_event], p_callback);

            return std::move(result);
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
            CallbackContainer& container = _callbacks[p_event];

            for (size_t i = 0; i < container.size(); i++)
            {
                container[i]();
            }
        }
    };
}
