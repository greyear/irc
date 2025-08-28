#pragma once

#include <iostream>
#include <set>
#include "Channel.hpp"

class Client
{
	private:
		int	_fd;
		std::string			_nick;
		std::string			_user;
		std::set<Channel>	_channels;
		bool				_isAuthenticated;
	public:
		Client();
		~Client();
};
