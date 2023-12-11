#pragma once

#include "threading/spk_thread.hpp"
#include <future>

namespace spk
{
	class PersistentWorker : public Thread
	{
	public:
		using Job = std::function<void()>;
		
	private:
		bool _isRunning;
		std::promise<void> _starterSignal;

		void _runJob(const Job& p_job);

	public:
		PersistentWorker(const std::wstring& p_threadName, const Job& p_job);

		~PersistentWorker();

		void start();
		void stop();
	};
}
