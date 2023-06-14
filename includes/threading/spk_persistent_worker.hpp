#pragma once
#include "threading/spk_thread.hpp"

namespace spk
{
	class PersistentWorker : public Thread
	{
	public:
		using Job = std::function<void()>;

	private:
		std::vector<Job> _jobs;
		bool _isRunning = false;

	public:
		PersistentWorker(const std::wstring & p_name);
		~PersistentWorker();

		void addJob(const Job& p_job);

		void start();
		void stop();
	};
}