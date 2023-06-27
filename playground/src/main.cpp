#include "playground.hpp"

void timerTest()
{
	spk::Timer timer;

	timer.setDuration(1000);
	timer.start();
	std::this_thread::sleep_for(std::chrono::milliseconds(500));
	spk::cout << "Timer is running: " << timer.isRunning() << std::endl;
	std::this_thread::sleep_for(std::chrono::milliseconds(500));
	spk::cout << "Timer is running: " << timer.isRunning() << L" remaining: "
		<< timer.remainingDuration() << L'/' << timer.duration() << std::endl;
	if (timer.status() == spk::Timer::State::Timeout)
		spk::cout << "Timer timeout" << std::endl;
	timer.stop();
	if (timer.status() == spk::Timer::State::Idle)
		spk::cout << "Timer idling" << std::endl;
}

int main()
{
	spk::TimeModule time_module;
	spk::PersistentWorker worker(L"Time handler");

	auto contract = worker.addJob([&]() { time_module.update(); });
	worker.start();


	worker.stop();
	return 0;
}