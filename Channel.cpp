#include "Channel.hpp"

Channel::Channel(const std::string& name)
	: _name(name), _inviteOnly(false), _topicRestricted(true),
	 _keyRequired(false), _hasLimit(false), _limit(0), _topicTime(0),
	 _creationTime(std::time(NULL))
{
}

Channel::~Channel()
{
}

const std::string& Channel::getName() const
{
	return (_name);
}

const std::string&	Channel::getTopic() const
{
	return _topic;
}

bool Channel::getInviteOnly() const
{
	return (_inviteOnly);
}

bool	Channel::getTopicRestriction() const
{
	return (_topicRestricted);
}

bool	Channel::getHasLimit() const
{
	return _hasLimit;
}

uint	Channel::getLimit() const
{
	return _limit;
}

const std::set<std::string>& Channel::getMembers() const
{
	return _members;
}

bool	Channel::getTopicRestricted() const
{
	return _topicRestricted;
}

bool	Channel::getKeyRequired() const
{
	return _keyRequired;
}

const std::string&	Channel::getChannelKey() const
{
	return _channelKey;
}

const	std::string& Channel::getTopicSetter() const
{
	return _topicSetter;
}

time_t	Channel::getTopicTime() const
{
	return _topicTime;
}

void	Channel::setTopic(const std::string &topic, const std::string &topicSetter)
{
	_topic = topic;
	_topicSetter = topicSetter;
	_topicTime = time(NULL);
}

std::string	Channel::getModestring() const
{
	std::string modestring = "+";
	std::string args;

	if (_keyRequired)
	{
		modestring += "k";
		args += " " + getChannelKey();
	}
	if (_hasLimit)
	{
		modestring += "l";
		args += " " + std::to_string(getLimit());
	}
	if (_inviteOnly)
		modestring += "i";
	if (_topicRestricted)
		modestring += "t";
	if (!args.empty())
		modestring += args;
	return (modestring);
}

time_t	Channel::getCreationTime() const
{
	return (_creationTime);
}

////

void	Channel::setTopicRestriction(bool topicRestrictionActive)
{
	_topicRestricted = topicRestrictionActive;
}

void	Channel::setInviteOnly(bool inviteOnlyActive)
{
	_inviteOnly = inviteOnlyActive;
}

void	Channel::setLimit(uint limit)
{
	_hasLimit = true;
	_limit = limit;
}

void	Channel::removeLimit()
{
	_hasLimit = false;
}

void	Channel::setKey(const std::string &newKey)
{
	_keyRequired = true;
	_channelKey = newKey;
}

void	Channel::removeKey()
{
	_keyRequired = false;
}

bool Channel::isEmpty()
{
	return (_members.empty());
}

bool	Channel::isOperator(const std::string &nick)
{
	return _operators.find(nick) != _operators.end();
}

bool	Channel::isMember(const std::string &nick)
{
	return _members.find(nick) != _members.end();
}

bool	Channel::isInvited(const std::string &nick)
{
	return _invitedUsers.find(nick) != _invitedUsers.end();
}

void	Channel::addMember(const std::string &nick)
{
	_members.insert(nick);
}

void	Channel::addOperator(const std::string &nick)
{
	_operators.insert(nick);
}

void	Channel::addInvited(const std::string &nick)
{
	_invitedUsers.insert(nick);
}

void	Channel::removeMember(const std::string &nick)
{
	_members.erase(nick);
}

void	Channel::removeOperator(const std::string &nick)
{
	_operators.erase(nick);
}
void	Channel::removeInvited(const std::string &nick)
{
	_invitedUsers.erase(nick);
}

void	Channel::updateNickChange(const std::string &oldNick, const std::string &newNick)
{
	if (isMember(oldNick))
	{
		_members.erase(oldNick);
		_members.insert(newNick);
	}
	if (isOperator(oldNick))
	{
		_operators.erase(oldNick);
		_operators.insert(newNick);
	}
	if (isInvited(oldNick))
	{
		_invitedUsers.erase(oldNick);
		_invitedUsers.insert(newNick);
	}
}