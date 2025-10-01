#pragma once

#include <iostream>
#include <string>
#include <map>
#include <memory>
#include "ACommand.hpp"
#include "PassCmd.hpp"
#include "UserCmd.hpp"
#include "NickCmd.hpp"
#include "PingCmd.hpp"
#include "PrivMsgCmd.hpp"
#include "JoinCmd.hpp"
#include "PartCmd.hpp"
#include "QuitCmd.hpp"
#include "KickCmd.hpp"
#include "InviteCmd.hpp"
#include "TopicCmd.hpp"

class CmdList
{
	private:
		std::map<std::string, std::unique_ptr<ACommand>> _commands;
		
	public:
		CmdList();
		~CmdList();
		
		ACommand* getCommand(const std::string& name) const;
};