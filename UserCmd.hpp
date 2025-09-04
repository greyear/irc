#pragma once

#include <iostream>
#include "ACommand.hpp"

class UserCmd : public ACommand
{
	protected:

	public:
		UserCmd();
		virtual ~UserCmd();
		bool needsRegistration() const override;
		void execute(Server* server, Client* client, const std::vector<std::string>& params) override;

};