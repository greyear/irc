#include "Channel.hpp"

Channel::Channel(const std::string& name)
	: _name(name), _inviteOnly(false), _topicRestricted(false), _passRequired(false), _hasLimit(false), _limit(0)
{

}

Channel::~Channel()
{

}

const std::string& Channel::getName()
{
	return (_name);
}

bool Channel::getInviteOnly()
{
	return (_inviteOnly);
}

bool Channel::isEmpty()
{
	return (_members.empty());
}