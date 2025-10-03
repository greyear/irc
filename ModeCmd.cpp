#include "ModeCmd.hpp"
#include "Client.hpp"
#include "Server.hpp"

struct ModeChange
{
    char mode;
    char sign;
    std::string param;
    ModeChange(char m, char s, const std::string& p = ""): mode(m), sign(s), param(p)
	{
		
	}
};

ModeCmd::ModeCmd(): _pattern("^[#&][^\\x00-\\x1F\\x7F\\s,:]{1,50}$")
{

}

ModeCmd::~ModeCmd()
{

}

bool ModeCmd::needsRegistration() const
{
	return true;
}

bool ModeCmd::isChannelNameValid(const std::string& channelName)
{
	return std::regex_match(channelName, _pattern);
}

void	ModeCmd::execute(Server* server, Client* client, const std::vector<std::string>& params, const std::string& multiWordParam)
{
	(void)multiWordParam;
	if (needsRegistration() && !client->checkRegistrationComplete())
	{
		server->sendError(client, ERR_NOTREGISTERED, ":You have not registered");
		return;
	}
	if (params.empty())
	{
		server->sendError(client, ERR_NEEDMOREPARAMS, "MODE :Not enough parameters");
		return;
	}
	
	const std::string& channelName = params[0];
	if (!isChannelNameValid(channelName))
	{
		if (channelName == client->getNick())
		{
			//by default we're saying your modes are + and nothing!
			std::string userModesMsg = ":" + server->getServerName() + " " + RPL_UMODEIS + " " + client->getNick() + " +" + "\r\n";
			server->sendToClient(client, userModesMsg);
		}
		else
			server->sendError(client, ERR_BADCHANMASK, channelName + " :Bad Channel Mask");
		return;
	}
	Channel* channel = server->getChannelByName(channelName);
	if (!channel)
	{
		server->sendError(client, ERR_NOSUCHCHANNEL, channelName + " :No such channel");
		return;
	}

	if (!client->isInChannel(channelName))
	{
		server->sendError(client, ERR_NOTONCHANNEL, channelName + " :You're not on that channel");
		return;
	}

	if (params.size() == 1)
		executeNoArgs(server, client, channel);
	else
	{
		if (!channel->isOperator(client->getNick())) //check how it works on real things! maybe not right!
		{
			server->sendError(client, ERR_CHANOPRIVSNEEDED, channelName + " :You're not channel operator");
			return;
		}
		const std::string& modestring = params[1];
		std::vector<std::string> args;
		if (params.size() > 2)
		{
			for (size_t i = 2; i < params.size(); ++i)
				args.push_back(params[i]);
		}
		executeWithArgs(server, client, channel, modestring, args);
	}
}

void	ModeCmd::executeNoArgs(Server* server, Client* client, Channel* channel)
{
	std::cout << "executing with no args" << std::endl;

	std::string modestring = channel->getModestring();
	std::string modestringMsg = ":" + server->getServerName() + " " + RPL_CHANNELMODEIS + " " + client->getNick() + " " + channel->getName() + " " + modestring + "\r\n";
	server->sendToClient(client, modestringMsg);

	std::string creationTimeMsg = ":" + server->getServerName() + " " + RPL_CREATIONTIME + " " + client->getNick() + " " + channel->getName() + " " + std::to_string(channel->getCreationTime()) + "\r\n";
	server->sendToClient(client, creationTimeMsg);
}

void	ModeCmd::executeWithArgs(Server* server, Client* client, Channel* channel, const std::string& modestring, std::vector<std::string>& args)
{
	std::cout << "executing with some args" << std::endl;

	if (!validateModestring(server, client, channel, modestring))
		return ;

	size_t aIndex = 0;
	char sign = '+';
	std::vector<ModeChange> successfulChanges;
	for (char c : modestring)
	{
		if (c == '+' || c == '-')
		{
			sign = c;
			continue ;
		}

		bool needsArgs = false;
		if (sign == '+')
		{
			if (c == 'k' || c == 'o' || c == 'l')
				needsArgs = true;
		}
		else //sign == '-'
		{
			if (c == 'k' || c == 'o')
				needsArgs = true;
		}

		std::string arg;
		if (needsArgs)
		{
			if (aIndex < args.size())
				arg = args[aIndex++];
			else
			{
				server->sendError(client, ERR_NEEDMOREPARAMS, "MODE :Not enough parameters");
				return;
			}
		}

		if (executeLetter(server, client, channel, c, sign, arg))
			successfulChanges.push_back(ModeChange(c, sign, arg));
	}
	if (!successfulChanges.empty())
		sendModeConfirmations(server, client, channel, successfulChanges);
}

bool	ModeCmd::validateModestring(Server* server, Client* client, Channel* channel, const std::string& modestring)
{
	char sign = '+';
	for (char c : modestring)
	{
		if (c == '+' || c == '-')
		{
			sign = c;
			continue ;
		}
		if (!(c == 'i' || c == 't' || c == 'k' || c == 'o' || c == 'l'))
		{
			std::cout << "CHAR IS: " + std::string(1, c) << std::endl;
			server->sendError(client, ERR_UNKNOWNMODE, std::string(1, c) + " :is unknown mode char to me for " + channel->getName());
			return false;
		}
	}
	return true;
}

bool	ModeCmd::executeLetter(Server* server, Client* client, Channel* channel, char mode, char sign, std::string arg)
{
	std::cout << "executing with args, mode is " << mode << ", sign is " << sign << std::endl;

	//need to check if arg is empty
	switch (mode)
	{
		case 'o':
			return handleO(server, client, channel, sign, arg);
		case 'k':
			return handleK(server, client, channel, sign, arg);
		case 'l':  //check if limit isn't a number, -1 and 0, maxInt+1
			return handleL(server, client, channel, sign, arg);
		case 'i':
			return handleI(server, client, channel, sign);
		case 't':
			return handleT(server, client, channel, sign);
		default:
			server->sendError(client, ERR_UNKNOWNMODE, std::string(1, mode) + " :is unknown mode char to me for " + channel->getName());
			return (false);
	}
}

bool ModeCmd::handleO(Server* server, Client* client, Channel* channel, char sign, const std::string& nickname)
{
	Client* target = server->getClientByNick(nickname);
	if (!target)
	{
		server->sendError(client, ERR_NOSUCHNICK, nickname + " :No such nick/channel");
		return (false);
	}
	if (!channel->isMember(nickname))
	{
		server->sendError(client, ERR_USERNOTINCHANNEL, nickname + " " + channel->getName() + " :They aren't on that channel");
		return (false);
	}
	if (sign == '+')
	{
		if (channel->isOperator(nickname))
			return (false);
		channel->addOperator(nickname);
	}
	else
	{
		if (!channel->isOperator(nickname))
			return (false);
		channel->removeOperator(nickname);
	}
	return (true);
}

bool ModeCmd::handleK(Server* server, Client* client, Channel* channel, char sign, const std::string& key)
{
	//maybe -k with no args should work
	if (sign == '+')
	{
		if (key.empty()) //some other checks and cutting
			return (false);
		channel->setKey(key);
	}
	else
	{
		if (!channel->getKeyRequired())
			return (false);
		channel->removeKey();
	}
	return (true);
}

bool ModeCmd::handleL(Server* server, Client* client, Channel* channel, char sign, const std::string& limitStr)
{
	if (sign == '+')
	{
		int limit;
		try
		{
			limit = std::stoi(limitStr);
		}
		catch(const std::exception& e)
		{
			return (false);
		}
		if (limit <= 0)
			return (false);
		channel->setLimit(limit);
	}
	else
	{
		if (!channel->getHasLimit())
			return (false);
		channel->removeLimit();
	}
	return (true);
}

bool ModeCmd::handleI(Server* server, Client* client, Channel* channel, char sign)
{
	(void)server;
	(void)client;

	if (sign == '+')
	{
		if (channel->getInviteOnly())
			return (false); //already set
		channel->setInviteOnly(true);
	}	
	else
	{
		if (!channel->getInviteOnly())
			return (false); //already UNset
		channel->setInviteOnly(false);
	}
	return (true);
}

bool ModeCmd::handleT(Server* server, Client* client, Channel* channel, char sign)
{
	(void)server;
	(void)client;

	if (sign == '+')
	{
		if (channel->getTopicRestriction())
			return (false); //already restricted
		channel->setTopicRestriction(true);
	}	
	else
	{
		if (!channel->getTopicRestriction())
			return (false); //already NOT
		channel->setTopicRestriction(false);
	}
	return (true);
}

void	ModeCmd::sendModeConfirmations(Server* server, Client* client, Channel* channel, const std::vector<ModeChange>& changes)
{
	std::string plusModes = "";
	std::string minusModes = "";
	std::vector<std::string> params;

	for (const auto& change : changes)
	{
		if (change.sign == '+')
		{
			plusModes += change.mode;
			if (!change.param.empty())
				params.push_back(change.param);
		}
		else //minus
		{
			minusModes += change.mode;
			if (change.mode == 'k')
				params.push_back("*");
			else
			{
				if (!change.param.empty())
					params.push_back(change.param);
			}
		}
	}

	std::string allModes = "";
	if (!plusModes.empty())
		allModes += "+" + plusModes;
	if (!minusModes.empty())
		allModes += "+" + minusModes;
	for (const auto& param : params)
		allModes += " " + param;

	std::string modeMsg = ":" + client->getFullIdentifier() + " MODE " + channel->getName() + " " + allModes + "\r\n";
	for (const std::string& memberNick : channel->getMembers())
	{
		Client* member = server->getClientByNick(memberNick);
		if (member)
			server->sendToClient(member, modeMsg);
	}
}