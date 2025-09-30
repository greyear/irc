#include "Channel.hpp"

Channel::Channel(const std::string& name)
	: _name(name), _inviteOnly(false), /* _topicRestricted(false), */ _keyRequired(false), _hasLimit(false), _limit(0)
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

bool	Channel::getKeyRequired() const
{
	return _keyRequired;
}

const std::string&	Channel::getChannelKey() const
{
	return _channelKey;
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