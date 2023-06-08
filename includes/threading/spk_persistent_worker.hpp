#pragma once
#include "threading/spk_thread.hpp"

namespace spk
{
	class PersistentWorker : public Thread
	{
	private:
		std::vector<std::function<void()>> _jobs;
		bool _isRunning = false;

	public:
		PersistentWorker(const std::wstring & p_name);
		~PersistentWorker();

		void addJob(std::function<void()> p_job);

		void start();
		void stop();
	};
}