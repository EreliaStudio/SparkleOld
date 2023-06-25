#include "playground.hpp"

spk::Promise<int> promise;

void callback()
{
	spk::cout << "Callback called" << std::endl;
}

void promiseTests()
{
	std::this_thread::sleep_for(std::chrono::seconds(1));
	promise.setValue(42);
	try
	{
		promise.setValue(42);
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << '\n';
	}
}

int main()
{
	auto contract = promise.subscribe(callback);
	auto second_contract = promise.subscribe([]() { spk::cout << L"Second callback called" << std::endl; });

	spk::Thread thread(spk::Thread::LaunchMethod::Immediate, L"Promise test", promiseTests);
	contract.resign();

	spk::cout << L"Promise value: " << promise.value() << std::endl;
	spk::cout << L"Promise value: " << promise.value() << std::endl;
	return 0;
}