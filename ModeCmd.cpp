#include "ModeCmd.hpp"
#include "Client.hpp"
#include "Server.hpp"

//what about users' modes?

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
	if (!isChannelNameValid(channelName)) //check mode for nick too!
	{
		std::cout << "DEBUGGING" << std::endl;
		//
		server->sendError(client, ERR_BADCHANMASK, client->getNick() + " " + channelName + " :Bad Channel Mask");
		return;
	}
	Channel* channel = server->getChannelByName(channelName);
	if (!channel)
	{
		server->sendError(client, ERR_NOSUCHCHANNEL, client->getNick() + " " + channelName + " :No such channel");
		return;
	}

	if (!client->isInChannel(channelName))
	{
		server->sendError(client, ERR_NOTONCHANNEL, client->getNick() + " " + channelName + " :You're not on that channel");
		return;
	}
	if (!channel->isOperator(client->getNick())) //check how it works on real things! maybe not right!
	{
		server->sendError(client, ERR_CHANOPRIVSNEEDED, client->getNick() + " " + channelName + " :You're not channel operator");
		return;
	}

	if (params.size() == 1)
		executeNoArgs(server, client, channel);
	else
	{
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
	(void)server;
	(void)client;
	(void)channel;
}

/*void	ModeCmd::executeWithArgs(Server* server, Client* client, Channel* channel, const std::string& modestring, std::vector<std::string>& args)
{
	std::cout << "executing with some args" << std::endl;

	size_t aIndex = 0;
	char sign = '+'; //look like in irssi by default is +
	for (char c : modestring)
	{
		if (c == '+' || c == '-') //what if not
		{
			sign = c;
			continue ;
		}
		if (sign == '+')
		{
			if (c == 'k' || c == 'o' || c == 'l')
			{
				if (aIndex < args.size()) //check with no args
					executeLetter(server, client, channel, c, sign, args[aIndex++]);
				else
				{
					server->sendError(client, ERR_NEEDMOREPARAMS, "MODE :Not enough parameters");
					return;//?
				}
			}
			else if (c == 'i' || c == 't')
				executeLetter(server, client, channel, c, sign, "");
			else
			{
				server->sendError(client, ERR_UNKNOWNMODE, std::string(1, c) + " :is unknown mode char to me for " + channel->getName());
				return;//?
			}
		}
		else //sign == '-'
		{
			if (c == 'k' || c == 'o')
			{
				if (aIndex < args.size()) //check with no args
					executeLetter(server, client, channel, c, sign, args[aIndex++]);
				else
				{
					server->sendError(client, ERR_NEEDMOREPARAMS, "MODE :Not enough parameters");
					return;
				}
			}
			else if (c == 'l' || c == 'i' || c == 't')
				executeLetter(server, client, channel, c, sign, "");
			else
			{
				server->sendError(client, ERR_UNKNOWNMODE, std::string(1, c) + " :is unknown mode char to me for " + channel->getName());
				return;//?
			}
		}
	}
}*/

void	ModeCmd::executeWithArgs(Server* server, Client* client, Channel* channel, const std::string& modestring, std::vector<std::string>& args)
{
	std::cout << "executing with some args" << std::endl;

	if (!validateModestring(server, client, channel, modestring))
		return ;

	size_t aIndex = 0;
	char sign = '+';
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
				return;//?
			}
		}

		executeLetter(server, client, channel, c, sign, arg);
	}
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
			server->sendError(client, ERR_UNKNOWNMODE, std::string(1, c) + " :is unknown mode char to me for " + channel->getName());
			return false;
		}
	}
	return true;
}

void	ModeCmd::executeLetter(Server* server, Client* client, Channel* channel, char mode, char sign, std::string arg)
{
	std::cout << "executing with args, mode is " << mode << ", sign is " << sign << std::endl;
	(void)server;
	(void)client;
	(void)channel;
	(void)mode;
	(void)sign;
	//need to check if arg is empty
	(void)arg;
}
