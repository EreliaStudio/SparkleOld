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
	spk::cout << L"Waited so far: " << chrono.duration() << L" (~1000)" << std::endl;
	std::this_thread::sleep_for(std::chrono::seconds(1));

	const long long &stop_time = chrono.stop();
	std::this_thread::sleep_for(std::chrono::seconds(1));
	spk::cout << L"Duration should be like stop_time: "
		<< chrono.duration() << L" == " << stop_time << L" (~2000)" << std::endl;

	chrono.stop();
	spk::cout << L"Duration should be like stop_time: "
		<< chrono.duration() << L" == " << stop_time << L" (~3000)" << std::endl;

	chrono.start();
	std::this_thread::sleep_for(std::chrono::seconds(1));
	spk::cout << L"Restarted chrono: " << chrono.stop() << L" (~1000)" << std::endl;
	return 0;
}