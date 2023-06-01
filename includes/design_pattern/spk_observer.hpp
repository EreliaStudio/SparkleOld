#pragma once

#include <map>
#include <vector>
#include "design_pattern/spk_contract_provider.hpp"

namespace spk
{
	/**
	 *  Observer is a class that can be used to subscribe to events.
	 *
	 * Role:
	 * Allow user to subscribe to events.
	 * Allow user to notify all subscribers of an event.
	 * Allow free handling of events' callbacks thanks to Contract.
	 * Release Contract memory as soon as the Contract is resigned.
	 */
	template <typename TEvent>
	class Observer : public ContractProvider
	{
	private:
		std::map<TEvent, CallbackContainer> _callbacks;

	public:
		Observer()
		{
		}

		Contract subscribe(TEvent p_event, Callback p_callback)
		{
			Contract result = ContractProvider::subscribe(_callbacks[p_event], p_callback);

			return (std::move(result));
		}

		void notify(TEvent p_event)
		{
			CallbackContainer &container = _callbacks[p_event];

			for (size_t i = 0; i < container.size(); i++)
			{
				container[i]();
			}
		}
	};
}