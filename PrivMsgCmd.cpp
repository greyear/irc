#include "PrivMsgCmd.hpp"
#include "Client.hpp"
#include "Server.hpp"

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
	if (!multiWordParam.empty())
		message = multiWordParam;
	else if (params.size() > 1)
		message = params[1];
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
   		message = message.substr(0, MAX_PRIVMSG_TXT); //TODO: check the message construction!
	}
	for (const std::string& targetNick : targets)
	{
		Client* targetClient = server->getClientByNick(targetNick);
		if (targetClient == nullptr)
		{
			server->sendError(client, ERR_NOSUCHNICK, " :No such nick/channel");
			continue;
		}
		
		std::string privMsg = ":" + client->getNick() + "!" + client->getUser() + "@" + client->getHostName() + " PRIVMSG " + targetNick + " :" + message;
		server->sendToClient(targetClient, privMsg);
	}
}
