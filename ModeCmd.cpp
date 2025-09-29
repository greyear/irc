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
	if (!isChannelNameValid(channelName))
	{
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

	//if (params.size() == 1)

	//cmd logic

}

