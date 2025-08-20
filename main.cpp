#include <iostream>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

int main() {
    int sockfd = socket(AF_INET, SOCK_STREAM, 0); // create socket

    struct sockaddr_in address;
    address.sin_family = AF_INET;        // IPv4
    address.sin_port = htons(8080);      // port 8080 (htons converts to network byte order)
    address.sin_addr.s_addr = INADDR_ANY; // listen on all network interfaces (can be replaced with 127.0.0.1)

    // bind the socket to the given address
    bind(sockfd, (struct sockaddr*)&address, sizeof(address));

    // start listening for incoming connections
    listen(sockfd, 3);

    std::cout << "Server listening on port 8080..." << std::endl;

    close(sockfd);
    return 0;
}
