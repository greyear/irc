#pragma once

#include <iostream>
#include <vector>

class Server;
class Client;

class ACommand
{
	protected:

	public:
		ACommand();
		virtual ~ACommand();
		virtual bool needsRegistration() const = 0;
		virtual void execute(Server* server, Client* client, const std::vector<std::string>& params, const std::string& multiWordParam) = 0;
};