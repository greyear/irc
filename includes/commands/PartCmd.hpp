#pragma once

#include <iostream>
#include "ACommand.hpp"
#include "../Channel.hpp"

class PartCmd : public ACommand
{
	public:
		PartCmd();
		virtual ~PartCmd();

		bool	needsRegistration() const override;
		void	execute(Server* server, Client* client, const std::vector<std::string>& params, const std::string& multiWordParam) override;
		void	partChannel(Client* client, Channel* channel);
		void	sendPartConfirmation(Server* server, Client* client, Channel* channel, const std::string& reason);
};