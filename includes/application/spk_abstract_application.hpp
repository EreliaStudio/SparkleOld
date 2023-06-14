#pragma once

#include <map>
#include <vector>
#include "threading/spk_persistent_worker.hpp"

namespace spk
{
	class AbstractApplication
	{
	public:
		using Job = std::function<void()>;

	private:
		std::map<std::wstring, spk::PersistentWorker *> _workers;
		std::vector<Job> _jobs;

		int _errorCode;
		bool _isRunning;

	protected:
		void addJob(const std::wstring &p_WorkerName, const Job& p_job);
		void addJob(const Job& p_job);
		virtual void setupJobs() = 0;

	public:
		AbstractApplication();

		~AbstractApplication();

		int run();
		void quit(int p_errorCode);
	};
}