#include "PartCmd.hpp"
#include "Client.hpp"
#include "Server.hpp"

std::vector<std::string> splitTargets(const std::string& targets);

PartCmd::PartCmd(): _pattern("^[#&][^\\x00-\\x1F\\x7F\\s,:]{1,50}$")
{

}

PartCmd::~PartCmd()
{

}

bool PartCmd::needsRegistration() const
{
	return (true);
}

bool PartCmd::isChannelNameValid(const std::string& channelName)
{
	return std::regex_match(channelName, _pattern);
}

void	PartCmd::execute(Server* server, Client* client, const std::vector<std::string>& params, const std::string& multiWordParam)
{
	(void)multiWordParam;
	if (needsRegistration() && !client->checkRegistrationComplete())
	{
		server->sendError(client, ERR_NOTREGISTERED, " :You have not registered");
		return;
	}

	if (params.empty())
	{
		server->sendError(client, ERR_NEEDMOREPARAMS, " PART :Not enough parameters");
		return;
	}

	std::vector<std::string> channels = splitTargets(params[0]);
	if (channels.empty())
	{
		server->sendError(client, ERR_NEEDMOREPARAMS, " PART :Not enough parameters");
		return;
	}

	std::string reason;
	if (!multiWordParam.empty())
		reason = multiWordParam;
	else if (params.size() > 1 && !params[1].empty())
		reason = params[1];
	
	for (size_t i = 0; i < channels.size(); ++i)
	{
		const std::string& channelName = channels[i];
		if (!isChannelNameValid(channelName))
		{
			server->sendError(client, ERR_BADCHANMASK, channelName + " :Bad Channel Mask");
			continue ;
		}

		Channel* channel = server->getChannelByName(channelName);
		if (!channel)
		{
			server->sendError(client, ERR_NOSUCHCHANNEL, channelName + " :No such channel");
			continue;
		}

		if (client->isInChannel(channelName))
		{
			server->sendError(client, ERR_NOTONCHANNEL, channelName + " :You're not on that channel");
			continue ;
		}
		

		sendPartConfirmation(server, client, channel, channelName);
		
	}
}

//TODO: think about last operator user leaving the channel and all notificatons regarding that!

void	PartCmd::partChannel(Client* client, Channel* channel)
{
	channel->removeMember(client->getNick());
	client->leaveChannel(channel->getName());

}

void	PartCmd::sendPartConfirmation(Server* server, Client* client, Channel* channel, const std::string& channelName)
{
	std::string partMsg = ":" + client->getFullIdentifier() + " PART " + channelName + "\r\n";
	
	for (const std::string& memberNick : channel->getMembers())
	{
		Client* member = server->getClientByNick(memberNick);
		if (member)
			server->sendToClient(member, partMsg);
	}
}