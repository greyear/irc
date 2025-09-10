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

void UserCmd::execute(Server* server, Client* client, const std::vector<std::string>& params)
{
	(void)server;
	(void)client;
	(void)params;
}