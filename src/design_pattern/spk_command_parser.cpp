#include "design_pattern/spk_command_parser.hpp"
#include <map>
CommandParser::CommandParser()
{
}

void CommandParser::setCommandIndicator(const std::wstring& p_commandIndicator)
{
    _commandsIndicator = p_commandIndicator;
}

void CommandParser::setNoCommandCallback(CommandCallback& p_callback)
{
    _noCommandCallback = p_callback;
}

void CommandParser::addCommandCallback(const std::wstring& p_command, CommandCallback& p_callback)
{
    _commandsCallback[p_command] = p_callback;
}

void CommandParser::parse(const std::wstring& p_commandLine)
{
    int indicatorPosition = p_commandLine.find_first_of(_commandsIndicator);
    if (p_commandLine.compare(0, _commandsIndicator.size(), _commandsIndicator) != 0)
    {
        _noCommandCallback(p_commandLine);
    }
    else
    {
     for (std::map<std::wstring , CommandCallback>::iterator it=_commandsCallback.begin(); it!=_commandsCallback.end(); ++it)
     {
        if (p_commandLine.compare(_commandsIndicator.size() , it->first.size(), it->first) == 0)
        {
            it->second(p_commandLine.c_str() + _commandsIndicator.size() + it->first.size()); //can be change for substr()
        }
     }
    }
}
