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
	(void)multiWordParam;
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
			server->sendError(client, ERR_USERONCHANNEL, channelName + " :is already on channel");
			continue ;
		}
		Channel* channel = server->getOrCreateChannel(channelName);
		if (channel->getInviteOnly() && !channel->isInvited(client->getNick()))
		{
			server->sendError(client, ERR_INVITEONLYCHAN, channelName + " :Cannot join channel (+i)");
			continue ;
		}
		if (channel->getHasLimit() && channel->getMembers().size() >= channel->getLimit())
		{
			server->sendError(client, ERR_CHANNELISFULL, channelName + " :Cannot join channel (+l)");
			continue ;
		}
		if (channel->getKeyRequired() && key != channel->getChannelKey())
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
		sendJoinConfirmation(server, client, channel, channel->getName());
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
	std::string joinMsg = ":" + client->getFullIdentifier() + " JOIN " + channelName + "\r\n";
	
	for (const std::string& memberNick : channel->getMembers())
	{
		Client* member = server->getClientByNick(memberNick);
		if (member)
			server->sendToClient(member, joinMsg);
	}

	if (!channel->getTopic().empty())
	{
		std::string topicMsg = ":" + server->getServerName() + " " + RPL_TOPIC + " " + client->getNick() + " " + channelName + " :" + channel->getTopic();
		server->sendToClient(client, topicMsg);
	} 
	else
	{
		std::string topicMsg = ":" + server->getServerName() + " " + RPL_NOTOPIC + " " + client->getNick() + " " + channelName + " :No topic is set";
		server->sendToClient(client, topicMsg);
	}

	sendMembersList(server, client, channel, channelName);
}

void JoinCmd::sendMembersList(Server* server, Client* client, Channel* channel, const std::string& channelName)
{
	std::string list;
	bool firstMember = true;
	for (const std::string& memberNick : channel->getMembers())
	{
		if (!firstMember)
			list += " ";
		firstMember = false;

		if (channel->isOperator(memberNick))
			list += "@";
		list += memberNick;
	}
	std::string listMsg = ":" + server->getServerName() + " " + RPL_NAMREPLY + " " + client->getNick() + " = " + channelName + " :" + list;
	server->sendToClient(client, listMsg);
	std::string listEndMsg = ":" + server->getServerName() + " " + RPL_ENDOFNAMES + " " + client->getNick() + " " + channelName + " :End of /NAMES list";
	server->sendToClient(client, listEndMsg);
}
