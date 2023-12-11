#include "threading/spk_worker.hpp"

namespace spk
{
	Worker::Worker(const std::wstring& p_name) :
		_isRunning(false),
		Thread(p_name, [&]()
		{
			_isRunning = true;
			while (_isRunning == true)
			{
				_jobQueue.wait();
				if (_jobQueue.empty() == false)
				{
					auto job = _jobQueue.pop_front();
					job();
				}
			}
		})
	{

	}

	Worker::~Worker()
	{
		stop();
	}

	void Worker::stop()
	{
		_isRunning = false;
		_jobQueue.clear();
		join();
	}
}
