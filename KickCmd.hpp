#pragma once

#include <iostream>
#include "ACommand.hpp"

class KickCmd : public ACommand
{
	public:
		KickCmd();
		virtual ~KickCmd();
		bool	needsRegistration() const override;
		//void	execute(Server* server, Client* client, const std::vector<std::string>& params, const std::string& multiWordParam) override;
};