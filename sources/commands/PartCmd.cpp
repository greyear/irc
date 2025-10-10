#include "../../includes/commands/PartCmd.hpp"
#include "../../includes/Client.hpp"
#include "../../includes/Server.hpp"

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
    if (params.size() > 1)
	{
        for (size_t i = 1; i < params.size(); ++i)
		{
            if (i > 1)
				reason += " ";
            reason += params[i];
        }
        if (!multiWordParam.empty())
		{
            if (!reason.empty())
				reason += " ";
            reason += multiWordParam;
        }
    }
	else if (!multiWordParam.empty())
	{
        reason = multiWordParam;
    }
	if (!reason.empty() && reason.length() > MAX_PART_REASON)
	{
		reason = reason.substr(0, MAX_PART_REASON);
	}
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

		if (!client->isInChannel(channelName))
		{
			server->sendError(client, ERR_NOTONCHANNEL, channelName + " :You're not on that channel");
			continue ;
		}
		
		sendPartConfirmation(server, client, channel, reason);
		partChannel(client, channel);
		if (channel->getMembers().empty())
			server->removeChannel(channelName);
	}
}

void	PartCmd::partChannel(Client* client, Channel* channel)
{
	channel->removeMember(client->getNick());
	channel->removeOperator(client->getNick());
	channel->removeInvited(client->getNick());
	client->leaveChannel(channel->getName());
}

void	PartCmd::sendPartConfirmation(Server* server, Client* client, Channel* channel, const std::string& reason)
{
	std::string partMsg = ":" + client->getFullIdentifier() + " PART " + channel->getName();
	if (!reason.empty())
		partMsg += " :" + reason;
	partMsg += "\r\n";
	for (const std::string& memberNick : channel->getMembers())
	{
		Client* member = server->getClientByNick(memberNick);
		if (member)
			server->sendToClient(member, partMsg);
	}
}