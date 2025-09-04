#pragma once

#include <iostream>
#include "ACommand.hpp"

class PrivMsgCmd : public ACommand
{
	protected:

	public:
		PrivMsgCmd();
		virtual ~PrivMsgCmd();
		bool needsRegistration() const override;
		void execute(Server* server, Client* client, const std::vector<std::string>& params) override;

};