
#include "PassCmd.hpp"
#include "Client.hpp"
#include "Server.hpp"

PassCmd::PassCmd()
{

}

PassCmd::~PassCmd()
{

}

bool PassCmd::needsRegistration() const
{
	return false;
}

void PassCmd::execute(Server* server, Client* client, const std::vector<std::string>& params, const std::string& multiWordParam)
{
	(void)multiWordParam;
	//std::cout << "pass exec called" << std::endl;
	if (client->getHasPass())
	{
        server->sendError(client->getFd(), ERR_ALREADYREGISTRED, ":You may not reregister");
        return;
    }
	if (params.empty())
	{
		server->sendError(client->getFd(), ERR_NEEDMOREPARAMS, "PASS :Not enough parameters");
		return;
	}
	
	//std::cout << "param0" << params[0] << std::endl;
	//std::cout << "getPass" << server->getPass() << std::endl;
	if (params[0] != server->getPass())
	{
		//std::cout << "before sendError" << std::endl;
		server->sendError(client->getFd(), ERR_PASSWDMISMATCH, ":Password incorrect");
		//std::cout << "before disconnecting" << std::endl; 
		//server->disconnectClient(client->getFd());
		return;
	}
	//std::cout << "getPass" << server->getPass() << std::endl;
	client->setHasPass(true);
}
