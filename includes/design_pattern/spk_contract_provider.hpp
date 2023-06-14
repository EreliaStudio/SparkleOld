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

			Contract(CallbackContainer &p_callbackOwner, Callback &p_callback);
			Contract(const Contract &p_other) = delete;

			bool isOriginal();

		public:
			Contract(Contract &&p_other);

			~Contract();

			void edit(const Callback& p_callback);

			void resign();
		};

	private:
	protected:
		Contract subscribe(CallbackContainer& p_callbackOwner, const Callback& p_callback);
	};
}
