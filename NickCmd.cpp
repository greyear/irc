#include "NickCmd.hpp"
#include "Client.hpp"
#include "Server.hpp"

NickCmd::NickCmd(): _pattern("^[a-zA-Z\\[\\]\\\\`_^{|}][a-zA-Z0-9\\[\\]\\\\`_^{|}-]{0,8}$")
{

}
NickCmd::~NickCmd()
{

}

bool NickCmd::needsRegistration() const
{
	return false;
}

bool NickCmd::isNickValid(const std::string& nick)
{
	return std::regex_match(nick, _pattern);
}

void NickCmd::execute(Server* server, Client* client, const std::vector<std::string>& params, const std::string& multiWordParam)
{
	(void)multiWordParam;
	if (params.empty())
	{
		server->sendError(client, ERR_NEEDMOREPARAMS, ":No nickname given");
		return;
	}

	if (!isNickValid(params[0]))
	{
		server->sendError(client, ERR_ERRONEUSNICKNAME, params[0] + " :Erroneus nickname");
		return;
	}

	if (server->isNicknameTaken(params[0]))
	{
		server->sendError(client, ERR_NICKNAMEINUSE, params[0] + " :Nickname is already in use");
		return;
	}

	if (client->checkRegistrationComplete()) 
	{
		const std::string& oldNick = client->getNick();
		const std::string& newNick = params[0];

		std::string nickChangeMsg = ":" + client->getFullIdentifier() + " NICK :" + newNick + "\r\n";
		server->sendToClient(client, nickChangeMsg);
		notifyAndUpdateChannels(client, server, nickChangeMsg, oldNick, newNick);
		client->setNick(newNick);
		return ;
	}
	client->setNick(params[0]);
	client->setHasNick(true);
	if (client->checkRegistrationComplete()) 
	{
		server->sendWelcomeMsg(client);
	}
}

void	NickCmd::notifyAndUpdateChannels(Client *client, Server *server, const std::string &nickChangeMsg, const std::string &oldNick, const std::string &newNick)
{
	std::set<Client*> recipients;
    for (const std::string& channelName : client->getChannels())
	{
        Channel* channel = server->getChannelByName(channelName);
		if (!channel)
			continue;
		for (const std::string& memberNick : channel->getMembers())
		{
			if (memberNick != oldNick)
			{
				Client* member = server->getClientByNick(memberNick);
				if (member)
					recipients.insert(member);
			}
		}
		channel->updateNickChange(oldNick, newNick);
    }
	for (Client* recipient : recipients)
	{
		server->sendToClient(recipient, nickChangeMsg);
	}
}