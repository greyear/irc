#pragma once

#include <iostream>
#include "ACommand.hpp"
#include "Channel.hpp"
#include <regex>

class ModeCmd : public ACommand
{
	private:
		std::regex _pattern;
	protected:

	public:
		ModeCmd();
		virtual ~ModeCmd();

		bool	needsRegistration() const override;
		bool	isChannelNameValid(const std::string& channelName);
		void	execute(Server* server, Client* client, const std::vector<std::string>& params, const std::string& multiWordParam) override;
		//void	sendInviteConfirmations(Server* server, Client* inviter, Client* invitee, Channel* channel);
};

/*
MODE - Change the channel’s mode:
MODE <target> [<modestring> [<mode arguments>...]]
We can also have multiple in 1 cmd like:
MODE #test +ol Alice 5


· i: Set/remove Invite-only channel
(type D: It’s just a boolean flag; no parameter. You toggle it: +i / -i.)


· t: Set/remove the restrictions of the TOPIC command to channel operators
(type D: It’s just a boolean flag; no parameter. You toggle it: +t / -t.)


· k: Set/remove the channel key (password)
(type B: +k <key> to set, -k <key> to remove)


· o: Give/take channel operator privilege
(type A: adding/removing a nickname to/from a list of channel ops)


· l: Set/remove the user limit to channel
(type C: Needs a parameter when setting (+l <limit>) but no parameter when unsetting (-l))

only channel operators can change modes

*/