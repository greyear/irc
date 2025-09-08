#include "CmdList.hpp"

CmdList::CmdList()
{
	_commands["PASS"] = std::make_unique<PassCmd>();
	_commands["NICK"] = std::make_unique<NickCmd>();
	_commands["USER"] = std::make_unique<UserCmd>();
	_commands["PING"] = std::make_unique<PingCmd>();
	_commands["PRIVMSG"] = std::make_unique<PrivMsgCmd>();
}


ACommand* CmdList::getCommand(const std::string& name)
{
	auto it = _commands.find(name);
	return (it != _commands.end()) ? it->second.get() : nullptr;
}


CmdList::~CmdList()
{
	
}