#pragma once

#include <iostream>
#include "ACommand.hpp"
#include "../Channel.hpp"
#include <regex>

class InviteCmd : public ACommand
{
	private:
		std::regex _pattern;
	public:
		InviteCmd();
		virtual ~InviteCmd();

		bool	needsRegistration() const override;
		bool	isChannelNameValid(const std::string& channelName);
		void	execute(Server* server, Client* client, const std::vector<std::string>& params, const std::string& multiWordParam) override;
		void	sendInviteConfirmations(Server* server, Client* inviter, Client* invitee, Channel* channel);
};
