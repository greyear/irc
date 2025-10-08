#include "../includes/Client.hpp"
#include "../includes/Server.hpp"

bool	isPortValid(char *portNumber);
bool	isPassValid(const std::string& pass);

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        std::cerr  << "Usage: " << argv[0] << " <port> <password>" << std::endl;
        return 1;
    }
    if (!isPortValid(argv[1]))
    {
        std::cerr  << "Port is not valid" << std::endl;
        return 1;
    }
    if (!isPassValid(argv[2]))
    {
        std::cerr  << "Password is not valid" << std::endl;
        return 1;
    }
    try
    {
        Server server(std::stoi(argv[1]), argv[2]);
        server.createSocket();
        server.start();
    }
    catch (const std::invalid_argument& e)
    {
        std::cerr << "Invalid argument: " << e.what() << std::endl;
        return 1;
    }
    catch (const std::system_error& e)
    {
        std::cerr << "System error: " << e.what() << std::endl;
        std::cerr << "Error code: " << e.code().value() << std::endl;
        return 1;
    }
    catch (const std::runtime_error& e)
    {
        std::cerr << "Runtime error: " << e.what() << std::endl;
        return 1;
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}
