#pragma once
#include <iostream>
#include "ACommand.hpp"
#include <sstream>


class TopicCmd : public ACommand
{
	public:
		TopicCmd();
		~TopicCmd(); //TODO : remove virtual froom all other child classes
		bool needsRegistration() const override;
		void execute(Server* server, Client* client, const std::vector<std::string>& params, const std::string& multiWordParam) override;
};