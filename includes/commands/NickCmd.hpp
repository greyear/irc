#pragma once

#include <iostream>
#include "ACommand.hpp"
#include <regex>

class NickCmd : public ACommand
{
	private:
		std::regex _pattern;
	public:
		NickCmd();
		virtual ~NickCmd();
		bool needsRegistration() const override;
		bool isNickValid(const std::string& nick);
		void execute(Server* server, Client* client, const std::vector<std::string>& params, const std::string& multiWordParam) override;
		void notifyAndUpdateChannels(Client *client, Server *server, const std::string &nickChangeMsg, const std::string &oldNick, const std::string &newNick);
};