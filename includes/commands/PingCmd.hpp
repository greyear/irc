#pragma once

#include <iostream>
#include "ACommand.hpp"

class PingCmd : public ACommand
{
	public:
		PingCmd();
		virtual ~PingCmd();

		bool needsRegistration() const override;
		void execute(Server* server, Client* client, const std::vector<std::string>& params, const std::string& multiWordParam) override;
};