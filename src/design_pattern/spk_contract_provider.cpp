#include "design_pattern/spk_contract_provider.hpp"

namespace spk
{
		ContractProvider::Contract::Contract(CallbackContainer &p_callbackOwner, Callback &p_callback) :
			_callbackOwner(p_callbackOwner),
			_callback(p_callback),
			_isOriginal(true)
		{

		}

		bool ContractProvider::Contract::isOriginal()
		{
			return (_isOriginal == true);
		}

		ContractProvider::Contract::Contract(Contract &&p_other) :
			_callbackOwner(p_other._callbackOwner),
			_callback(p_other._callback),
			_isOriginal(true)
		{
			p_other._isOriginal = false;
		}

		ContractProvider::Contract& ContractProvider::Contract::operator=(Contract &&p_other) noexcept
		{
			if (this != &p_other)
			{
				resign();

				_isOriginal = p_other._isOriginal;
				_callbackOwner = std::move(p_other._callbackOwner);
				_callback = std::move(p_other._callback);

				p_other._isOriginal = false;
			}

			return *this;
		}

		ContractProvider::Contract::~Contract()
		{
			if (isOriginal() == true)
			{
				resign();
			}
		}

		void ContractProvider::Contract::edit(const Callback& p_callback)
		{
			if (isOriginal() == true)
			{
				_callback = p_callback;
			}
			else
			{
				throw std::runtime_error("Can't edit a resigned contract");
			}
		}

		void ContractProvider::Contract::resign()
		{
			if (isOriginal() == true)
			{
				_isOriginal = false;
				_callback = nullptr;
				for (auto it = _callbackOwner.begin(); it != _callbackOwner.end(); ++it)
				{
					if (&(*it) == &_callback)
					{
						_callbackOwner.erase(it);
						break;
					}
				}
			}
			else
			{
				throw std::runtime_error("Can't resign an already resigned contract");
			}
		}

		ContractProvider::Contract ContractProvider::subscribe(CallbackContainer& p_callbackOwner, const Callback& p_callback)
		{
			p_callbackOwner.push_back(p_callback);
			return (std::move(Contract(p_callbackOwner, p_callbackOwner.back())));
		}
}
