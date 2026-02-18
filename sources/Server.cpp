#include "../includes/Server.hpp"

volatile sig_atomic_t Server::_sigTermination = 0;
std::string	addCRLF(const std::string& msg);

Server::Server(int portNumber, std::string const &password): _port(portNumber), _pass(password), _fd(-1), _epollFd(-1)
{
	_serverName = "hive.irc.net";
	signal(SIGINT, sigHandler);
	signal(SIGTERM, sigHandler);
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
	for (const auto& client : _clients)
	{
		if (client.second->getFd() != -1)
			close(client.second->getFd());
	}
	_clients.clear();
	_channels.clear();
	signal(SIGINT, SIG_DFL);
	signal(SIGTERM, SIG_DFL);
}

std::string Server::getPass() const
{
	return _pass;
}

const std::string& Server::getServerName() const
{
	return _serverName;
}

Client* Server::getClientByNick(const std::string& nick) const
{
	for (const auto& client : _clients)
	{
		if (client.second->getNick() == nick)
			return (client.second.get());
	}
	return nullptr;
}

Channel* Server::getChannelByName(const std::string& name) const
{
	for (const auto& channel : _channels)
	{
		if (channel.second->getName() == name)
			return (channel.second.get());
	}
	return nullptr;
}

const std::map<std::string, std::unique_ptr<Channel>>& Server::getChannels() const
{
	return _channels;
}

const std::map<int, std::unique_ptr<Client>>& Server::getClients() const
{
	return _clients;
}

void Server::setNonBlocking(int fd)
{
	fcntl(fd, F_SETFL, O_NONBLOCK);
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
		throw std::system_error(errno, std::system_category(), "epoll_ctl_del");
}

void	Server::createSocket()
{
	_epollFd = epoll_create1(EPOLL_CLOEXEC);
	if (_epollFd == -1)
		throw std::system_error(errno, std::system_category(), "epoll_create1");
	
	_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (_fd == -1)
		throw std::system_error(errno, std::system_category(), "socket");

	int opt = 1;
	setsockopt(_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
	setNonBlocking(_fd);
	
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
	if (_clients.size() >= MAX_CLIENTS)
	{
		handleMaxClientsError();
		return ;
	}
	struct sockaddr_in client_addr;
	socklen_t addr_len = sizeof(client_addr);
	
	int clientFd = accept(_fd, (struct sockaddr*)&client_addr, &addr_len);
	if (clientFd == -1)
		throw std::system_error(errno, std::system_category(), "accept");
	
	setNonBlocking(clientFd);
	std::string ip = inet_ntoa(client_addr.sin_addr);

	addToEpoll(clientFd, EPOLLIN | EPOLLET);
	
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

bool	Server::removeClient(Client *client)
{
	if (!client)
		return (false);
	int clientFd = client->getFd();
	std::string clientNick = client->getNick();

	for (const std::string& channelName : client->getChannels())
	{
		auto channelIt = _channels.find(channelName);
		if (channelIt != _channels.end())
		{
			Channel* channel = channelIt->second.get();
			
			channel->removeMember(clientNick);
			channel->removeOperator(clientNick);
			channel->removeInvited(clientNick);
			
			if (channel->isEmpty())
			{
				_channels.erase(channelIt);
			}
		}
	}
	return disconnectClient(clientFd);
}

void	Server::handleMaxClientsError()
{
	struct sockaddr_in client_addr;
	socklen_t addr_len = sizeof(client_addr);
	
	int clientFd = accept(_fd, (struct sockaddr*)&client_addr, &addr_len);
	if (clientFd == -1)
		throw std::system_error(errno, std::system_category(), "accept");

	std::string ip = inet_ntoa(client_addr.sin_addr);
	const std::string errorMsg = "ERROR :Closing Link: " + ip + " (Server full - " + 
								std::to_string(MAX_CLIENTS) + " clients maximum)\r\n";
	
	ssize_t sent = send(clientFd, errorMsg.c_str(), errorMsg.length(), 0);
	if (sent == -1)
	{
		std::cerr << "Failed to send error message to " << ip << std::endl;
	}
	close(clientFd);
}


Channel* Server::createNewChannel(const std::string& name)
{
	_channels[name] = std::make_unique<Channel>(name);
	Channel* channelPtr = _channels[name].get();
	return channelPtr;
}

Channel* Server::getOrCreateChannel(const std::string& name)
{
	Channel* existing = getChannelByName(name);
	if (existing)
		return existing;
	return createNewChannel(name);
}

void Server::removeChannel(const std::string& name)
{
	auto it = _channels.find(name);
	if (it != _channels.end())
		_channels.erase(it);
}

void Server::handleClientRead(int clientFd)
{
	char buffer[BUFFER_SIZE];
	ssize_t bytesRecieved;
	
	std::map<int, std::unique_ptr<Client>>::iterator it = _clients.find(clientFd);
	if (it == _clients.end())
	{
		std::cerr << "Client not found for read: " << clientFd << std::endl;
		return;
	}
	Client* client = it->second.get();
	while ((bytesRecieved = recv(clientFd, buffer, BUFFER_SIZE, MSG_DONTWAIT)) > 0)
	{
		client->appendToReadBuffer(buffer, bytesRecieved);
		while (client->hasCompleteMessage())
		{
			std::string message = client->extractNextMessage();
			if (!message.empty())
				processMessage(clientFd, message);
			if (_clients.find(clientFd) == _clients.end())
				return; 
		}
		
		if (client->isBufferTooLarge())
		{
			std::cerr << "Buffer overflow reached: disconnecting client " << clientFd << std::endl;
			disconnectClient(clientFd);
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
				std::cerr << "Client " << clientFd << " connection timed out" << std::endl;
		}
		else if (errno == ECONNRESET)
		{
			if (disconnectClient(clientFd))
				std::cerr << "Client " << clientFd << " connection reset" << std::endl;
		}
		else
		{
			if (disconnectClient(clientFd))
				std::cerr << "recv() error on client " << clientFd << std::endl;
		}
	}
}

void Server::handleClientWrite(int clientFd)
{
	std::map<int, std::unique_ptr<Client>>::iterator it = _clients.find(clientFd);
	if (it == _clients.end())
	{
		std::cerr << "Client not found for write: " << clientFd << std::endl;
		return;
	}
	Client* client = it->second.get();
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
	
  	std::string cleanMessage;
	for (char c : message)
	{
		if ((c >= 32 && c <= 126) || c == '\r' || c == '\n' || c == '\x01')
		{
			cleanMessage += c;
		}
	}

	std::istringstream iss(cleanMessage);
	std::string cmdName;
	iss >> cmdName;

	std::transform(cmdName.begin(), cmdName.end(), cmdName.begin(), ::toupper);
	if (cmdName == "CAP" || cmdName == "WHO")
		return ;

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

	std::string msg001 = ":" + _serverName + " " + RPL_WELCOME + " " + nick + 
						 " :Welcome to the Internet Relay Network " + 
						 client->getFullIdentifier() + "\r\n";
	sendToClient(client, msg001);
	std::string msg002 = ":" + _serverName + " " + RPL_YOURHOST + " " + nick + 
						" :Your host is " + client->getHostName() + 
						", running version 1.0" + "\r\n";
	sendToClient(client, msg002);
	std::string msg003 = ":" + _serverName + " " + RPL_CREATED + " " + nick + 
						" :This server was created today" + "\r\n";
	sendToClient(client, msg003);
	std::string msg004 = ":" + _serverName + " " + RPL_MYINFO + " " + nick + " " +
						client->getHostName() + " 1.0" + " io ntk\r\n";
	sendToClient(client, msg004);
}

void	Server::sendError(Client *client, const std::string& errCode, const std::string& msg)
{
	std::string error = ":" + _serverName + " " + errCode + " " + client->getNick() + " " + msg + "\r\n";
	sendToClient(client , error);
}

void	Server::sendToClient(Client *client, const std::string& msg)
{
	if (!client)
		return;
	
	std::string fullMsg = addCRLF(msg);
	client->appendToWriteBuffer(fullMsg);

	if (client->hasUnsentData() && !client->isEpollOutActive())
	{
		addEpollOut(client->getFd());
		client->setIsEpollOutActive(true);
	}
}

void Server::start() 
{
	struct epoll_event events[MAX_EVENTS];
	
	while (!_sigTermination)
	{
		int nfds = epoll_wait(_epollFd, events, MAX_EVENTS, -1);
		if (nfds == -1)
			throw std::system_error(errno, std::system_category(), "epoll_wait");
		for (int i = 0; i < nfds; i++)
		{
			int fd = events[i].data.fd;
			if (fd == _fd)
			{
				acceptNewClient();
			}
			else if (events[i].events & EPOLLIN)
			{
				handleClientRead(fd);
			}
			else if (events[i].events & EPOLLOUT)
			{
				handleClientWrite(fd);
			}
			else if (events[i].events & (EPOLLHUP | EPOLLERR))
			{
				disconnectClient(fd);
			}
		}
	}
}
	
void	Server::sigHandler(int sig)
{
	if (sig == SIGINT || sig == SIGTERM)
		_sigTermination = 1;
}
