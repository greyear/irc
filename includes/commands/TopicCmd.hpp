#pragma once
#include <iostream>
#include "ACommand.hpp"
#include "../Channel.hpp"

class TopicCmd : public ACommand
{
	public:
		TopicCmd();
		virtual ~TopicCmd();
		bool needsRegistration() const override;
		void execute(Server* server, Client* client, const std::vector<std::string>& params, const std::string& multiWordParam) override;
		void sendTopicNotification(Server* server, Client* client, Channel* channel, const std::string& newTopic);
};