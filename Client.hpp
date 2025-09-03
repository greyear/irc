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
		std::string			_messageBuffer;
		std::set<Channel>	_channels;
		bool				_hasPass;
		bool				_hasUser;
		bool				_hasNick;
		bool				_registered;
	public:
		Client(int fd);
		~Client();

		bool	getHasPass() const;
		bool	getHasUser() const;
		bool	getHasNick() const;
		bool	isRegistered() const;

		void	setHasPass(bool received);
		void	setHasUser(bool received);
		void	setHasNick(bool received);
		void	setRegistered();

		void appendToBuffer(const char* data, size_t length);
		bool hasCompleteMessage() const;
		std::string extractNextMessage();
		bool isBufferTooLarge() const;
		void clearBuffer();
};
