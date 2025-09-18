#include "JoinCmd.hpp"
#include "Client.hpp"
#include "Server.hpp"

std::vector<std::string> splitTargets(const std::string& targets);

JoinCmd::JoinCmd(): _pattern("^[#&][^\\x00-\\x1F\\x7F\\s,:]{1,50}$")
{

}

JoinCmd::~JoinCmd()
{

}

bool JoinCmd::needsRegistration() const
{
	return true;
}

bool JoinCmd::isChannelNameValid(const std::string& channelName)
{
	return std::regex_match(channelName, _pattern);
}

void JoinCmd::execute(Server* server, Client* client, const std::vector<std::string>& params, const std::string& multiWordParam)
{
	if (needsRegistration() && !client->checkRegistrationComplete())
	{
		server->sendError(client, ERR_NOTREGISTERED, " :You have not registered");
		return;
	}

	if (params.empty())
	{
		server->sendError(client, ERR_NEEDMOREPARAMS, " JOIN :Not enough parameters");
		return;
	}

	std::vector<std::string> channels = splitTargets(params[0]);
	if (channels.empty())
	{
		server->sendError(client, ERR_NEEDMOREPARAMS, " JOIN :Not enough parameters");
		return;
	}
	std::vector<std::string> keys;
	if (params.size() > 1 && !params[1].empty())
		keys = splitTargets(params[1]);
	
	for (size_t i = 0; i < channels.size(); ++i)
	{
		const std::string& channelName = channels[i];
		std::string key = (i < keys.size()) ? keys[i] : "";
		if (!isChannelNameValid(channelName))
		{
			server->sendError(client, ERR_BADCHANMASK, channelName + " :Bad Channel Mask");
			continue ;
		}
		if (client->isInChannel(channelName))
		{
			server->sendError(client, ERR_USERONCHANNEL, client->getNick() + channelName + " :is already on channel");
			continue ;
		}
		Channel* channel = server->getOrCreateChannel(channelName);
		
		//chech if user's already in the channel
		//check if channel's invite only and !channel->isInvited(client->getNick())
		//channel has user limit and it's full
		//channel requires key and wrong/no key provided
		//Too many channels (most servers limit to ~20-100 channels per user)
		//join
	}
		
}

/*
modes
keys

*/
