#include "Client.hpp"

Client::Client(int fd): _fd(fd), _nick(""), _user(""), _isAuthenticated(false)
{
}
Client::~Client()
{
    
}