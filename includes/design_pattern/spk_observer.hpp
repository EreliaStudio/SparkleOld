#pragma once 

#include <map>
#include <vector>
#include "design_pattern/spk_contractable_object.hpp"

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
	template<typename TEvent>
	class Observer : public ContractableObject
	{
	private:
		std::map<TEvent, std::vector<Contract*>> _callbackMap;

	public:
		~Observer()
		{
			_resignAll();
		}

		Contract& subscribe(TEvent p_event, Callback p_callback)
		{
			Contract& result = ContractableObject::subscribe(p_callback);

			_setResignCallback(result, 
				std::bind([&](Contract* p_contract, std::vector<Contract*>* contener){
					auto tmp = std::find(contener->begin(), contener->end(), p_contract);

					if (tmp != contener->end())
						contener->erase(tmp);
				}, &result, &_callbackMap[p_event]));

			_callbackMap[p_event].push_back(&result);
			return (result);
		}

		void notify(TEvent p_event)
		{
			std::vector<Contract*>& tmpArray = _callbackMap[p_event];

			for (size_t i = 0; i < tmpArray.size(); i++)
				_executeContract(*tmpArray[i]);
		}
	};
}