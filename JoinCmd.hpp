#pragma once

#include <iostream>
#include "ACommand.hpp"
#include <regex>

class JoinCmd : public ACommand
{
	private:
		std::regex _pattern;
	protected:

	public:
		JoinCmd();
		virtual ~JoinCmd();

		bool needsRegistration() const override;
		bool isChannelNameValid(const std::string& channelName);
		void execute(Server* server, Client* client, const std::vector<std::string>& params, const std::string& multiWordParam) override;
		void	joinChannel(Client* client, Channel* channel);
};