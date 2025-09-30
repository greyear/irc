#pragma once

#include <iostream>
#include "ACommand.hpp"
#include "Channel.hpp"

class KickCmd : public ACommand
{
	public:
		KickCmd();
		virtual ~KickCmd();
		bool	needsRegistration() const override;
		void	execute(Server* server, Client* client, const std::vector<std::string>& params, const std::string& multiWordParam) override;
		void	sendKickConfirmation(Server* server, Client* client, Channel* channel, const std::string& channelName, const std::string &targetNick, const std::string& reason);
};