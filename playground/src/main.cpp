#include "playground.hpp"

int main()
{
	spk::JSON::File file;
	std::vector<std::wstring> jsonFiles(spk::listFile(L"json", L".json"));

	for (std::wstring filePath : jsonFiles)
	{
		try
		{
			if (filePath.find(L"copy") != std::wstring::npos)
			{
				std::filesystem::remove(filePath);
				continue;
			}
			file.load(filePath);
			spk::cout << "File {" << filePath << "}: " << std::endl
				<< file << std::endl;
			file.save(filePath.erase(filePath.find_last_of(L'.')).append(L"_copy.json"));
		}
		catch(const std::exception& e)
		{
			std::cerr << '\t' << e.what() << '\n';
		}
	}

	return (0);
}