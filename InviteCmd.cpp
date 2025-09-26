#include "InviteCmd.hpp"
#include "Client.hpp"
#include "Server.hpp"

InviteCmd::InviteCmd(): _pattern("^[#&][^\\x00-\\x1F\\x7F\\s,:]{1,50}$")
{

}

InviteCmd::~InviteCmd()
{

}

bool InviteCmd::needsRegistration() const
{
	return true;
}

bool InviteCmd::isChannelNameValid(const std::string& channelName)
{
	return std::regex_match(channelName, _pattern);
}

void InviteCmd::execute(Server* server, Client* client, const std::vector<std::string>& params, const std::string& multiWordParam)
{
	(void)multiWordParam;
	if (needsRegistration() && !client->checkRegistrationComplete()) //?
	{
		server->sendError(client, ERR_NOTREGISTERED, " :You have not registered");
		return;
	}
	if (params.size() < 2)
	{
		server->sendError(client, ERR_NEEDMOREPARAMS, " INVITE :Not enough parameters");
		return;
	}
	const std::string& inviteeName = params[0];
	Client* invitee = server->getClientByNick(inviteeName);
	if (!invitee)
	{
		server->sendError(client, ERR_NOSUCHNICK, inviteeName + " :No such nick/channel");
		return;
	}
	const std::string& channelName = params[1];
	if (!isChannelNameValid(channelName))
	{
		server->sendError(client, ERR_BADCHANMASK, channelName + " :Bad Channel Mask");
		return;
	}
	if (!client->isInChannel(channelName))
	{
		server->sendError(client, ERR_NOTONCHANNEL, channelName + " :You're not on that channel");
		return;
	}
	if (invitee->isInChannel(channelName))
	{
		server->sendError(client, ERR_USERONCHANNEL, channelName + " :is already on channel");
		return;
	}
	Channel* channel = server->getChannelByName(channelName);
	if (!channel)
	{
		server->sendError(client, ERR_NOSUCHCHANNEL, channelName + " :No such channel");
		return;
	}
	if (channel->getInviteOnly() && !channel->isOperator(client->getNick()))
	{
		server->sendError(client, ERR_CHANOPRIVSNEEDED, channelName + " :You're not channel operator");
		return;
	}
	if (channel->isInvited(invitee->getNick()))
	{
		server->sendError(client, ERR_USERALREADYINVITED, inviteeName + " " + channelName + " :is already invited");
		return;
	}
	channel->_invitedUsers.insert(invitee->getNick());
	sendInviteConfirmation(server, client, channel, channel->getName());
}

void InviteCmd::sendInviteConfirmation(Server* server, Client* client, Channel* channel, const std::string& channelName)
{
	std::string InviteMsg = ":" + client->getFullIdentifier() + " Invite " + channelName + "\r\n";
	
	for (const std::string& memberNick : channel->getMembers())
	{
		Client* member = server->getClientByNick(memberNick);
		if (member)
			server->sendToClient(member, InviteMsg);
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

void InviteCmd::sendMembersList(Server* server, Client* client, Channel* channel, const std::string& channelName)
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
