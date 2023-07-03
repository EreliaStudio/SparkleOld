#include "playground.hpp"

int main()
{
	std::filesystem::path path;
	spk::JSON::File file;
	std::wstring filesName[6] = {
		 L"array", L"literal",
		 L"number", L"object",
		 L"string", L"test_json"
	};

	for (unsigned short i(0); i < 6; ++i)
	{
		path = L"json/" + filesName[i] + L".json";
		file.load(path);
		spk::cout << "File {" << path << "}: " << std::endl
			<< file << std::endl;
	}

	return (0);
}