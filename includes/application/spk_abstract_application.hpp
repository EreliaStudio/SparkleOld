#pragma once

#include <map>
#include <vector>
#include <string>
#include <functional>
#include "threading/spk_persistent_worker.hpp"

namespace spk
{
	class AbstractApplication
	{
	private:
		std::map<std::wstring, spk::PersistentWorker *> _workers;
		std::vector<std::function<void()>> _jobs;

		int _errorCode;
		bool _isRunning;

	protected:
		void addJob(const std::wstring &p_WorkerName, std::function<void()> p_Job);
		void addJob(std::function<void()> p_Job);
		virtual void setupJobs() = 0;

	public:
		AbstractApplication();

		~AbstractApplication();

		int run();
		void quit(int p_errorCode);
	};
}