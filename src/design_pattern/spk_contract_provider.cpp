#include "design_pattern/spk_contract_provider.hpp"
#include "iostream/spk_iostream.hpp"

namespace spk
{
		ContractProvider::Contract::Contract() :
			_callbackOwner(nullptr),
			_callback(nullptr),
			_isOriginal(false)
		{

		
		}
		
		ContractProvider::Contract::Contract(CallbackContainer *p_callbackOwner, Callback *p_callback) :
			_callbackOwner(p_callbackOwner),
			_callback(p_callback),
			_isOriginal(true)
		{

		}

		bool ContractProvider::Contract::isOriginal()
		{
			return (_isOriginal == true);
		}

		bool ContractProvider::Contract::isValid()
		{
			return (_callbackOwner != nullptr);
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
				if (isOriginal() == true && isValid() == true)
				{
					resign();
				}

				_isOriginal = p_other._isOriginal;
				_callbackOwner = p_other._callbackOwner;
				_callback = p_other._callback;

				p_other._isOriginal = false;
			}

			return *this;
		}

		ContractProvider::Contract::~Contract()
		{
			if (isOriginal() == true && isValid() == true)
			{
				resign();
			}
		}

		void ContractProvider::Contract::edit(const Callback& p_callback)
		{
			if (isOriginal() == true && isValid() == true)
			{
				*_callback = p_callback;
			}
			else
			{
				throw std::runtime_error("Can't edit a resigned contract");
			}
		}

		void ContractProvider::Contract::resign()
		{
			if (isOriginal() == true && isValid() == true)
			{
				_isOriginal = false;
				if (isValid() == true)
				{
					_callback = nullptr;
					for (auto it = _callbackOwner->begin(); it != _callbackOwner->end(); ++it)
					{
						if (&(*it) == _callback)
						{
							_callbackOwner->erase(it);
							break;
						}
					}	
				}				
			}
			else
			{
				throw std::runtime_error("Can't resign an already resigned contract");
			}
		}

		std::shared_ptr<ContractProvider::Contract> ContractProvider::subscribe(CallbackContainer& p_callbackOwner, const Callback& p_callback)
		{
			p_callbackOwner.push_back(p_callback);
			std::shared_ptr<ContractProvider::Contract> result = std::shared_ptr<ContractProvider::Contract>(
					new ContractProvider::Contract(&p_callbackOwner, &(p_callbackOwner.back()))
				);
			_contracts.push_back(result);
			return (result);
		}

		ContractProvider::~ContractProvider()
		{
			for (size_t i = 0; i < _contracts.size(); i++)
			{
				_contracts[i]->_callbackOwner = nullptr;
			}
		}
}
