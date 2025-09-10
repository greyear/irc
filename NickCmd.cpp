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

//TODO: validate nickname itself following documentation

void NickCmd::execute(Server* server, Client* client, const std::vector<std::string>& params, const std::string& multiWordParam)
{
	(void)multiWordParam;
	if (params.empty())
	{
		server->sendError(client->getFd(), ERR_NEEDMOREPARAMS, ":No nickname given");
		return;
	}
	//TODO: check what error's comes first? in case we have both
	/*if (!client->getHasPass())
	{
		server->sendError(client->getFd(), ERR_PASSWDMISMATCH, ":Password incorrect");
		//server->disconnectClient(client->getFd());
		return;
	}*/
	if (server->isNicknameTaken(params[0]))
	{
		server->sendError(client->getFd(), ERR_NICKNAMEINUSE, params[0] + " :Nickname is already in use");
		return;
	}
	if (client->checkRegistrationComplete()) 
	{
		std::string msg = client->getNick() + 
						 " changed his nickname to " + params[0] + "\r\n";
		server->sendInfo(client, msg);
		client->setNick(params[0]);
		return ;
	}
	client->setNick(params[0]);
	client->setHasNick(true);
	if (client->checkRegistrationComplete()) 
	{
		server->sendWelcomeMsg(client);
	}
}
