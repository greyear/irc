#pragma once

#include <iostream>
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
		std::set<Channel>	_channels;
		bool				_isAuthenticated;
	public:
		Client(int fd);
		~Client();
};
