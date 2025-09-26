#include "KickCmd.hpp"
#include "Client.hpp"
#include "Server.hpp"

KickCmd::KickCmd()
{

}
KickCmd::~KickCmd()
{

}

bool KickCmd::needsRegistration() const
{
	return true;
}

void	KickCmd::execute(Server* server, Client* client, const std::vector<std::string>& params, const std::string& multiWordParam)
{
	if (needsRegistration() && !client->checkRegistrationComplete())
	{
		server->sendError(client, ERR_NOTREGISTERED, " :You have not registered");
		return;
	}
	if (params.empty() || params.size() < 2)
	{
		server->sendError(client, ERR_NEEDMOREPARAMS, " Kick :Not enough parameters");
		return;
	}

	const std::string& channelName = params[0];
	const std::string& targetNick = params[1];

	if (!server->getChannelByName(channelName))
	{
		server->sendError(client, ERR_NOSUCHCHANNEL, channelName + " :No such channel");
		return;
	}
}

	//not enough parameters
	//no such channel
	//#newchan You're not on that channel

	//#newchan You're not a channel operator
	//newnick00: No such nick/channel
	//newnick #newchan They aren't on that channel