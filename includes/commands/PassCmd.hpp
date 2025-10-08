#pragma once

#include <iostream>
#include "ACommand.hpp"

class PassCmd : public ACommand
{
	public:
		PassCmd();
		virtual ~PassCmd();
		bool needsRegistration() const override;
		void execute(Server* server, Client* client, const std::vector<std::string>& params, const std::string& multiWordParam) override;
};