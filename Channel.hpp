#pragma once

#include <iostream>
#include <set>

class Channel
{
	private:
		std::string				_name;
		std::string				_topic;
		std::set<std::string>	_members;
		std::set<std::string>	_operators;
		std::set<std::string>	_invitedUsers;
		bool					_inviteOnly;
		bool					_topicRestricted;
		bool					_passRequired;
		std::string				_channelPass;
		bool					_hasLimit;
		int						_limit;
	public:
		Channel(const std::string& name);
		~Channel();

		const std::string&		getName();
		bool					getInviteOnly();
		bool					isEmpty();

};
