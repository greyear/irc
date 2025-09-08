#include "NickCmd.hpp"
#include "Client.hpp"
#include "Server.hpp"

NickCmd::NickCmd()
{

}
NickCmd::~NickCmd()
{

}

bool NickCmd::needsRegistration() const
{
	return false;
}

void NickCmd::execute(Server* server, Client* client, const std::vector<std::string>& params)
{
	if (params.empty())
	{
		server->sendError(client->getFd(), ERR_NEEDMOREPARAMS, ":No nickname given");
		return;
	}
	if (!client->getHasPass())
	{
		server->sendError(client->getFd(), ERR_PASSWDMISMATCH, ":Password incorrect");
		server->disconnectClient(client->getFd());
		return;
	}
	
	if (server->isNicknameTaken(params[0]))
	{
		server->sendError(client->getFd(), ERR_NICKNAMEINUSE, params[0] + " :Nickname is already in use");
		return;
	}
	//TODO: validate nickname itself following documentation
	client->setNick(params[0]);
	client->setHasNick(true);
	if (client->checkRegistrationComplete()) 
	{
		server->sendWelcomeMsg(client);
	}
}
