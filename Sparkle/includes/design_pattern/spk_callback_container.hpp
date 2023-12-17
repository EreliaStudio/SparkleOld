#pragma once

#include <functional>
#include <deque>
#include <cstddef>
#include <stdexcept>
#include <memory>

namespace spk
{
	class CallbackContainer
	{
	public:
		using Callback = std::function<void()>;
		using Container = std::list<Callback>;

	private:
		Container _callbacks;

	public:
		class Contract {
			friend class CallbackContainer;

		private:
			Container::iterator _iterator;
			CallbackContainer::Container* _container;
			bool _isOwner;

			Contract(Container::iterator p_iterator, CallbackContainer::Container* p_container);

		public:
			Contract();

			Contract(const Contract&) = delete;
			Contract& operator=(const Contract&) = delete;

			Contract(Contract&& other) noexcept;
			Contract& operator=(Contract&& other) noexcept;

			~Contract();

			void amend(std::function<void()> p_newCallback);
			void resign();
		};

		size_t size() const;
		Contract subscribe(std::function<void()> p_callback);
		void notify() const;
	};
}
