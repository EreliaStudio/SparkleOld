#include "playground.hpp"

#define FILE_AMOUNT 1

int main()
{
	std::filesystem::path path;
	spk::JSON::File file;
	std::wstring filesName[FILE_AMOUNT] = {
		//  L"array", L"literal",
		 L"number"/*, L"object",*/
		//  L"string", L"test_json"
	};

	for (unsigned short i(0); i < FILE_AMOUNT; ++i)
	{
		path = L"json/" + filesName[i] + L".json";
		file.load(path);
		spk::cout << "File {" << path << "}: " << std::endl
			<< file << std::endl;
	}

	return (0);
}