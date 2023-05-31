#pragma once 

#include <functional>
#include <deque>
#include <cstddef>

namespace spk
{
	/**
	 *  ContractableObject is a base class for objects that can be subscribed to events.
	 * 
	 * Role:
	 * Allow user to keep track of all subscriptions and resign from them.
	 * subscription are handling via contracts that can store callback and resign callback.
	 * Allow user to edit callback function.
	 * Allow user to inherit from this class and use it's functionality.
	 * 
	 * Warning:
	 * The owner of the contract is the ContractableObject that created it.
	 * If the owner is destroyed, all contracts will be destroyed as well.
	 * A user should not be able to instantiate a contract on his own.
	 */
	class ContractableObject
	{
	public:
		class Contract;

		using Callback = std::function<void()>;
		using Container = std::deque<Contract>;

		class Contract
		{
			friend class ContractableObject;

		private:
			ContractableObject *_owner;
			Callback _callback;
			Callback _resignCallback;

			Contract(ContractableObject *p_owner):
				_owner(p_owner),
				_callback(nullptr),
				_resignCallback(nullptr)
			{}

			Contract(ContractableObject *p_owner, Callback p_callback):
				_owner(p_owner),
				_callback(p_callback),
				_resignCallback(nullptr)
			{}

		public:

			Contract(const Contract& p_other) = delete;

			Contract(Contract&& p_other) noexcept
			{
				_owner = p_other._owner;
				_callback = std::move(p_other._callback);
				_resignCallback = std::move(p_other._resignCallback);
			}

			Contract& operator=(Contract&& p_other) noexcept
			{
				if (this != &p_other)
				{
					_owner = p_other._owner;
					_callback = std::move(p_other._callback);
					_resignCallback = std::move(p_other._resignCallback);
				}

				return *this;
			}

			~Contract()
			{
				resign();
			}

			void edit(Callback p_callback)
			{
				_callback = p_callback;
			}

			void resign()
			{
				_callback = nullptr;
				if (_resignCallback != nullptr)
				{
					_resignCallback();
					_resignCallback = nullptr;
				}
				if (_owner != nullptr)
					_owner->_removeContract(this);
			}
		};

	private:
		void _removeContract(Contract* p_contract)
		{
			for (size_t i = 0; i < _contracts.size(); ++i)
			{
				if (&_contracts[i] == p_contract)
				{
					p_contract->_owner = nullptr;
					_contracts.erase(_contracts.begin() + i);
					return;
				}
			}
		}

	protected:
		Container _contracts;

		void _executeContract(Contract& p_contract)
		{
			if (p_contract._callback != nullptr)
				p_contract._callback();
		}

		void _setResignCallback(Contract& p_contract, Callback p_callback)
		{
			p_contract._resignCallback = p_callback;
		}

		void _resignAll()
		{
			for (size_t i = 0; i < _contracts.size(); i++)
			{
				_contracts[i].resign();
			}
			_contracts.clear();
		}

	public:
		~ContractableObject()
		{}

		Contract& subscribe(Callback p_callback)
		{
			_contracts.push_back(Contract(this, p_callback));
			Contract& result = _contracts.back();
			return (result);
		}
	};
}
