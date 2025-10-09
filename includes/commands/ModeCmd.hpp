#pragma once

#include <iostream>
#include "ACommand.hpp"
#include "../Channel.hpp"
#include <regex>

class ModeCmd : public ACommand
{
	private:
		std::regex _pattern;
		struct ModeChange
		{
			char mode;
			char sign;
			std::string param;
			ModeChange(char m, char s, const std::string& p = "");
		};
	public:
		ModeCmd();
		virtual ~ModeCmd();

		bool	needsRegistration() const override;
		bool	isChannelNameValid(const std::string& channelName);
		bool	validateModestring(Server* server, Client* client, Channel* channel, const std::string& modestring);
		void	execute(Server* server, Client* client, const std::vector<std::string>& params, const std::string& multiWordParam) override;
		void	executeNoArgs(Server* server, Client* client, Channel* channel);
		void	executeWithArgs(Server* server, Client* client, Channel* channel, const std::string& modestring, std::vector<std::string>& args);
		bool	executeLetter(Server* server, Client* client, Channel* channel, char mode, char sign, std::string& arg);
		bool	handleO(Server* server, Client* client, Channel* channel, char sign, const std::string& nickname);
		bool	handleK(Server* server, Client* client, Channel* channel, char sign, std::string& key);
		bool	handleL(Server* server, Client* client, Channel* channel, char sign, std::string& limitStr);
		bool	handleI(Server* server, Client* client, Channel* channel, char sign);
		bool	handleT(Server* server, Client* client, Channel* channel, char sign);
		void	sendModeConfirmations(Server* server, Client* client, Channel* channel, const std::vector<ModeChange>& changes);
};
