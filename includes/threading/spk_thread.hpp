#pragma once

#include <thread>
#include <functional>
#include <string>
#include <future>

#include "iostream/spk_iostream.hpp"
#include "debug/spk_profiler.hpp"

namespace spk
{
	/**
	 * @class Thread
	 * @brief Class for creating and managing a thread.
	 * 
	 * This class provides a convenient way to create and manage a thread. 
	 * It allows you to specify the thread function and its arguments, 
	 * as well as the launch method (immediate or delayed).
	 */
	class Thread
	{
	public:
		/**
		 * @enum LaunchMethod
		 * @brief An enum class to represent the launch method of the thread.
		 * 
		 * This enum class has two launch methods: Delayed and Immediate. 
		 * The Delayed method creates the thread but does not start it immediately. 
		 * The Immediate method creates and starts the thread immediately.
		 */
		enum class LaunchMethod
		{
			Delayed,
			Immediate
		};

	private:
		std::function<void()> _funct;
		std::thread _thread;
		std::promise<void> _starterSignal;

	public:
		/**
		 * @brief Construct a new Thread object with a specified thread name and function.
		 * 
		 * This constructor creates a new thread with the specified thread name and function. 
		 * The function can take any number of arguments.
		 * 
		 * @tparam Funct The type of the thread function.
		 * @tparam Args The types of the arguments of the thread function.
		 * @param p_threadName The name of the thread.
		 * @param p_funct The thread function.
		 * @param p_args The arguments of the thread function.
		 */
		template <typename Funct, typename... Args>
		Thread(const std::wstring& p_threadName, Funct &&p_funct, Args &&...p_args) :
			_funct(std::bind(std::forward<Funct>(p_funct), std::forward<Args>(p_args)...)),
			_starterSignal()
		{
			auto wrapper = [&](const std::wstring& threadName)
			{
				spk::cout.setPrefix(threadName);
				spk::cerr.setPrefix(threadName);
				spk::Profiler::instance()->defineThreadName(threadName);
				_starterSignal.get_future().wait();
				_funct();
			};
			_thread = std::thread(wrapper, p_threadName);
		}

		/**
		 * @brief Construct a new Thread object with a specified launch method, thread name, and function.
		 * 
		 * This constructor creates a new thread with the specified launch method, thread name, and function. 
		 * The function can take any number of arguments. If the launch method is Immediate, the thread is started immediately.
		 * 
		 * @tparam Funct The type of the thread function.
		 * @tparam Args The types of the arguments of the thread function.
		 * @param p_launchMethod The launch method of the thread.
		 * @param p_threadName The name of the thread.
		 * @param p_funct The thread function.
		 * @param p_args The arguments of the thread function.
		 */
		template <typename Funct, typename... Args>
		Thread(LaunchMethod p_launchMethod, std::wstring p_threadName, Funct &&p_funct, Args &&...p_args) :
			Thread(p_threadName, p_funct, p_args...)
		{
			if (p_launchMethod == LaunchMethod::Immediate)
			{
				start();
			}
		}

		/**
		 * @brief Destroy the Thread object.
		 * 
		 * This is the destructor for the Thread class. 
		 * It joins the thread if its still running.
		 */
		~Thread();

		/**
		 * @brief Get the ID of the thread.
		 * 
		 * This function returns the ID of the thread.
		 * 
		 * @return The ID of the thread.
		 */
		std::thread::id getId() const
		{
			return (_thread.get_id());
		}

		/**
		 * @brief Join the thread.
		 * 
		 * This function blocks the current thread until the associated thread finishes its execution.
		 */
		void join();

		/**
		 * @brief Start the thread.
		 * 
		 * This function starts the thread by releasing the starter signal. 
		 * The thread function will begin executing once this signal is released.
		 */
		void start();
	};
}
