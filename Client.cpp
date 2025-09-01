#include "Client.hpp"

Client::Client(int fd): _fd(fd), _nick(""), _user(""), _isAuthenticated(false)
{
	std::cout << "_fd: "<< _fd << std::endl;
	std::cout << "_isAuthenticated: " << _isAuthenticated << std::endl;
}

Client::~Client()
{
    
}