#include "../../includes/commands/InviteCmd.hpp"
#include "../../includes/Client.hpp"
#include "../../includes/Server.hpp"

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
	if (needsRegistration() && !client->checkRegistrationComplete())
	{
		server->sendError(client, ERR_NOTREGISTERED, ":You have not registered");
		return;
	}
	if (params.size() < 2)
	{
		server->sendError(client, ERR_NEEDMOREPARAMS, "INVITE :Not enough parameters");
		return;
	}

	const std::string& inviteeName = params[0];
	const std::string& channelName = params[1];
	if (!isChannelNameValid(channelName))
	{
		server->sendError(client, ERR_BADCHANMASK, channelName + " :Bad Channel Mask");
		return;
	}

	Client* invitee = server->getClientByNick(inviteeName);
	if (!invitee)
	{
		server->sendError(client, ERR_NOSUCHNICK, inviteeName + " :No such nick/channel");
		return;
	}
	if (!invitee->checkRegistrationComplete())
	{
    	server->sendError(client, ERR_NOSUCHNICK, inviteeName + " :No such nick/channel");
    	return;
	}
	
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
	if (!channel->isOperator(client->getNick()))
	{
		server->sendError(client, ERR_CHANOPRIVSNEEDED, channelName + " :You're not channel operator");
		return;
	}

	if (invitee->isInChannel(channelName))
	{
		server->sendError(client, ERR_USERONCHANNEL, inviteeName + " " + channelName + " :is already on channel");
		return;
	}

	channel->addInvited(invitee->getNick());
	sendInviteConfirmations(server, client, invitee, channel);
}

void InviteCmd::sendInviteConfirmations(Server* server, Client* inviter, Client* invitee, Channel* channel)
{
	std::string msgToInviter = ":" + server->getServerName() + " " + RPL_INVITING + " " + inviter->getNick() + " " + invitee->getNick() + " " + channel->getName() + "\r\n";
	server->sendToClient(inviter, msgToInviter);

	std::string msgToInvitee = ":" + inviter->getFullIdentifier() + " INVITE " + invitee->getNick() + " :" + channel->getName() + "\r\n";
	server->sendToClient(invitee, msgToInvitee);
}
