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

void PrivMsgCmd::execute(Server* server, Client* client, const std::vector<std::string>& params, const std::string& multiWordParam)
{
	(void)server;
	(void)client;
	(void)params;
	(void)multiWordParam;
}