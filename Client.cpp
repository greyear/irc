#include "Client.hpp"

Client::Client(int fd):
_fd(fd), _nick(""), _user(""), _hasPass(false), _hasUser(false), _hasNick(false), _registered(false)
{
	std::cout << "_fd: "<< _fd << std::endl;
	std::cout << "_registered: " << _registered << std::endl; //TODO clean
}

Client::~Client()
{
    
}

bool	Client::getHasPass() const
{
	return(_hasPass);
}

bool	Client::getHasUser() const
{
	return(_hasUser);
}

bool	Client::getHasNick() const
{
	return(_hasNick);
}

bool	Client::isRegistered() const
{
	return(_registered);
}

void	Client::setHasPass(bool received)
{
	_hasPass = received;
}

void	Client::setHasUser(bool received)
{
	_hasUser = received;
}

void	Client::setHasNick(bool received)
{
	_hasNick = received;
}

void	Client::setRegistered()
{
	_registered = true;
}

void Client::appendToBuffer(const char* data, size_t length)
{
	_messageBuffer.append(data, length);
}

bool Client::hasCompleteMessage() const
{
	return _messageBuffer.find("\r\n") != std::string::npos;
}

std::string Client::extractNextMessage()
{
	size_t pos = _messageBuffer.find("\r\n");
	if (pos == std::string::npos)
	{
		return "";
	}
	std::string message = _messageBuffer.substr(0, pos);
	_messageBuffer.erase(0, pos + 2);
	return message;
}

bool Client::isBufferTooLarge() const
{
	return _messageBuffer.length() > MAX_BUFFER_SIZE;
}

void Client::clearBuffer()
{
	_messageBuffer.clear();
}
