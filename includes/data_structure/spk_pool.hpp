#pragma once
#include <deque>
#include <memory>
#include <functional>
#include <mutex>

namespace spk
{
	template<typename TType>
	class Pool {
	public:
		using Destructor = typename std::function<void(TType* p_toReturn)>;
		using Object = typename std::unique_ptr<TType, Destructor>;

	private:
		std::recursive_mutex _mutex;
		std::deque<std::unique_ptr<TType>> _preallocatedElements;

		const Destructor _destructorLambda = [&](TType* p_toReturn){
			_release(p_toReturn);
		};

		void _release(TType* p_toReturn)
		{
			std::lock_guard<std::recursive_mutex> lock(_mutex);
			_preallocatedElements.push_back(std::unique_ptr<TType>(p_toReturn));
		}

	public:
		Pool(const size_t& p_poolSize = 0)
		{
			resize(p_poolSize);
		}

		void resize(size_t p_newSize) {
			std::lock_guard<std::recursive_mutex> lock(_mutex);
			while (_preallocatedElements.size() > p_newSize) {
				_preallocatedElements.pop_back();
			}
			while (_preallocatedElements.size() < p_newSize) {
				_preallocatedElements.push_front(std::make_unique<TType>());
			}
		}

		Object obtain() {
			std::lock_guard<std::recursive_mutex> lock(_mutex);
			
			if (_preallocatedElements.empty())
			{
				_preallocatedElements.push_front(std::make_unique<TType>());
			}

			Object item(_preallocatedElements.front().release(), _destructorLambda);

			_preallocatedElements.pop_front();

			return std::move(item);
		}
	};
}