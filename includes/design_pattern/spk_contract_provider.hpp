#pragma once

#include <functional>
#include <deque>
#include <cstddef>
#include <stdexcept>

namespace spk
{
	class ContractProvider
	{
	public:
		using Callback = std::function<void()>;
		using CallbackContainer = std::deque<Callback>;

		class Contract
		{
			friend class ContractProvider;

		private:
			bool _isOriginal = true;
			CallbackContainer &_callbackOwner;
			Callback &_callback;

			Contract(CallbackContainer &p_callbackOwner, Callback &p_callback) :
				_callbackOwner(p_callbackOwner),
				_callback(p_callback),
				_isOriginal(true)
			{

			}

			Contract(const Contract &p_other) = delete;

			bool isOriginal()
			{
				return (_isOriginal == true);
			}

		public:
			Contract(Contract &&p_other) :
				_callbackOwner(p_other._callbackOwner),
				_callback(p_other._callback),
				_isOriginal(true)
			{
				p_other._isOriginal = false;
			}

			~Contract()
			{
				if (isOriginal() == true)
				{
					resign();
				}
			}

			void edit(Callback p_callback)
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

			void resign()
			{
				if (isOriginal() == true)
				{
					_isOriginal = false;
					_callback = nullptr;
					_callbackOwner.erase(std::remove_if(_callbackOwner.begin(), _callbackOwner.end(),
														[&](const auto &callback)
														{ return &callback == &_callback; }),
										 _callbackOwner.end());
				}
				else
				{
					throw std::runtime_error("Can't resign an already resigned contract");
				}
			}
		};

	private:
	protected:
		Contract subscribe(CallbackContainer &p_callbackOwner, Callback &p_callback)
		{
			p_callbackOwner.push_back(p_callback);
			return (std::move(Contract(p_callbackOwner, p_callbackOwner.back())));
		}
	};
}
