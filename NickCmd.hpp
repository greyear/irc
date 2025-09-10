#pragma once

#include <iostream>
#include "ACommand.hpp"

class NickCmd : public ACommand
{
	protected:

	public:
		NickCmd();
		virtual ~NickCmd();
		bool needsRegistration() const override;
		void execute(Server* server, Client* client, const std::vector<std::string>& params, const std::string& multiWordParam) override;

};