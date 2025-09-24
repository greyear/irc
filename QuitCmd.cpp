#include "QuitCmd.hpp"
#include "Client.hpp"
#include "Server.hpp"

QuitCmd::QuitCmd()
{

}
QuitCmd::~QuitCmd()
{

}
bool QuitCmd::needsRegistration() const
{
	return (false);
}
void QuitCmd::execute(Server* server, Client* client, const std::vector<std::string>& params, const std::string& multiWordParam)
{
	std::string reason;
    if (params.size() > 1)
	{
        for (size_t i = 1; i < params.size(); ++i)
		{
            if (i > 1)
				reason += " ";
            reason += params[i];
        }
        if (!multiWordParam.empty())
		{
            if (!reason.empty())
				reason += " ";
            reason += multiWordParam;
        }
    }
	else if (!multiWordParam.empty())
	{
        reason = multiWordParam;
    }
	sendQuitConfirmation(server, client, reason);
	server->removeClient(client);
}

void QuitCmd::sendQuitConfirmation(Server* server, Client* client, const std::string& reason)
{
	std::set<Client*> recipients;
    for (const std::string& channelName : client->getChannels())
	{
        Channel* channel = server->getChannelByName(channelName);
        if (channel)
		{
            for (const std::string& memberNick : channel->getMembers())
			{
                if (memberNick != client->getNick())
				{
                    Client* member = server->getClientByNick(memberNick);
                    if (member)
						recipients.insert(member);
                }
            }
        }
    }

	std::string quitMsg = ":" + client->getNick() + " QUIT";
	if (!reason.empty())
	{
		quitMsg += " :" + reason;
	}

	for (Client* recipient : recipients)
	{
		server->sendToClient(recipient, quitMsg);
	}
}