#pragma once 

#include <map>
#include <vector>
#include "design_pattern/spk_contract_factory.hpp"

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
	class Observer : public ContractFactory
	{
	private:
		using ObserverRowContainer = std::deque<Callback*>;
		using ObserverMapContainer = std::map<TEvent, ObserverRowContainer>;
		ObserverMapContainer _callbackMap;

	public:
		~Observer()
		{

		}

		std::shared_ptr<Contract> subscribe(TEvent p_event, Callback p_callback)
		{
			std::shared_ptr<Contract> result = ContractFactory::subscribe(p_callback);

			Callback* callbackPtr = getContractCallbackAddress(result);
			_callbackMap[p_event].push_back(callbackPtr);
			setContractResignCallback(result, std::bind(
				[&](Callback* p_callbackToDelete, ObserverRowContainer* callbackArray){
					callbackArray->erase(std::remove(callbackArray->begin(), callbackArray->end(), p_callbackToDelete), callbackArray->end());
				},
				callbackPtr, &(_callbackMap[p_event]))
				);

			return (result);
		}

		void notify(TEvent p_event)
		{
			ObserverRowContainer& callbackArray = _callbackMap[p_event];

			for (size_t i = 0; i < callbackArray.size(); i++)
			{
				(*(callbackArray[i]))();
			}
		}

		ObserverRowContainer& container(TEvent p_event)
		{
			return (_callbackMap[p_event]);
		}

		ObserverMapContainer& mapContainer()
		{
			return (_callbackMap);	
		}
	};
}