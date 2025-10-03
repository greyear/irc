#include "TopicCmd.hpp"
#include "Client.hpp"
#include "Server.hpp"

TopicCmd::TopicCmd()
{

}
TopicCmd::~TopicCmd()
{

}
bool TopicCmd::needsRegistration() const
{
	return (true);
}
void TopicCmd::execute(Server* server, Client* client, const std::vector<std::string>& params, const std::string& multiWordParam)
{
	if (needsRegistration() && !client->checkRegistrationComplete())
	{
		server->sendError(client, ERR_NOTREGISTERED, " :You have not registered");
		return;
	}
	if (params.size() < 1)
	{
		server->sendError(client, ERR_NEEDMOREPARAMS, " TOPIC :Not enough parameters");
		return;
	}
	const std::string& channelName = params[0];
	const std::string& clientNick = client->getNick();
	Channel* channel = server->getChannelByName(channelName);

	if (!channel)
	{
		server->sendError(client, ERR_NOSUCHCHANNEL, clientNick + " " + channelName + " :No such channel");
		return;
	}
	if (!client->isInChannel(channelName))
	{
		server->sendError(client, ERR_NOTONCHANNEL, clientNick + " " + channelName + " :You're not on that channel");
		return;
	}

    // (TOPIC #channel)
    if (multiWordParam.empty() && params.size() < 2)
    {
        const std::string &topic = channel->getTopic();
        if (topic.empty())
        {
            server->sendError(client, RPL_NOTOPIC, clientNick + " " + channelName + " :No topic is set");
        }
        else
        {
            std::string topicMsg =  ":" + server->getServerName() + " " + RPL_TOPIC + " " + clientNick + " " + channelName + " :" + topic;
            server->sendToClient(client, topicMsg);
         
			// RPL_TOPICWHOTIME
			if (channel->getTopicTime() != 0)
			{
				std::string topicWhoMsg;
				topicWhoMsg =  ":" + server->getServerName() + " " + RPL_TOPICWHOTIME + " " + clientNick + " " + channelName + " "
							+ channel->getTopicSetter() + " " + std::to_string(channel->getTopicTime());
				server->sendToClient(client, topicWhoMsg);
			}
        }
        return;
    }
	if (channel->getTopicRestricted() && !channel->isOperator(clientNick))
	{
		server->sendError(client, ERR_CHANOPRIVSNEEDED, clientNick + " " + channelName + " :You're not channel operator");
		return;
	}

	std::string newTopic;
	for (size_t i = 1; i < params.size(); i++)
	{
		if (!newTopic.empty())
			newTopic += " ";
		newTopic += params[i];
	}
	if (!multiWordParam.empty())
	{
		if (!newTopic.empty())
			newTopic += " ";
		newTopic += multiWordParam;
	}
	channel->setTopic(newTopic, client->getFullIdentifier());

	std::string topicMsg = ":" + client->getFullIdentifier() + " TOPIC " + channelName + " :" + newTopic;
	for (const std::string& memberNick : channel->getMembers())
	{
		Client* memberClient = server->getClientByNick(memberNick);
		if (memberClient)
		{
			server->sendToClient(memberClient, topicMsg);
		}
	}
}
