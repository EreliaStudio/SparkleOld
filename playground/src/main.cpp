#include "playground.hpp"

int main()
{
	spk::JSON::File file;
	std::wstring filesName[6] = {
		 L"array", L"literal",
		 L"number", L"object",
		 L"string", L"test_json"
	};

	for (unsigned short i(0); i < 6; ++i)
	{
		file.load(L"json/" + filesName[i] + L".json");
		spk::cout << "File number [" << i << "]: " << std::endl
			<< file << std::endl;
	}

	return (0);
}