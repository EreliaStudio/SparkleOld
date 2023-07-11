#include "playground.hpp"


int main()
{
	CommandParser parser;

	parser.setCommandsIndicator(L"/");
	

	parser.setNoCommandCallback([&](const std::wstring& p_lineToExecute){spk::cout << L"Command not found : " << p_lineToExecute << std::endl;});
	parser.addCommandCallback(L"add", [&](const std::wstring& p_lineToExecute){spk::cout << L"Launching the add command with [" << p_lineToExecute << L"]" << std::endl; });
	parser.addCommandCallback(L"add1", [&](const std::wstring& p_lineToExecute){spk::cout << L"Launching the add1 command with [" << p_lineToExecute << L"]" << std::endl; });
	parser.addCommandCallback(L"add2", [&](const std::wstring& p_lineToExecute){spk::cout << L"Launching the add2 command with [" << p_lineToExecute << L"]" << std::endl; });
	parser.addCommandCallback(L"remove", [&](const std::wstring& p_lineToExecute){spk::cout << L"Launching the remove command with [" << p_lineToExecute << L"]" << std::endl; });

	parser.parse(L"/add This is another test");
	parser.parse(L"/add1 This is another with add1 test");
	parser.parse(L"/add2 This is another with add2 test");
	parser.parse(L"/remove This is a test with the remove command");
	parser.parse(L"coucou This is a t/est without indicator");
	parser.parse(L"/notValidCommand This is a test with an indicator, but with an invalid command key");


	parser.parse(L"");
	parser.parse(L"/add");
	parser.parse(L"/removeThis is a test with the remove command");

	return (0);
}