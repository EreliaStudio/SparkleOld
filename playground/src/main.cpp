#include "playground.hpp"

spk::RandomGenerator<unsigned short> random_generator( 42 );

void generateTenNumbers()
{
	spk::cout << "Generating 10 number in range: " << random_generator.min()
		<< L" <= " << random_generator.max() << std::endl;
	for ( int i = 0; i < 10; ++i )
	{
		spk::cout << random_generator() << std::endl;
	}
}

int main()
{
	spk::cout << L"Random test: \n"
		<< random_generator.min() << L" <= " << random_generator() << L" <= " << random_generator.max() << std::endl;

	random_generator.setDistributionRange(0, 1);
	generateTenNumbers();

	random_generator.setDistributionRange(0, 10);
	generateTenNumbers();
  
	return 0;
}