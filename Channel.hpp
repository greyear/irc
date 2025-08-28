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
		std::string				_owner;
		bool					_inviteOnly;
		bool					_topicRestricted;
		bool					_passRequired;
		std::string				_channelPass;
		bool					_isLimit;
		int						_limit;
	public:
		Channel(std::string name, std::string owner);
		~Channel();


		const std::string&		getName();
};
