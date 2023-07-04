#include "playground.hpp"

int main()
{
	spk::DataBuffer dataBuffer;



	int valueA1 = 42;
	int valueA2 = 12;
	std::wstring valueB1 = L"Coucou";

	dataBuffer << valueA1;
	dataBuffer << valueA2;
	dataBuffer << valueB1;



	int extractedValueA;
	int extractedValueA2;
	std::wstring extractedValueB;

	dataBuffer >> extractedValueA;
	dataBuffer >> extractedValueA2;
	dataBuffer >> extractedValueB;
	
	spk::cout << "Value A : " << extractedValueA << " - " << extractedValueA2 << " - " << extractedValueB << std::endl;

	return 0;
}