#include "KickCmd.hpp"
#include "Client.hpp"
#include "Server.hpp"

std::vector<std::string> splitTargets(const std::string& targets);
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
	if (params.empty() || params.size() < 2) //check to remove .empty
	{
		server->sendError(client, ERR_NEEDMOREPARAMS, " KICK :Not enough parameters");
		return;
	}

	const std::string& channelName = params[0];
	const std::string& kickerNick = client->getNick();
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
	if (!channel->isOperator(kickerNick))
	{
		server->sendError(client, ERR_CHANOPRIVSNEEDED, client->getNick() + " " + channelName + " :You're not channel operator");
		return;
	}
	std::vector<std::string> targets;
	if (params.size() > 1 && !params[1].empty())
		targets = splitTargets(params[1]);
	for (const std::string &targetNick : targets)
	{
		std::cout << " targetNick :" << targetNick << std::endl;
		if (targetNick == kickerNick)
		{
			server->sendError(client, ERR_CANTKICKYOURSELF, client->getNick() + " " + channelName + " :You cannot kick yourself from the channel");
			continue;
		}
		Client* targetClient = server->getClientByNick(targetNick);
		if (!targetClient)
		{
			server->sendError(client, ERR_NOSUCHNICK, client->getNick() + " " + targetNick + " :No such nick/channel");
			continue;
		}
		if (!channel->isMember(targetNick))
		{
			server->sendError(client, ERR_USERNOTINCHANNEL, client->getNick() + " " + targetNick + " " + channelName + " :They aren't on that channel");
			continue;
		}
		
		channel->removeMember(targetNick);
		channel->removeOperator(targetNick); 
		targetClient->leaveChannel(channelName);
		std::string reason;
		if (!multiWordParam.empty())
			reason = multiWordParam;
		else
			reason = targetNick;
		sendKickConfirmation(server, client, channel, channelName, targetNick, reason);
	}
}

void	KickCmd::sendKickConfirmation(Server* server, Client* client, Channel* channel, const std::string& channelName, const std::string &targetNick, const std::string& reason)
{
	std::string kickMessage = ":" + client->getFullIdentifier() + " KICK " + channelName + " " + targetNick + " :" + reason;

	for (const std::string& memberNick : channel->getMembers())
	{
		Client* memberClient = server->getClientByNick(memberNick);
		if (memberClient)
		{
			server->sendToClient(memberClient, kickMessage);
		}
	}
	Client* kickedClient = server->getClientByNick(targetNick);
	if (kickedClient)
		server->sendToClient(kickedClient, kickMessage);
}


	//not enough parameters X
	//no such channel X
	//#newchan You're not on that channel X
	//#newchan You're not a channel operator x

	//newnick00: No such nick/channel
	//newnick #newchan They aren't on that channel

	// Remove the target user from the channel
	// Remove from operators if they were one
	//remove the channel from client's list