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
	std::cout << server->getServerName() << std::endl;
	if (params[0] != server->getServerName())
		server->sendError(client, ERR_NOSUCHSERVER, server->getServerName() + " :No such server");
	std::string serverName = server->getServerName();
	std::string pongMsg = ":" + serverName + " PONG " + serverName;
	server->sendToClient(client, pongMsg);
}

/*
PING is sent by server : send token - read the PING using reading buffer, in exec parsed and validated
PONG is sent by client : send token - put the message into write buffer and then send to server fd
server reads PONG with recv 
*/