#pragma once

#include <iostream>
#include <sstream>
#include <algorithm>
#include <map>
#include <vector>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/epoll.h>
#include <fcntl.h>
#include <memory>
#include "Client.hpp"
#include "CmdList.hpp"
#include "errors.hpp"
#include "macro.hpp"

class Server
{
	private:
		int								_port; //?
		std::string						_pass;
		int								_fd;
		std::string						_serverName;
		std::map<int, std::unique_ptr<Client>> _clients;
		std::map<std::string, Channel>	_channels;
		int								_epollFd;
		CmdList							_cmdList;
	public:
		Server(int portNumber, std::string const &password);
		~Server();

		std::string getPass() const;

		void	createSocket();
		void	start();
		void	setNonBlocking(int fd);
		void	addToEpoll(int fd, uint32_t events);
		void	removeFromEpoll(int fd);
		void	acceptNewClient();
		bool	isNicknameTaken(const std::string& newNick);
		void	handleClientData(int clientFd);
		void	sendError(int clientFd, const std::string& errCode, const std::string& msg);
		void	processMessage(int clientFd, const std::string& message);
		void	disconnectClient(int clientFd);
		void	sendWelcomeMsg(Client *client);
		void	sendInfo(Client *client, const std::string& msg);

};
