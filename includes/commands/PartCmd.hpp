#pragma once

#include <iostream>
#include "ACommand.hpp"
#include "../Channel.hpp"
#include <regex>

class PartCmd : public ACommand
{
	private:
		std::regex _pattern;
	public:
		PartCmd();
		virtual ~PartCmd();

		bool	needsRegistration() const override;
		bool	isChannelNameValid(const std::string& channelName);
		void	execute(Server* server, Client* client, const std::vector<std::string>& params, const std::string& multiWordParam) override;
		void	partChannel(Client* client, Channel* channel);
		void	sendPartConfirmation(Server* server, Client* client, Channel* channel, const std::string& reason);
};