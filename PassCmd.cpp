#pragma once

#include "PassCmd.hpp"

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

void PassCmd::execute(Server* server, Client* client, const std::vector<std::string>& params)
{
	if (client->getHasNick() || client->getHasUser())
	{
        server->sendError(client->getFd(), ERR_ALREADYREGISTRED, ":You may not reregister");
        return;
    }

	if (params.empty())
	{
		server->sendError(client->getFd(), ERR_NEEDMOREPARAMS, "PASS :Not enough parameters"); 
	}
}
