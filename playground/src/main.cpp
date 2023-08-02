#include "playground.hpp"

int main()
{
	spk::PseudoRandomGenerator<int> random;
	
	spk::cout << "Random : " << random.sample(15, 15) << std::endl;

	spk::cout << "Hello world" << std::endl;
	return (0);
}