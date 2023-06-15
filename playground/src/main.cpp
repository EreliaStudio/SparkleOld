#include "playground.hpp"
spk::PersistentWorker worker(L"Worker");

spk::ContractProvider::Contract addAJob(const std::wstring& to_print)
{
	return worker.addJob([to_print]() {
		spk::cout << to_print << std::endl;
	});
}

void resignAndWait(spk::ContractProvider::Contract&& contract)
{
	contract.resign();
	std::this_thread::sleep_for(std::chrono::seconds(1));
}

int main()
{
	spk::ContractProvider::Contract first_contract = addAJob(L"First");
	spk::ContractProvider::Contract second_contract = addAJob(L"Second");
	spk::ContractProvider::Contract third_contract = addAJob(L"Third");

	if (worker.isRunning() == false)
		worker.start();

	std::this_thread::sleep_for(std::chrono::seconds(1));

	spk::cout << "Resigning first contract" << std::endl;
	resignAndWait(std::move(first_contract));

	worker.pause();
	spk::cout << "Not much should happen now" << std::endl;
	std::this_thread::sleep_for(std::chrono::seconds(1));
	spk::cout << "Okay, back at it" << std::endl;
	worker.resume();
	std::this_thread::sleep_for(std::chrono::seconds(1));

	spk::cout << "Resigning third contract" << std::endl;
	resignAndWait(std::move(third_contract));

	spk::cout << "Resigning second contract" << std::endl;
	resignAndWait(std::move(second_contract));

	worker.stop();
	return 0;
}