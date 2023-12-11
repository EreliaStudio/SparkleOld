#pragma once

#include <mutex>
#include <deque>
#include <condition_variable>

namespace spk
{
	/**
	 * @class ThreadSafeQueue
	 * @brief A thread-safe queue implementation.
	 *
	 * This class provides a thread-safe implementation of a queue. It allows multiple threads
	 * to push and pop items from the queue without causing data races or deadlocks.
	 *
	 * @tparam TType The type of items stored in the queue.
	 */
	template <typename TType>
	class ThreadSafeQueue
	{
	protected:
		std::mutex muxQueue;	/**< Mutex for protecting the access to the queue. */
		std::mutex muxBlocking; /**< Mutex for blocking thread synchronization. */

		std::deque<TType> _content;				/**< The underlying deque container storing the items in the queue. */
		std::condition_variable cvBlocking; /**< Condition variable for blocking thread synchronization. */

	public:
		/**
		 * @brief Construct a new ThreadSafeQueue object.
		 *
		 * This is the default constructor for the ThreadSafeQueue class.
		 */
		ThreadSafeQueue() = default;

		/**
		 * @brief Copy constructor is deleted.
		 *
		 * Copying a ThreadSafeQueue is not allowed to avoid data races.
		 */
		ThreadSafeQueue(const ThreadSafeQueue<TType> &) = delete;

		/**
		 * @brief Destroy the ThreadSafeQueue object.
		 *
		 * This is the destructor for the ThreadSafeQueue class. It clears the content of the queue.
		 */
		virtual ~ThreadSafeQueue() { clear(); }

		/**
		 * @brief Access an item in the queue by index.
		 *
		 * This operator allows accessing an item in the queue by index.
		 *
		 * @param p_index The index of the item.
		 * @return The item at the specified index.
		 */
		const TType &operator[](const size_t &p_index) const
		{
			return (_content[p_index]);
		}

		/**
		 * @brief Get the reference to the front item in the queue.
		 *
		 * This function returns a reference to the front item in the queue.
		 *
		 * @return Reference to the front item.
		 */
		TType &front()
		{
			std::scoped_lock lock(muxQueue);
			return _content.front();
		}

		/**
		 * @brief Get the reference to the back item in the queue.
		 *
		 * This function returns a reference to the back item in the queue.
		 *
		 * @return Reference to the back item.
		 */
		TType &back()
		{
			std::scoped_lock lock(muxQueue);
			return _content.back();
		}

		/**
		 * @brief Get the const reference to the front item in the queue.
		 *
		 * This function returns a const reference to the front item in the queue.
		 *
		 * @return Const reference to the front item.
		 */
		const TType &front() const
		{
			std::scoped_lock lock(muxQueue);
			return _content.front();
		}

		/**
		 * @brief Get the const reference to the back item in the queue.
		 *
		 * This function returns a const reference to the back item in the queue.
		 *
		 * @return Const reference to the back item.
		 */
		const TType &back() const
		{
			std::scoped_lock lock(muxQueue);
			return _content.back();
		}

		/**
		 * @brief Remove and return the front item from the queue.
		 *
		 * This function removes and returns the front item from the queue.
		 *
		 * @return The front item.
		 */
		TType pop_front()
		{
			std::scoped_lock lock(muxQueue);
			auto t = std::move(_content.front());
			_content.pop_front();
			return (std::move(t));
		}

		/**
		 * @brief Remove and return the back item from the queue.
		 *
		 * This function removes and returns the back item from the queue.
		 *
		 * @return The back item.
		 */
		TType pop_back()
		{
			std::scoped_lock lock(muxQueue);
			auto t = std::move(_content.back());
			_content.pop_back();
			return (std::move(t));
		}

		/**
		 * @brief Push an item to the back of the queue.
		 *
		 * This function pushes an item to the back of the queue.
		 *
		 * @param item The item to be pushed.
		 */
		void push_back(TType &&item)
		{
			std::scoped_lock lock(muxQueue);
			_content.emplace_back(std::move(item));

			std::unique_lock<std::mutex> ul(muxBlocking);
			cvBlocking.notify_one();
		}

		/**
		 * @brief Push an item to the front of the queue.
		 *
		 * This function pushes an item to the front of the queue.
		 *
		 * @param item The item to be pushed.
		 */
		void push_front(TType &&item)
		{
			std::scoped_lock lock(muxQueue);
			_content.emplace_front(std::move(item));

			std::unique_lock<std::mutex> ul(muxBlocking);
			cvBlocking.notify_one();
		}

		/**
		 * @brief Check if the queue is empty.
		 *
		 * This function checks if the queue is empty.
		 *
		 * @return True if the queue is empty, false otherwise.
		 */
		bool empty()
		{
			return _content.empty();
		}

		/**
		 * @brief Get the size of the queue.
		 *
		 * This function returns the size of the queue.
		 *
		 * @return The size of the queue.
		 */
		size_t size()
		{
			return _content.size();
		}

		/**
		 * @brief Clear the queue.
		 *
		 * This function clears the content of the queue.
		 */
		void clear()
		{
			std::scoped_lock lock(muxQueue);
			_content.clear();
		}

		/**
		 * @brief Wait for the queue to become non-empty.
		 *
		 * This function waits until the queue becomes non-empty.
		 *
		 */
		void wait()
		{
			while (empty())
			{
				std::unique_lock<std::mutex> ul(muxBlocking);
				cvBlocking.wait(ul);
			}
		}
	};
}
