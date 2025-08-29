#pragma once

#include <iostream>
#include <map>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/epoll.h>
#include <fcntl.h>
#include <memory>
#include "Client.hpp"
#include "macro.hpp"

class Server
{
	private:
		int								_port; //?
		std::string						_pass;
		int								_fd;
		std::map<int, std::unique_ptr<Client>> _clients; 
		std::map<std::string, Channel>	_channels;
		int								_epollFd;
	public:
		Server(int portNumber, std::string const &password);
		void	createSocket();
		void setNonBlocking(int fd);
		void addToEpoll(int fd, uint32_t events);
		void acceptNewClient();
		void start();
		~Server();

};
