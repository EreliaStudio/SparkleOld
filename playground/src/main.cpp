#include "playground.hpp"

spk::Value<int>::Default defaultA = 42;
spk::Value<int>::Default defaultB = 66;

spk::Value<int> valueA = defaultA;
spk::Value<int> valueB = spk::Value<int>(defaultA);
spk::Value<int> valueC = spk::Value<int>(defaultA, 100);

void printValues(size_t p_nbRun, std::wstring p_message)
{
	spk::cout << " --- RUN " << p_nbRun << " : " << p_message << " ---" << std::endl;
	spk::cout << "DefaultA : " << defaultA << std::endl;
	spk::cout << "DefaultB : " << defaultB << std::endl;
	spk::cout << "ValueA : " << valueA << std::endl;
	spk::cout << "ValueB : " << valueB << std::endl;
	spk::cout << "ValueC : " << valueC << std::endl;
	spk::cout << " ---" << std::endl;
}

int main()
{
	defaultA = 42;
	defaultB = 66;

	valueA = defaultA;
	valueB = spk::Value<int>(defaultA);
	valueC = spk::Value<int>(defaultA, 100);

	printValues(0, L"Initial state");

	valueA = 15;

	printValues(1, L"Setting valueA");

	valueC = valueA + valueB;

	printValues(2, L"Checking sum operation");

	valueA.reset();

	printValues(3, L"Reseting value A");

	defaultA = 1337;

	printValues(4, L"Editing default value A");

	valueA.setDefaultValue(defaultB);

	printValues(5, L"Setting value A default value to B");

	if (true)
	{
		spk::Value<int>::Contract tmpContractA = valueA.subscribe([&](){
			spk::cout << "ValueA edition" << std::endl;
		});
		spk::Value<int>::Contract tmpContractB = valueB.subscribe([&](){
			spk::cout << "ValueB edition" << std::endl;
		});
		spk::Value<int>::Contract tmpContractC = valueC.subscribe([&](){
			spk::cout << "ValueC edition" << std::endl;
		});

		spk::cout << "Step 1" << std::endl;

		valueA = 15;

		spk::cout << "Step 2" << std::endl;
		
		valueB = 64;

		spk::cout << "Step 3" << std::endl;
		
		valueC = 128;

		spk::cout << "Step 4" << std::endl;
		
		valueA.reset();

		spk::cout << "Step 5" << std::endl;

		valueB = valueA;

		spk::cout << "Step 6" << std::endl;

		valueC = defaultB;

		spk::cout << "Step 7" << std::endl;

		defaultB = 0;

		spk::cout << "Step 8" << std::endl;
		
	}

	return (0);
}