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
	if (params.size() < 2)
	{
		server->sendError(client, ERR_NEEDMOREPARAMS, " KICK :Not enough parameters");
		return;
	}

	const std::string& channelName = params[0];
	const std::string& kickerNick = client->getNick();
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
	if (!channel->isOperator(kickerNick))
	{
		server->sendError(client, ERR_CHANOPRIVSNEEDED, channelName + " :You're not channel operator");
		return;
	}
	std::vector<std::string> targets;
	if (params.size() > 1 && !params[1].empty())
		targets = splitTargets(params[1]);
	for (const std::string &targetNick : targets)
	{
		if (targetNick == kickerNick)
		{
			server->sendError(client, ERR_CANTKICKYOURSELF, channelName + " :You cannot kick yourself from the channel");
			continue;
		}
		Client* targetClient = server->getClientByNick(targetNick);
		if (!targetClient)
		{
			server->sendError(client, ERR_NOSUCHNICK, targetNick + " :No such nick/channel");
			continue;
		}
		if (!channel->isMember(targetNick))
		{
			server->sendError(client, ERR_USERNOTINCHANNEL, targetNick + " " + channelName + " :They aren't on that channel");
			continue;
		}
		
		channel->removeMember(targetNick);
		channel->removeOperator(targetNick); 
		targetClient->leaveChannel(channelName);

		std::string reason;
		for (size_t i = 2; i < params.size(); i++)
		{
			if (i > 2)
				reason += " ";
			reason += params[i];
		}
		if (!multiWordParam.empty())
		{
			if (!reason.empty())
				reason += " ";
			reason += multiWordParam;
		}
		//truncate if long
		if (reason.empty())
			reason = targetNick; // TODO: Default = kicker's nick or target's nick ?
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
