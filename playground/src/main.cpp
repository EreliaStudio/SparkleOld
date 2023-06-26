#include "playground.hpp"


int main()
{
	spk::Thread thread(spk::Thread::LaunchMethod::Delayed, L"Thread", []()
	{
		spk::cout << L"Hello world!" << std::endl;
	});
	thread.start();
	thread.join();

	return 0;
}