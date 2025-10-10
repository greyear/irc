#include "../includes/CmdList.hpp"

CmdList::CmdList()
{
	_commands["PASS"] = std::make_unique<PassCmd>();
	_commands["NICK"] = std::make_unique<NickCmd>();
	_commands["USER"] = std::make_unique<UserCmd>();
	_commands["PING"] = std::make_unique<PingCmd>();
	_commands["PRIVMSG"] = std::make_unique<PrivMsgCmd>();
	_commands["JOIN"] = std::make_unique<JoinCmd>();
	_commands["PART"] = std::make_unique<PartCmd>();
	_commands["QUIT"] = std::make_unique<QuitCmd>();
	_commands["KICK"] = std::make_unique<KickCmd>();
	_commands["INVITE"] = std::make_unique<InviteCmd>();
	_commands["TOPIC"] = std::make_unique<TopicCmd>();
	_commands["MODE"] = std::make_unique<ModeCmd>();
}


ACommand* CmdList::getCommand(const std::string& name) const
{
	auto it = _commands.find(name);
	return (it != _commands.end()) ? it->second.get() : nullptr;
}

CmdList::~CmdList()
{
	
}