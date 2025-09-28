#pragma once

#include <iostream>
#include "ACommand.hpp"
#include "Channel.hpp"
#include <regex>

class InviteCmd : public ACommand
{
	private:
		std::regex _pattern;
	protected:

	public:
		InviteCmd();
		virtual ~InviteCmd();

		bool	needsRegistration() const override;
		bool	isChannelNameValid(const std::string& channelName);
		void	execute(Server* server, Client* client, const std::vector<std::string>& params, const std::string& multiWordParam) override;
		void	sendInviteConfirmations(Server* server, Client* inviter, Client* invitee, Channel* channel);

};
		
/*
/invite nickname #channel

not enough parameters - ERR_NEEDMOREPARAMS!
no channel in input - ERR_NOSUCHCHANNEL!
inviter not in channel - ERR_NOTONCHANNEL!
inviter is not channel operator && channel is invite only - ERR_CHANOPRIVSNEEDED!
invitee already in channel - ERR_USERONCHANNEL!
invitee does not exist - ERR_NOSUCHNICK!
invitee is already invited - ERR_USERALREADYINVITED!

invitee is successfully invited - RPL_INVITING to inviter!
":dan-!d@localhost INVITE Wiz #test"
invitee receives an INVITE message
":inviter!user@host INVITE target :#channel"



*/