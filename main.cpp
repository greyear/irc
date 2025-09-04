#include "Client.hpp"
#include "Server.hpp"

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        std::cerr  << "Usage: " << argv[0] << " <port> <password>" << std::endl;
        return 1;
    }
    try
    {
        //Todo: validate password and port number
        Server server(std::stoi(argv[1]), argv[2]);
        server.createSocket();
        server.start();
    }
    catch (const std::invalid_argument& e)
    {
        // Most specific - parameter validation errors
        std::cerr << "Invalid argument: " << e.what() << std::endl;
        return 1;
    }
    catch (const std::system_error& e)
    {
        // Specific - system call errors  
        std::cerr << "System error: " << e.what() << std::endl;
        std::cerr << "Error code: " << e.code().value() << std::endl;
        return 1;
    }
    catch (const std::runtime_error& e)
    {
        // More general - runtime errors
        std::cerr << "Runtime error: " << e.what() << std::endl;
        return 1;
    }
    catch (const std::exception& e)
    {
        // Most general - catches everything else
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}
