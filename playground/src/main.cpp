#include "playground.hpp"

int main()
{
	spk::JSON::File file;
	std::vector<std::wstring> filePath = spk::listFile(L"json", L".json");

	for (const auto& path : filePath)
	{
		try
		{
			file.load(path);
			std::wcout << L"Path: " << path << std::endl;
			std::wcout << L"File: " << file << std::endl;
		}
		catch (const std::exception& e)
		{
			std::cerr << e.what() << '\n';
		}
	}
	return (0);
}
