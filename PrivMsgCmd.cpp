#include "PrivMsgCmd.hpp"
#include "Client.hpp"
#include "Server.hpp"

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

std::vector<std::string> splitTargets(const std::string& targets)
{
	std::vector<std::string> result;
	std::stringstream ss(targets);
	std::string eachTarget;
	
	while (std::getline(ss, eachTarget, ',')) //reads from ss to eachTarget till , or the end of ss
	{
		//TODO: do we need to handle "nick1,nick2, nick3" as nc does OR parse all 3?
		eachTarget.erase(0, eachTarget.find_first_not_of(" \t")); //removes leading spaces
		eachTarget.erase(eachTarget.find_last_not_of(" \t") + 1); //removes following spaces
		
		if (!eachTarget.empty())
			result.push_back(eachTarget);
	}
	return result;
}

void PrivMsgCmd::execute(Server* server, Client* client, const std::vector<std::string>& params, const std::string& multiWordParam)
{
	if (params.empty())
	{
		server->sendError(client->getFd(), ERR_NORECIPIENT, " :No recipient given (PRIVMSG)");
		return;
	}

	std::string message;
	if (!multiWordParam.empty())
		message = multiWordParam;
	else if (params.size() > 1)
		message = params[1];
	else
	{
		server->sendError(client->getFd(), ERR_NOTEXTTOSEND, " :No text to send");
		return;
	}
	
	std::vector<std::string> targets = splitTargets(params[0]);
	if (targets.empty())
	{
		server->sendError(client->getFd(), ERR_NORECIPIENT, " :No recipient given (PRIVMSG)");
		return;
	}

	for (const std::string& targetNick : targets)
	{
		Client* targetClient = server->getClientByNick(targetNick);
		if (targetClient == nullptr)
		{
			server->sendError(client->getFd(), ERR_NOSUCHNICK, client->getNick() + " :No such nick/channel");
			continue;
		}
		
		std::string privMsg = ":" + client->getNick() + "!" + client->getUser() + "@" + client->getHostName() + " PRIVMSG " + targetNick + " :" + message;
		server->sendToClient(targetClient, privMsg);
	}
}
