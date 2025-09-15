#pragma once

#include <iostream>
#include <string>
#include <set>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include "Channel.hpp"
#include "macro.hpp"

class Client
{
	private:
		int					_fd;
		std::string			_nick;
		std::string			_user;
		std::string			_realName;
		std::string			_readBuffer;
		std::string			_writeBuffer;
		bool				_hasUnsentData;
		std::set<Channel>	_channels;
		bool				_hasPass;
		bool				_hasUser;
		bool				_hasNick;
		bool				_registered; //TODO: check if we need that
	public:
		Client(int fd);
		~Client();

		int		getFd() const;
		const std::string&	getNick() const;
		const std::string&	getUser() const;
		const std::string&	getRealName() const;
		bool	getHasPass() const;
		bool	getHasUser() const;
		bool	getHasNick() const;
		bool	isRegistered() const;
		bool	hasUnsentData() const;
		void	setNick(const std::string& nick);
		void	setUser(const std::string& user);
		void	setRealName(const std::string& realName);
		void	setHasPass(bool received);
		void	setHasUser(bool received);
		void	setHasNick(bool received);
		void	setRegistered();
		bool	checkRegistrationComplete();

		void appendToReadBuffer(const char* data, size_t length);
		void appendToWriteBuffer(const std::string& msg);
		bool hasCompleteMessage() const;
		std::string extractNextMessage();
		bool isBufferTooLarge() const;
		void clearBuffer();
		int	flushWriteBuffer();
};
