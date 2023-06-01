#pragma once 

#include <functional>
#include <deque>
#include <cstddef>

#include <iostream>
#define DEBUG_LINE() std::cout << __FUNCTION__ << "::" << __LINE__ << std::endl

namespace spk
{
	class ContractFactory
	{
	public:
		using Callback = std::function<void()>;
		using Container = std::deque<Callback>;

		class Contract
		{
			friend class ContractFactory;
			friend class std::shared_ptr<Contract>;
		private:
			ContractFactory* _owner;
			Callback& _callback;
			Callback _resignCallback;

			Contract(ContractFactory* p_owner, Callback& p_callback) :
				_owner(p_owner),
				_callback(p_callback),
				_resignCallback(nullptr)
			{
				if (_callback == nullptr)
					throw std::runtime_error("Can't create a contract with no callback");
			}

		public:
			~Contract()
			{
				if (_callback != nullptr)
					resign();
			}

			void edit(Callback p_callback)
			{
				if (_callback == nullptr)
					throw std::runtime_error("Can't edit a resigned contract");

				if (p_callback == nullptr)
					throw std::runtime_error("Can't set a contract to nullptr");

				_callback = p_callback;
			}

			void resign()
			{
				if (_callback == nullptr)
					throw std::runtime_error("Can't edit a resigned contract");
				
				if (_resignCallback != nullptr)
					_resignCallback();

				_owner->_removeUnusedCallback(*this);

				_callback = nullptr;
			}
		};

	private:
		Container _callbacks;

		void _removeUnusedCallback(Contract& p_contract)
		{
			Callback* toDelete = &(p_contract._callback);

			for (size_t i = 0; i < _callbacks.size(); i++)
			{
				if (&_callbacks[i] == &p_contract._callback)
				{
					_callbacks.erase(_callbacks.begin() + i);
					return ;
				}
			}
		}

	protected:
		std::shared_ptr<Contract> subscribe(Callback p_callback)
		{
			_callbacks.push_back(p_callback);

			std::shared_ptr<Contract> result = std::shared_ptr<Contract>(new Contract(this, _callbacks.back()));

			return (result);
		}

		Callback* getContractCallbackAddress(const std::shared_ptr<Contract>& p_contract)
		{
			return (&(p_contract->_callback));
		}

		void setContractResignCallback(std::shared_ptr<Contract> p_contract, Callback p_callback)
		{
			p_contract->_resignCallback = p_callback;
		}

		void executeContract(const std::shared_ptr<Contract>& p_contract)
		{
			p_contract->_callback();
		}

public:
		Container& container()
		{
			return (_callbacks);
		}
	};
}
