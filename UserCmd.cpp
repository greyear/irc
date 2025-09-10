#include "UserCmd.hpp"
#include "Client.hpp"
#include "Server.hpp"

UserCmd::UserCmd()
{

}

UserCmd::~UserCmd()
{

}

bool UserCmd::needsRegistration() const
{
	return false;
}

//TODO: userValidation method

void UserCmd::execute(Server* server, Client* client, const std::vector<std::string>& params, const std::string& multiWordParam)
{
	if ((params.size() < 3) || (params.size() == 3 && multiWordParam.empty()))
	{
		server->sendError(client->getFd(), ERR_NEEDMOREPARAMS, "USER :Not enough parameters");
		return;
	}
	//TODO: check what error's comes first? in case we have both
	if (client->checkRegistrationComplete()) 
	{
		server->sendError(client->getFd(), ERR_ALREADYREGISTRED, ":You may not reregister");
		return;
	}

	std::string userName; //TODO: recheck about 0 and *
	userName = params[0];
	std::string realName;
	if (params.size() == 4)
		realName = params[3];
	else
		realName = multiWordParam;
	
	client->setUser(userName);
	client->setHasUser(true);
	client->setRealName(realName);
	
	if (client->checkRegistrationComplete()) 
	{
		server->sendWelcomeMsg(client);
	}
}
