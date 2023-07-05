#include "playground.hpp"

int main()
{
	spk::JSON::File file;
	std::vector<std::wstring> jsonFiles(spk::listFile(L"json", L".json"));

	for (std::wstring filePath: jsonFiles)
	{
		file.load(filePath);
		spk::cout << "File {" << filePath << "}: " << std::endl
			<< file << std::endl;
	}

	return (0);
}