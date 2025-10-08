#pragma once

#include <iostream>
#include "ACommand.hpp"
#include "../Channel.hpp"

class PrivMsgCmd : public ACommand
{
	public:
		PrivMsgCmd();
		virtual ~PrivMsgCmd();
		bool needsRegistration() const override;
		void execute(Server* server, Client* client, const std::vector<std::string>& params, const std::string& multiWordParam) override;
		void sendToChanMembers(Channel *targetChan, const std::string& channelMsg, Client *client, Server *server);
};