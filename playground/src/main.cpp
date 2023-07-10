#include "playground.hpp"

int main()
{
	CommandParser parser;
	
	parser.setCommandIndicator(L"//");
	
	parser.setNoCommandCallback([&](const std::wstring& p_lineToExecute){spk::cout << L"Command not found : " << p_lineToExecute << std::endl;});
	parser.addCommandCallback(L"add", [&](const std::wstring& p_lineToExecute){spk::cout << L"Launching the add command with [" << p_lineToExecute << L"]" << std::endl; });
	parser.addCommandCallback(L"remove", [&](const std::wstring& p_lineToExecute){spk::cout << L"Launching the remove command with [" << p_lineToExecute << L"]" << std::endl; });

	parser.parse(L"/add This is a test");
	parser.parse(L"/add This is another test");
	parser.parse(L"/remove This is a test with the remove command");
	parser.parse(L"coucou This is a test without indicator");
	parser.parse(L"/notValidCommand This is a test with an indicator, but with an invalid command key");

	return (0);
}