#include "playground.hpp"

int main()
{
	spk::Chronometer chrono;
	spk::TimeModule _timeModule;
	spk::PersistentWorker worker(L"TimeUpdater");

	auto contract = worker.addJob([&]()
	{
		_timeModule.update();
	});
	worker.start();
	chrono.start();
	std::this_thread::sleep_for(std::chrono::seconds(1));
	chrono.start();
	std::this_thread::sleep_for(std::chrono::seconds(1));

	spk::cout << "Chronometer: " << chrono.stop() << "ms" << std::endl;
	return 0;
}