#pragma once

#include <iostream>
#include "ACommand.hpp"
#include "Channel.hpp"
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
	protected:

	public:
		ModeCmd();
		virtual ~ModeCmd();

		bool	needsRegistration() const override;
		bool	isChannelNameValid(const std::string& channelName);
		bool	validateModestring(Server* server, Client* client, Channel* channel, const std::string& modestring);
		void	execute(Server* server, Client* client, const std::vector<std::string>& params, const std::string& multiWordParam) override;
		void	executeNoArgs(Server* server, Client* client, Channel* channel);
		void	executeWithArgs(Server* server, Client* client, Channel* channel, const std::string& modestring, std::vector<std::string>& args);
		bool	executeLetter(Server* server, Client* client, Channel* channel, char mode, char sign, std::string& arg); //check if need so many inputs
		bool	handleO(Server* server, Client* client, Channel* channel, char sign, const std::string& nickname);
		bool	handleK(Server* server, Client* client, Channel* channel, char sign, std::string& key);
		bool	handleL(Server* server, Client* client, Channel* channel, char sign, std::string& limitStr);
		bool	handleI(Server* server, Client* client, Channel* channel, char sign);
		bool	handleT(Server* server, Client* client, Channel* channel, char sign);
		void	sendModeConfirmations(Server* server, Client* client, Channel* channel, const std::vector<ModeChange>& changes);
};

/*
MODE - Change the channel’s mode:
MODE <target> [<modestring> [<mode arguments>...]]
We can also have multiple in 1 cmd like:
MODE #test +ol Alice 5
/mode #chan k key is treated the same as /mode #chan +k key
one letter takes 1 arg at most!
MODE #test +oo Alice Pete

· i: Set/remove Invite-only channel
(type D: It’s just a boolean flag; no parameter. You toggle it: +i / -i.)
0 when +, 0 when -

· t: Set/remove the restrictions of the TOPIC command to channel operators
(type D: It’s just a boolean flag; no parameter. You toggle it: +t / -t.)
0 when +, 0 when -

· k: Set/remove the channel key (password)
(type B: +k <key> to set, -k <key> to remove)
1 when +, 1 when -

· o: Give/take channel operator privilege
(type A: adding/removing a nickname to/from a list of channel ops)
1 when +, 1 when -

· l: Set/remove the user limit to channel
(type C: Needs a parameter when setting (+l <limit>) but no parameter when unsetting (-l))
1 when +, 0 when -

only channel operators can change modes

*/