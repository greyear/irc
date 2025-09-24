#pragma once

#include <iostream>
#include <set>
#include "ACommand.hpp"

class QuitCmd : public ACommand
{
	protected:

	public:
		QuitCmd();
		virtual ~QuitCmd();
		bool needsRegistration() const override;
		void execute(Server* server, Client* client, const std::vector<std::string>& params, const std::string& multiWordParam) override;
		void sendQuitConfirmation(Server* server, Client* client, const std::string& reason);
};