#include "../../includes/commands/ModeCmd.hpp"
#include "../../includes/Client.hpp"
#include "../../includes/Server.hpp"

ModeCmd::ModeChange::ModeChange(char m, char s, const std::string& p): mode(m), sign(s), param(p)
{
	
}

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
		server->sendError(client, ERR_NEEDMOREPARAMS, " MODE :Not enough parameters");
		return;
	}
	
	const std::string& channelName = params[0];
	Channel* channel = server->getChannelByName(channelName);
	if (!channel)
	{
		if (channelName == client->getNick())
		{
			std::string userModesMsg = ":" + server->getServerName() + " " + RPL_UMODEIS + " " + client->getNick() + " +" + "\r\n";
			server->sendToClient(client, userModesMsg);
		}
		else
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
		const std::string& modestring = params[1];
		if (modestring == "b" || modestring == "+b")
			return;
		if (!channel->isOperator(client->getNick()))
		{
			std::cout << "MODE cmd is executed for channel" << channel->getName() + ", params are: " + params[1] << std::endl;
			server->sendError(client, ERR_CHANOPRIVSNEEDED, channelName + " :You're not channel operator");
			return;
		}
		
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
	std::string modestring = channel->getModestring();
	std::string modestringMsg = ":" + server->getServerName() + " " + RPL_CHANNELMODEIS + " " + client->getNick() + " " + channel->getName() + " " + modestring + "\r\n";
	server->sendToClient(client, modestringMsg);

	std::string creationTimeMsg = ":" + server->getServerName() + " " + RPL_CREATIONTIME + " " + client->getNick() + " " + channel->getName() + " " + std::to_string(channel->getCreationTime()) + "\r\n";
	server->sendToClient(client, creationTimeMsg);
}

void	ModeCmd::executeWithArgs(Server* server, Client* client, Channel* channel, const std::string& modestring, std::vector<std::string>& args)
{
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
		else
		{
			if (c == 'o')
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
	for (char c : modestring)
	{
		if (c == '+' || c == '-')
		{
			continue ;
		}
		if (!(c == 'i' || c == 't' || c == 'k' || c == 'o' || c == 'l' || c == 'b'))
		{
			server->sendError(client, ERR_UNKNOWNMODE, std::string(1, c) + " :is unknown mode char to me for " + channel->getName());
			return false;
		}
	}
	return true;
}

bool	ModeCmd::executeLetter(Server* server, Client* client, Channel* channel, char mode, char sign, std::string& arg)
{
	switch (mode)
	{
		case 'o':
			return handleO(server, client, channel, sign, arg);
		case 'k':
			return handleK(server, client, channel, sign, arg);
		case 'l':
			return handleL(server, client, channel, sign, arg);
		case 'i':
			return handleI(server, client, channel, sign);
		case 't':
			return handleT(server, client, channel, sign);
		case 'b':
			return (false);
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
		channel->addOperator(nickname);
	else
		channel->removeOperator(nickname);
	return (true);
}

bool ModeCmd::handleK(Server* server, Client* client, Channel* channel, char sign, std::string& key)
{
	(void)server;
	(void)client;
	if (sign == '+')
	{
		if (key.empty())
			return (false);
		if (key.length() > MAX_CHANNEL_KEY)
			key = key.substr(0, 23);
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

bool ModeCmd::handleL(Server* server, Client* client, Channel* channel, char sign, std::string& limitStr)
{
	(void)server;
	(void)client;
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
		limitStr = std::to_string(limit);
		channel->setLimit(limit);
	}
	else
	{
		if (!channel->getHasLimit())
			return (false);
		channel->removeLimit();
		limitStr = "";
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
			return (false);
		channel->setInviteOnly(true);
	}	
	else
	{
		if (!channel->getInviteOnly())
			return (false);
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
			return (false);
		channel->setTopicRestriction(true);
	}	
	else
	{
		if (!channel->getTopicRestriction())
			return (false);
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
		else
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
		allModes += "-" + minusModes;
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