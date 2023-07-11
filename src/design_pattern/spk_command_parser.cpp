#include "design_pattern/spk_command_parser.hpp"

namespace spk
{
	CommandParser::CommandParser() : _commandsSeparator(L" ")
	{
	}

	void CommandParser::setCommandsIndicator(const std::wstring& p_commandsIndicator)
	{
		_commandsIndicator = p_commandsIndicator;
	}

	void CommandParser::setCommandsSeparator(const std::wstring& p_commandsSeparator)
	{
		_commandsSeparator = p_commandsSeparator;
	}


	void CommandParser::setNoCommandCallback(const CommandCallback& p_callback)
	{
		_noCommandCallback = p_callback;
	}

	void CommandParser::addCommandCallback(const std::wstring& p_command, const CommandCallback& p_callback)
	{
		_commandsCallback[p_command] = p_callback;
	}

	void CommandParser::parse(const std::wstring& p_commandLine)
	{
		if (p_commandLine.find(_commandsIndicator) != 0)
		{
			_noCommandCallback(p_commandLine);
			return;
		}

		size_t separatorPosition = p_commandLine.find(_commandsSeparator, _commandsIndicator.size());
		std::wstring command = p_commandLine.substr(_commandsIndicator.size(), separatorPosition - _commandsIndicator.size());

		if (_commandsCallback.contains(command) == true)
		{
			if (separatorPosition == std::string::npos)
			{
				_commandsCallback[command](L"");
			}
			else
			{
				_commandsCallback[command](p_commandLine.substr(_commandsSeparator.size() + separatorPosition));
			}
		}
		else
		{
			_noCommandCallback(p_commandLine);
		}
	}
}