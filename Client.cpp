#include "Client.hpp"

Client::Client(int fd):
	_fd(fd), _nick(""), _user(""), _hasUnsentData(false), _isEpollOutActive(false),
	_hasPass(false), _hasUser(false), _hasNick(false), _registered(false), _channelLimit(CHANNEL_LIMIT)
{
	//std::cout << "_fd: "<< _fd << std::endl;
	//std::cout << "_registered: " << _registered << std::endl; //TODO clean
}

Client::~Client()
{
    
}

int	Client::getFd() const
{
	return _fd;
}

const std::string&	Client::getNick() const
{
	return _nick;
}

const std::string&	Client::getUser() const
{
	return _user;
}

const std::string&	Client::getRealName() const
{
	return _realName;
}

const std::string&	Client::getHostName() const
{
	return _hostName;
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

uint	Client::getChannelLimit() const
{
	return(_channelLimit);
}

const std::set<std::string>&	Client::getChannels() const
{
	return (_channels);
}

std::string Client::getFullIdentifier() const
{
	return (_nick + "!" + _user + "@" + _hostName);
}

bool	Client::isRegistered() const
{
	return(_registered);
}

bool Client::hasUnsentData() const
{
    return _hasUnsentData;
}

bool	Client::isEpollOutActive() const
{
	return _isEpollOutActive;
}

void	Client::setNick(const std::string& nick)
{
	_nick = nick;
}

void	Client::setUser(const std::string& user)
{
	_user = user;
}

void	Client::setRealName(const std::string& realName)
{
	_realName = realName;
}

void	Client::setHostName(const std::string& hostName)
{
	_hostName = hostName;
}

void	Client::setHasPass(bool received)
{
	//std::cout << "setHasPass" << std::endl;
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

void	Client::setIsEpollOutActive(bool value)
{
	_isEpollOutActive = value;
}

bool	Client::checkRegistrationComplete()
{
	return (_hasPass && _hasUser && _hasNick);
}

void Client::appendToReadBuffer(const char* data, size_t length)
{
	_readBuffer.append(data, length);
}

void Client::appendToWriteBuffer(const std::string& msg)
{
	_writeBuffer += msg;
	_hasUnsentData = true;
}

bool Client::hasCompleteMessage() const
{
	return _readBuffer.find("\r\n") != std::string::npos;
}

std::string Client::extractNextMessage()
{
	size_t pos = _readBuffer.find("\r\n");
	if (pos == std::string::npos)
	{
		return "";
	}
	std::string message = _readBuffer.substr(0, pos);
	_readBuffer.erase(0, pos + 2);
	return message;
}

bool Client::isBufferTooLarge() const
{
	return _readBuffer.length() > MAX_BUFFER_SIZE;
}

void Client::clearBuffer()
{
	_readBuffer.clear();
}

int	Client::flushWriteBuffer()
{
	if (_writeBuffer.empty())
	{
		_hasUnsentData = false;
		return FLUSH_SUCCESS;
	}
	ssize_t sent = send(_fd, _writeBuffer.c_str(), _writeBuffer.size(), MSG_DONTWAIT);
	if (sent > 0)
	{
		_writeBuffer.erase(0, sent);
		if (_writeBuffer.empty())
		{
			_hasUnsentData = false;
			return FLUSH_SUCCESS;
		}
		return FLUSH_PARTIAL;
	}
	else if (sent == 0 || (sent == -1 && (errno == EAGAIN || errno == EWOULDBLOCK)))
		return FLUSH_LATER;
	return FLUSH_ERROR;
}

bool Client::isInChannel(const std::string& channelName) const
{
	return _channels.find(channelName) != _channels.end();
}

void	Client::addChannel(const std::string& name)
{
	_channels.insert(name);
}

void	Client::leaveChannel(const std::string& name)
{
	_channels.erase(name);
}