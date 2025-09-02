#include "Server.hpp"

Server::Server(int portNumber, std::string const &password): _port(0), _pass(""), _fd(-1), _epollFd(-1)  
{
    //validate port 
    //validate password

    _port = portNumber;
    _pass = password;
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
    
    // Add client to epoll for reading
    addToEpoll(clientFd, EPOLLIN | EPOLLET); // Edge-triggered mode
    
    _clients[clientFd] = std::make_unique<Client>(clientFd);
    std::cout << "New client connected: fd=" << clientFd << std::endl;
}

void Server::disconnectClient(int client_fd)
{
		//std::cout << "Client disconnected: " << _clients[client_fd] << " (fd=" << client_fd << ")" << std::endl;
		
	removeFromEpoll(client_fd);
	close(client_fd);
	_clients.erase(client_fd);
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
            
            if (fd == _fd)
            {
                // New connection
                acceptNewClient();
            }
            else if (events[i].events & EPOLLIN)
            {
                // Data available to read from client
                //handle Client data
            }
            else if (events[i].events & (EPOLLHUP | EPOLLERR))
            {
                // Client disconnected or error
               disconnectClient(fd);
            }
        }
    }
}
	