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
#include <arpa/inet.h>
#include <memory>
#include "Client.hpp"
#include "CmdList.hpp"
#include "errors.hpp"
#include "macros.hpp"

class Server
{
	private:
		int								_port; //?
		std::string						_pass;
		int								_fd;
		std::string						_serverName;
		std::map<int, std::unique_ptr<Client>> _clients; //put some limits (42)
		std::map<std::string, std::unique_ptr<Channel>> _channels;
		int								_epollFd;
		CmdList							_cmdList;
	public:
		Server(int portNumber, std::string const &password);
		~Server();

		std::string getPass() const;
		const std::string& getServerName() const;
		Client* getClientByNick(const std::string& nick) const;
		Channel* getChannelByName(const std::string& name) const;

		void	createSocket();
		void	start();
		void	setNonBlocking(int fd);
		void	addToEpoll(int fd, uint32_t events);
		void	addEpollOut(int fd);
		void	removeEpollOut(int fd);
		void	removeFromEpoll(int fd);

		//clients
		void	acceptNewClient();
		bool	disconnectClient(int clientFd);
		bool	isNicknameTaken(const std::string& newNick);

		//channels
		Channel* createNewChannel(const std::string& name);
		Channel* getOrCreateChannel(const std::string& name);


		//handling recv and send
		void	handleClientRead(int clientFd);
		void	handleClientWrite(int clientFd);
		void	processMessage(int clientFd, const std::string& message);
		void	removeChannel(const std::string& name);

		//sending messages
		void	sendWelcomeMsg(Client *client);
		void	sendError(Client *client, const std::string& errCode, const std::string& msg);
		void	sendInfo(Client *client, const std::string& msg);
		void	sendToClient(Client *client, const std::string& msg);
};
