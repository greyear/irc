#include "../../includes/commands/PrivMsgCmd.hpp"
#include "../../includes/Client.hpp"
#include "../../includes/Server.hpp"

std::vector<std::string> splitTargets(const std::string& targets);

PrivMsgCmd::PrivMsgCmd()
{

}

PrivMsgCmd::~PrivMsgCmd()
{

}

bool PrivMsgCmd::needsRegistration() const
{
	return true;
}

void PrivMsgCmd::execute(Server* server, Client* client, const std::vector<std::string>& params, const std::string& multiWordParam)
{
	if (needsRegistration() && !client->checkRegistrationComplete())
	{
		server->sendError(client, ERR_NOTREGISTERED, " :You have not registered");
		return;
	}

	if (params.empty())
	{
		server->sendError(client, ERR_NORECIPIENT, " :No recipient given (PRIVMSG)");
		return;
	}
	std::string message;
	if (params.size() > 1)
	{
		for (size_t i = 1; i < params.size(); ++i)
		{
			if (i > 1)
				message += " ";
			message += params[i];
		}
		if (!multiWordParam.empty())
		{
			if (!message.empty())
				message += " ";
			message += multiWordParam;
		}
	}
	else if (!multiWordParam.empty())
	{
		message = multiWordParam;
	}
	else
	{
		server->sendError(client, ERR_NOTEXTTOSEND, " :No text to send");
		return;
	}
	std::vector<std::string> targets = splitTargets(params[0]);
	if (targets.empty())
	{
		server->sendError(client, ERR_NORECIPIENT, " :No recipient given (PRIVMSG)");
		return;
	}
	if (message.length() > MAX_PRIVMSG_TXT)
	{
		message = message.substr(0, MAX_PRIVMSG_TXT);
	}
	for (const std::string& target : targets)
	{
		if (!target.empty() && (target[0] == '#' || target[0] == '&'))
		{
			Channel *targetChan = server->getChannelByName(target);
			if (!targetChan)
			{
				server->sendError(client, ERR_NOSUCHNICK, target + " :No such nick/channel");
				continue;
			}
				if (!targetChan->isMember(client->getNick()))
				{
					server->sendError(client, ERR_CANNOTSENDTOCHAN, target + " :Cannot send to channel");
					continue;
				}
			std::string channelMsg = ":" + client->getFullIdentifier() + " PRIVMSG " + target + " :" + message;
			sendToChanMembers(targetChan, channelMsg, client, server);
		}
		else
		{
			Client* targetClient = server->getClientByNick(target);
			if (!targetClient)
			{
				server->sendError(client, ERR_NOSUCHNICK, target + " :No such nick/channel");
				continue;
			}
			
			std::string privMsg = ":" + client->getFullIdentifier() + " PRIVMSG " + target + " :" + message;
			server->sendToClient(targetClient, privMsg);
		}
	}
}

void PrivMsgCmd::sendToChanMembers(Channel *targetChan, const std::string& channelMsg, Client *client, Server *server)
{
	const std::set<std::string>& members = targetChan->getMembers();

	for (const std::string& memberNick : members)
	{
		if (memberNick == client->getNick())
			continue;
		
		Client* member = server->getClientByNick(memberNick);
		if (member)
		{
			server->sendToClient(member, channelMsg);
		}
	}
}
