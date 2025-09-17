#include "Server.hpp"

std::string	addCRLF(const std::string& msg);

Server::Server(int portNumber, std::string const &password): _port(0), _pass("pass"), _fd(-1), _epollFd(-1)  
{
	//validate port 
	//validate password

	_port = portNumber;
	_pass = password;
	_serverName = "hive.irc.net";
	 std::cout << "_port: " << _port << std::endl;
}

Server::~Server()
{
	if (_fd != -1)
	{
		close(_fd);
		_fd = -1;
	}
	if (_epollFd != -1)
	{
		close(_epollFd);
		_epollFd = -1;
	}
	//Todo: close client fds
}

std::string Server::getPass() const
{
	return _pass;
}

const std::string& Server::getServerName() const
{
	return _serverName;
}

Client* Server::getClientByNick(const std::string& nick)
{
	for (const auto& client : _clients)
	{
		if (client.second->getNick() == nick)
			return (client.second.get());
	}
	return nullptr;
}

void Server::setNonBlocking(int fd)
{
	int flags = fcntl(fd, F_GETFL, 0);
	fcntl(fd, F_SETFL, flags | O_NONBLOCK);
}

void Server::addToEpoll(int fd, uint32_t events)
{
	struct epoll_event ev;
	ev.events = events;
	ev.data.fd = fd;
	
	if (epoll_ctl(_epollFd, EPOLL_CTL_ADD, fd, &ev) == -1)
		throw std::system_error(errno, std::system_category(), "epoll_ctl_add");
}

void Server::addEpollOut(int fd)
{
	struct epoll_event ev;
	ev.events = EPOLLIN | EPOLLOUT;
	ev.data.fd = fd;
	
	epoll_ctl(_epollFd, EPOLL_CTL_MOD, fd, &ev);
	//std::cout << "added epollout" << std::endl;
}

void Server::removeEpollOut(int fd)
{
	struct epoll_event ev;
	ev.events = EPOLLIN;
	ev.data.fd = fd;
	
	epoll_ctl(_epollFd, EPOLL_CTL_MOD, fd, &ev);
	
	Client* client = _clients[fd].get();
	if (client)
		client->setIsEpollOutActive(false);
}

void Server::removeFromEpoll(int fd)
{
	if (epoll_ctl(_epollFd, EPOLL_CTL_DEL, fd, nullptr) == -1)
		throw std::system_error(errno, std::system_category(), "epoll_ctl_add");
}

void	Server::createSocket()
{
	_epollFd = epoll_create1(EPOLL_CLOEXEC);
	if (_epollFd == -1)
		throw std::system_error(errno, std::system_category(), "epoll_create1");
	
	_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (_fd == -1)
		throw std::system_error(errno, std::system_category(), "socket");

// Set socket options
	int opt = 1;
	setsockopt(_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
	setNonBlocking(_fd);
	
	// Bind and listen
	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = INADDR_ANY;
	addr.sin_port = htons(_port);

	if (bind(_fd, (struct sockaddr*)&addr, sizeof(addr)) == -1)
		throw std::system_error(errno, std::system_category(), "bind");
		
	if (listen(_fd, MAX_QUEUE) == -1)
	   throw std::system_error(errno, std::system_category(), "listen");

	addToEpoll(_fd, EPOLLIN);
	
	std::cout << "IRC Server listening on port " << _port << std::endl;

}

void Server::acceptNewClient()
{
	struct sockaddr_in client_addr;
	socklen_t addr_len = sizeof(client_addr);
	
	int clientFd = accept(_fd, (struct sockaddr*)&client_addr, &addr_len);
	if (clientFd == -1)
		throw std::system_error(errno, std::system_category(), "accept");
	
	setNonBlocking(clientFd);
	std::string ip = inet_ntoa(client_addr.sin_addr);

	// Add client to epoll for reading
	addToEpoll(clientFd, EPOLLIN | EPOLLET); // Edge-triggered mode
	
	_clients[clientFd] = std::make_unique<Client>(clientFd);
	_clients[clientFd].get()->setHostName(ip);
	std::cout << "New client connected: fd=" << clientFd << std::endl;
}

bool	Server::isNicknameTaken(const std::string& newNick)
{
	for (const auto& client : _clients)
	{
		if (client.second->getNick() == newNick)
			return (true);
	}
	return (false);
}

void Server::handleClientRead(int clientFd)
{
	char buffer[BUFFER_SIZE];
	ssize_t bytesRecieved;
	
	std::map<int, std::unique_ptr<Client>>::iterator it = _clients.find(clientFd);
	if (it == _clients.end())
	{
		std::cerr << "Client not found for read: " << clientFd << std::endl;
		return; //TODO: check what if client doesn't exist yet or gets deleted before calling this
	}
	Client* client = it->second.get();
	while ((bytesRecieved = recv(clientFd, buffer, BUFFER_SIZE, MSG_DONTWAIT)) > 0)
	{
		client->appendToReadBuffer(buffer, bytesRecieved);
		while (client->hasCompleteMessage())
		{
			std::string message = client->extractNextMessage();
			//std::cout << message <<  " :  The message" << std::endl;
			if (!message.empty())
				processMessage(clientFd, message);
		}
		
		if (client->isBufferTooLarge())
		{
			std::cerr << "Buffer overflow reached: disconnecting client " << clientFd << std::endl; //TODO: change to nick later when we have getters
			disconnectClient(clientFd); //TODO: check if nothing was allocated
			return;
		}
	}
	if (bytesRecieved == 0)
	{
		if (disconnectClient(clientFd))
			std::cout << "Client " << clientFd << " closed connection" << std::endl;
	}
	else if (bytesRecieved == -1)
	{
		if (errno == EAGAIN || errno == EWOULDBLOCK)
			return;
		else if (errno == ETIMEDOUT)
        {
			if (disconnectClient(clientFd))
				std::cout << "Client " << clientFd << " connection timed out" << std::endl;
        }
		else if (errno == ECONNRESET)
		{
			if (disconnectClient(clientFd))
				std::cout << "Client " << clientFd << " connection reset" << std::endl;
		}
		else
		{
			if (disconnectClient(clientFd))
				std::cerr << "recv() error on client " << clientFd << std::endl;
		}
	}
}

//TODO: finish later...
void Server::handleClientWrite(int clientFd)
{
	std::map<int, std::unique_ptr<Client>>::iterator it = _clients.find(clientFd);
	if (it == _clients.end())
	{
		std::cerr << "Client not found for write: " << clientFd << std::endl;
		return;
	}
	Client* client = it->second.get();
	//std::cout << "before flushing" << std::endl;
	int flushRes = client->flushWriteBuffer();
	if (flushRes == FLUSH_SUCCESS)
	{
		removeEpollOut(clientFd);
		return ;
	}
	else if (flushRes == FLUSH_PARTIAL || flushRes == FLUSH_LATER)
	{
		return ;
	}
	else if (flushRes == FLUSH_ERROR)
	{
		if (disconnectClient(clientFd))
			std::cerr << "send() error on client " << clientFd << std::endl;
	}
}

void	Server::processMessage(int clientFd, const std::string& message)
{
	Client *client = _clients[clientFd].get();
	if (!client)
		return ;
	
	std::istringstream iss(message);
	std::string cmdName;
	iss >> cmdName;
	std::transform(cmdName.begin(), cmdName.end(), cmdName.begin(), ::toupper);
	//std::cout << "cmdNAME in processMsg : "<< cmdName << std::endl;

	std::vector<std::string> params;
	std::string param;
	while (iss >> param && param[0] != ':')
		params.push_back(param);
	
	std::string multiWordParam;
	if (!param.empty() && param[0] == ':')
	{
		multiWordParam = param.substr(1);
		std::string additionalWords;
		std::getline(iss, additionalWords);
		if (!additionalWords.empty())
		{
			multiWordParam += additionalWords;
		}
	}

	ACommand* command = _cmdList.getCommand(cmdName);
	if (!command)
	{
		sendError(client, ERR_UNKNOWNCOMMAND, cmdName + " :Unknown command");
		return ;
	}
	
	if (command->needsRegistration() && !client->checkRegistrationComplete())
	{
		sendError(client, ERR_NOTREGISTERED, ":You have not registered");
		return;
	}
	//std::cout << "before calling exec "<< cmdName << std::endl;
	command->execute(this, client, params, multiWordParam);
}

bool Server::disconnectClient(int clientFd)
{
	if (_clients.find(clientFd) != _clients.end())
	{
		removeFromEpoll(clientFd);
		close(clientFd);
		_clients.erase(clientFd);
		return(true);
	}
	return(false);
}

void	Server::sendWelcomeMsg(Client *client)
{
	std::string nick = client->getNick();
	std::string user = client->getUser();

	std::string msg001 = ":" + _serverName + " 001 " + nick + 
						 " :Welcome to the Internet Relay Network " + 
						 nick + "!" + user  + "@" + _serverName + "\r\n";
	sendToClient(client, msg001);
	//send(fd, msg001.c_str(), msg001.length(), 0);
	std::string msg002 = ":" + _serverName + " 002 " + nick + 
						" :Your host is " + _serverName + 
						", running version 1.0" + "\r\n";
	sendToClient(client, msg002);
	//send(fd, msg002.c_str(), msg002.length(), 0);
	std::string msg003 = ":" + _serverName + " 003 " + nick + 
						" :This server was created today" + "\r\n";
	sendToClient(client, msg003);
	//send(fd, msg003.c_str(), msg003.length(), 0);
	std::string msg004 = ":" + _serverName + " 004 " + nick + " " + 
						_serverName + " 1.0" + " io ntk\r\n";
	sendToClient(client, msg004);
	//send(fd, msg004.c_str(), msg004.length(), 0);
}

void	Server::sendError(Client *client, const std::string& errCode, const std::string& msg)
{
	std::string error = ":server " + errCode + " " + msg + "\r\n";
	sendToClient(client , error);
}

void	Server::sendInfo(Client *client, const std::string& msg)
{
	sendToClient(client, msg);
}

void	Server::sendToClient(Client *client, const std::string& msg)
{
	if (!client)
		return;
	
	std::string fullMsg = addCRLF(msg);
	client->appendToWriteBuffer(fullMsg); //change to full

	/*  TODO: do we need to check if buffer empty?
		bool hasOutputData() const {
        return !_outputBuffer.empty();
    }*/

	if (client->hasUnsentData() && !client->isEpollOutActive())
	{
        addEpollOut(client->getFd());
		client->setIsEpollOutActive(true);
    }
}

void Server::start() 
{
	struct epoll_event events[MAX_EVENTS];
	
	while (true)
	{
		// Wait for events (-1 means wait indefinitely)
		int nfds = epoll_wait(_epollFd, events, MAX_EVENTS, -1);
		
		if (nfds == -1)
			throw std::system_error(errno, std::system_category(), "epoll_wait");
		//Todo: signals 
		// Process all ready file descriptors
		for (int i = 0; i < nfds; i++)
		{
			int fd = events[i].data.fd;
			//std::cout << " Ready fd here in the loop" << fd <<  std::endl;
			if (fd == _fd)
			{
				//std::cout << " trying to accept again ??" << std::endl;
				acceptNewClient();
			}
			else if (events[i].events & EPOLLIN)
			{
				//std::cout << " got an EPOLLIN flag in the loop ??" << std::endl;
				// Data available to read from client
				handleClientRead(fd);
			}
			else if (events[i].events & EPOLLOUT)
			{
				handleClientWrite(fd);
			}
			else if (events[i].events & (EPOLLHUP | EPOLLERR))
			{
				// Client disconnected or error
			   disconnectClient(fd);
			}
		}
	}
}
	

//USER eetu 0 * : anya eetu something 
//realname: " anya eetu something"

//USER eetu 0 * :anya eetu something 
//realname: "anya eetu something"