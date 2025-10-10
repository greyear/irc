#include "../../includes/commands/PassCmd.hpp"
#include "../../includes/Client.hpp"
#include "../../includes/Server.hpp"

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

	if (client->checkRegistrationComplete())
	{
		server->sendError(client, ERR_ALREADYREGISTRED, ":You may not reregister");
		return;
	}
	if (params.empty())
	{
		server->sendError(client, ERR_NEEDMOREPARAMS, "PASS :Not enough parameters");
		return;
	}
	if (params[0] != server->getPass())
	{
		server->sendError(client, ERR_PASSWDMISMATCH, ":Password incorrect");
		return;
	}
	client->setHasPass(true);
}
