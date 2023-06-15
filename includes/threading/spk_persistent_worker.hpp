#pragma once
#include "threading/spk_thread.hpp"
#include "design_pattern/spk_contract_provider.hpp"

namespace spk
{
	class PersistentWorker : public Thread, public ContractProvider
	{
	public:
		using Job = ContractProvider::Callback;

	private:
		CallbackContainer _jobs;
		bool _isRunning = false;
		bool _isPaused = false;

	public:
		PersistentWorker(const std::wstring & p_name);
		~PersistentWorker();

		Contract addJob(const Job& p_job);

		constexpr bool isRunning() const { return (_isRunning); }
		void start();
		void stop();

		void join() = delete;

		void pause();
		void resume();
	};
}