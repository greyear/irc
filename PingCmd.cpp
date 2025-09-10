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

void PingCmd::execute(Server* server, Client* client, const std::vector<std::string>& params)
{
	(void)server;
	(void)client;
	(void)params;
}