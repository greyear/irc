#pragma once

#include <iostream>
#include "ACommand.hpp"
#include "../Channel.hpp"
#include <regex>

class JoinCmd : public ACommand
{
	private:
		std::regex _pattern;
	public:
		JoinCmd();
		virtual ~JoinCmd();

		bool	needsRegistration() const override;
		bool	isChannelNameValid(const std::string& channelName);
		void	execute(Server* server, Client* client, const std::vector<std::string>& params, const std::string& multiWordParam) override;
		void	joinChannel(Client* client, Channel* channel);
		void	sendJoinConfirmation(Server* server, Client* client, Channel* channel, const std::string& channelName);
		void	sendMembersList(Server* server, Client* client, Channel* channel, const std::string& channelName);
};