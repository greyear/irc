#pragma once

#include <iostream>
#include <set>

class Channel
{
	private:
		std::string				_name;
		std::string				_topic;
		std::set<std::string>	_members;
		std::set<std::string>	_operators; // MODE -o
		std::set<std::string>	_invitedUsers; // TODO : when quit cmd is called ? 
		bool					_inviteOnly; // MODE -i
		//bool					_topicRestricted; // MODE -t
		bool					_keyRequired; // MODE -k
		std::string				_channelKey;
		bool					_hasLimit; // MODE -l
		uint					_limit;
	public:
		Channel(const std::string& name);
		~Channel();

		const std::string&				getName() const;
		const std::string&				getTopic() const;
		bool							getInviteOnly() const;
		bool							getHasLimit() const;
		uint							getLimit() const;
		const std::set<std::string>&	getMembers() const;
		bool							getKeyRequired() const;
		const std::string&				getChannelKey() const;
		bool							isEmpty();
		bool							isOperator(const std::string &nick);
		bool							isMember(const std::string &nick);
		bool							isInvited(const std::string &nick);
		void							addMember(const std::string &nick);
		void							addOperator(const std::string &nick);
		void							addInvited(const std::string &nick);
		void							removeMember(const std::string &nick);
		void							removeOperator(const std::string &nick);
		void							removeInvited(const std::string &nick);
};
