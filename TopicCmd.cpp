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
void TopicCmd::execute(Server* server, Client* client, const std::vector<std::string>& params, const std::string& multiWordParam) override
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
	const std::string& clientNick = getNick();
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
	if (channel->getTopicRestricted() && !channel->isOperator(clientNick))
	{
		server->sendError(client, ERR_CHANOPRIVSNEEDED, clientNick + " " + channelName + " :You're not channel operator");
		return;
	}
}

// topic #newchan
//15:25 -!- No topic set for #newchan


// #newchanalalalala: No such channel
//-!- #newchan You're not a channel operator

//nc
//TOPIC #newchan NEWtopichere
//:uranium.libera.chat 482 newnick4 #newchan :You're not a channel operator

//nc 
//TOPIC #newchan
//:uranium.libera.chat 332 newnick4 #newchan :some cool topic here
//:uranium.libera.chat 333 newnick4 #newchan newnick2!~fsolomon@194.136.126.51 1759235429
