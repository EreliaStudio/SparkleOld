#include "playground.hpp"

int main()
{
	spk::TranslationAtlas atlas;
	std::vector<std::wstring> args = { L"key", L"key2", L"key3", L"key4",
										L"key5", L"key6", L"key7", L"key8" };

	try
	{
		atlas.load(L"playground/src/en_EN.json");
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << '\n';
	}

	for (const auto& key : args)
		spk::cout << atlas.get(key) << std::endl;
	return (0);
}
