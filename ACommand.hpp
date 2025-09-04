#pragma once

#include <iostream>
#include "Server.hpp"
#include "Client.hpp"

class ACommand
{
	protected:

	public:
		ACommand();
		virtual ~ACommand();
		virtual bool needsRegistration() const = 0;
		virtual void execute(Server* server, Client* client, const std::vector<std::string>& params) = 0;
};