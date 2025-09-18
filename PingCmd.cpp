#include "PingCmd.hpp"
#include "Client.hpp"
#include "Server.hpp"

PingCmd::PingCmd()
{

}

PingCmd::~PingCmd()
{

}

bool PingCmd::needsRegistration() const
{
	return true;
}

void PingCmd::execute(Server* server, Client* client, const std::vector<std::string>& params, const std::string& multiWordParam)
{
	(void)multiWordParam;
	if (params.empty())
	{
		server->sendError(client, ERR_NEEDMOREPARAMS, "PING :Not enough parameters");
		return;
	}
	if (params[0] != server->getServerName())
		server->sendError(client, ERR_NOSUCHSERVER, server->getServerName() + " :No such server");
	std::string serverName = server->getServerName();
	std::string pongMsg = ":" + serverName + " PONG " + serverName;
	server->sendToClient(client, pongMsg);
}
