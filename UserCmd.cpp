#include "UserCmd.hpp"
#include "Client.hpp"
#include "Server.hpp"

UserCmd::UserCmd(): _userNamePattern("^[A-Za-z0-9\\-_[\\]\\{\\}\\|^]{1,9}$"),
    _realNamePattern("^[^\\x00-\\x1F]{1,512}$")
{

}

UserCmd::~UserCmd()
{

}

bool UserCmd::needsRegistration() const
{
	return false;
}

bool UserCmd::isUserNameValid(const std::string& userName)
{
	return std::regex_match(userName, _userNamePattern);
}

bool UserCmd::isRealNameValid(const std::string& realName)
{
	return std::regex_match(realName, _realNamePattern);
}

void UserCmd::execute(Server* server, Client* client, const std::vector<std::string>& params, const std::string& multiWordParam)
{
	if (client->checkRegistrationComplete()) 
	{
		server->sendError(client, ERR_ALREADYREGISTRED, ":You may not reregister");
		return;
	}
	if ((params.size() < 3) || (params.size() == 3 && multiWordParam.empty()))
	{
		server->sendError(client, ERR_NEEDMOREPARAMS, "USER :Not enough parameters");
		return;
	}

	if (params.size() > 3 && !multiWordParam.empty())
	{
		std::string badUser = "";
		for (size_t i = 0; i < params.size() - 2; ++i)
		{
			//std::cout << badUser
			if (i != 0)
				badUser = badUser + ' ';
			badUser = badUser + params[i];
		}
		server->sendError(client, ERR_ERRONEUSNICKNAME, badUser + " :Erroneus nickname");
		return;
	}


	std::string userName; //TODO: recheck about 0 and *
	userName = params[0];
	std::string realName;
	if (params.size() == 4 && multiWordParam.empty())
		realName = params[3];
	else
		realName = multiWordParam;
	
	if (!isUserNameValid(userName))
	{
		server->sendError(client, ERR_ERRONEUSNICKNAME, userName + " :Erroneus nickname");
		return;
	}
	if (!isRealNameValid(realName))
	{
		server->sendError(client, ERR_ERRONEUSNICKNAME, realName + " :Erroneus nickname");
		return;
	}

	client->setUser(userName);
	client->setHasUser(true);
	client->setRealName(realName);
	
	if (client->checkRegistrationComplete()) 
	{
		server->sendWelcomeMsg(client);
	}
}
