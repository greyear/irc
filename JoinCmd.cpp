#include "JoinCmd.hpp"
#include "Client.hpp"
#include "Server.hpp"

std::vector<std::string> splitTargets(const std::string& targets);

JoinCmd::JoinCmd(): _pattern("^[#&][^\\x00-\\x1F\\x7F\\s,:]{1,50}$")
{

}

JoinCmd::~JoinCmd()
{

}

bool JoinCmd::needsRegistration() const
{
	return true;
}

bool JoinCmd::isChannelNameValid(const std::string& channelName)
{
	return std::regex_match(channelName, _pattern);
}

void JoinCmd::execute(Server* server, Client* client, const std::vector<std::string>& params, const std::string& multiWordParam)
{
	if (needsRegistration() && !client->checkRegistrationComplete())
	{
		server->sendError(client, ERR_NOTREGISTERED, " :You have not registered");
		return;
	}

	if (params.empty())
	{
		server->sendError(client, ERR_NEEDMOREPARAMS, " JOIN :Not enough parameters");
		return;
	}

	std::vector<std::string> channels = splitTargets(params[0]);
	if (channels.empty())
	{
		server->sendError(client, ERR_NEEDMOREPARAMS, " JOIN :Not enough parameters");
		return;
	}
	std::vector<std::string> keys;
	if (params.size() > 1 && !params[1].empty())
		keys = splitTargets(params[1]);
	
	for (size_t i = 0; i < channels.size(); ++i)
	{
		const std::string& channelName = channels[i];
		std::string key = (i < keys.size()) ? keys[i] : "";
		if (!isChannelNameValid(channelName))
		{
			server->sendError(client, ERR_BADCHANMASK, channelName + " :Bad Channel Mask");
			continue ;
		}
		if (client->isInChannel(channelName))
		{
			server->sendError(client, ERR_USERONCHANNEL, client->getNick() + channelName + " :is already on channel");
			continue ;
		}
		Channel* channel = server->getOrCreateChannel(channelName);
		if (channel->getInviteOnly() && !channel->isInvited(client->getNick()))
		{
			server->sendError(client, ERR_INVITEONLYCHAN, channelName + " :Cannot join channel (+i)");
			continue ;
		}
		if (channel->getHasLimit() && channel->.size() == channel->getLimit())
		{
			server->sendError(client, ERR_CHANNELISFULL, channelName + " :Cannot join channel (+l)");
			continue ;
		}
		if (channel->getPassRequired() && key != channel->getChannelPass())
		{
			server->sendError(client, ERR_BADCHANNELKEY, channelName + " :Cannot join channel (+k)");
			continue ;
		}
		if (client->getChannelLimit() == client->getChannels().size())
		{
			server->sendError(client, ERR_TOOMANYCHANNELS, channelName + " :You have joined too many channels");
			break ;
		}
		joinChannel(client, channel);
	}
}

void	JoinCmd::joinChannel(Client* client, Channel* channel)
{
	channel->addMember(client->getNick());
	client->addChannel(channel->getName());
	if(channel->getMembers().size() == 1)
		channel->addOperator(client->getNick());
}



void JoinCmd::sendJoinConfirmation(Server* server, Client* client, Channel* channel, const std::string& channelName)
{
    // 1. Send JOIN message to all channel members (including the joiner)
    std::string joinMsg = ":" + client->getNick() + "!" + client->getUser() + "@" + client->getHostName() 
                         + " JOIN :" + channelName + "\r\n";
    
    for (const std::string& memberNick : channel->getMembers())
	{
        Client* member = server->getClientByNick(memberNick);
        if (member)
		{
            server->sendToClient(member, joinMsg);
        }
    }
    
/*     // 2. Send channel topic (if exists)
    if (!channel->getTopic().empty())
	{
        server->sendReply(client, RPL_TOPIC, channelName + " :" + channel->getTopic());
    } 
	else
	{
        server->sendReply(client, RPL_NOTOPIC, channelName + " :No topic is set");
    }
    
    // 3. Send names list
    sendNamesList(server, client, channel, channelName); */
}
