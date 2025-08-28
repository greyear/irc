#pragma once

#include <iostream>
#include <map>
#include "Client.hpp"


class Server
{
	private:
		int								_port; //?
		std::string						_pass;
		int								_fd;
		std::map<int, Client>			_clients; //pointers?
		std::map<std::string, Channel>	_channels;
		int								_epollFd;
	public:
		Server(int portNumber);
		~Server();

};
