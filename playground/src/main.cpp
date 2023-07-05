#include "playground.hpp"

void enableUnicode()
{
	std::locale::global(std::locale(""));
	std::wcout.imbue(std::locale());
	std::wcin.imbue(std::locale());
	std::wcerr.imbue(std::locale());
}

int main()
{
	spk::JSON::File file;
	std::vector<std::wstring> jsonFiles(spk::listFile(L"json", L".json"));

	enableUnicode();
	for (std::wstring filePath : jsonFiles)
	{
		if (filePath != L"json/string.json")
			continue;
		file.load(filePath);
		spk::cout << "File {" << filePath << "}: " << std::endl
			<< file << std::endl;
	}

	return (0);
}