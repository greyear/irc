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

		const std::string&				getName() const;
		const std::string&				getTopic() const;
		bool							getInviteOnly() const;
		bool							getHasLimit() const;
		int								getLimit() const;
		const std::set<std::string>&	getMembers() const;
		bool							getPassRequired() const;
		const std::string&				getChannelPass() const;
		bool							isEmpty();
		bool							isInvited(const std::string &nick);
		void							addMember(const std::string &nick);
		void							addOperator(const std::string &nick);
};
