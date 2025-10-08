#pragma once

#include <iostream>
#include <string>
#include <map>
#include <memory>
#include "./commands/ACommand.hpp"
#include "./commands/PassCmd.hpp"
#include "./commands/UserCmd.hpp"
#include "./commands/NickCmd.hpp"
#include "./commands/PingCmd.hpp"
#include "./commands/PrivMsgCmd.hpp"
#include "./commands/JoinCmd.hpp"
#include "./commands/PartCmd.hpp"
#include "./commands/QuitCmd.hpp"
#include "./commands/KickCmd.hpp"
#include "./commands/InviteCmd.hpp"
#include "./commands/TopicCmd.hpp"
#include "./commands/ModeCmd.hpp"

class CmdList
{
	private:
		std::map<std::string, std::unique_ptr<ACommand>> _commands;
		
	public:
		CmdList();
		~CmdList();
		
		ACommand* getCommand(const std::string& name) const;
};