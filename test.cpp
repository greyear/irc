#include <sys/epoll.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <fcntl.h>
#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <cstring>

class IRCServer {
private:
    int epoll_fd;
    int server_fd;
    std::map<int, std::string> clients; // fd -> nickname mapping
    
    void setNonBlocking(int fd) {
        int flags = fcntl(fd, F_GETFL, 0);
        fcntl(fd, F_SETFL, flags | O_NONBLOCK);
    }
    
    void addToEpoll(int fd, uint32_t events) {
        struct epoll_event ev;
        ev.events = events;
        ev.data.fd = fd;
        
        if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, fd, &ev) == -1) {
            perror("epoll_ctl: add");
        }
    }
    
    void removeFromEpoll(int fd) {
        if (epoll_ctl(epoll_fd, EPOLL_CTL_DEL, fd, nullptr) == -1) {
            perror("epoll_ctl: del");
        }
    }
    
public:
    IRCServer(int port) {
        // Create epoll instance
        epoll_fd = epoll_create1(0);
        if (epoll_fd == -1) {
            perror("epoll_create1");
            exit(1);
        }
        
        // Create server socket
        server_fd = socket(AF_INET, SOCK_STREAM, 0);
        if (server_fd == -1) {
            perror("socket");
            exit(1);
        }
        
        // Set socket options
        int opt = 1;
        setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
        setNonBlocking(server_fd);
        
        // Bind and listen
        struct sockaddr_in addr;
        addr.sin_family = AF_INET;
        addr.sin_addr.s_addr = INADDR_ANY;
        addr.sin_port = htons(port);
        
        if (bind(server_fd, (struct sockaddr*)&addr, sizeof(addr)) == -1) {
            perror("bind");
            exit(1);
        }
        
        if (listen(server_fd, 128) == -1) {
            perror("listen");
            exit(1);
        }
        
        // Add server socket to epoll for accepting new connections
        addToEpoll(server_fd, EPOLLIN);
        
        std::cout << "IRC Server listening on port " << port << std::endl;
    }
    
    void acceptNewClient() {
        struct sockaddr_in client_addr;
        socklen_t addr_len = sizeof(client_addr);
        
        int client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &addr_len);
        if (client_fd == -1) {
            if (errno != EAGAIN && errno != EWOULDBLOCK) {
                perror("accept");
            }
            return;
        }
        
        setNonBlocking(client_fd);
        
        // Add client to epoll for reading
        addToEpoll(client_fd, EPOLLIN | EPOLLET); // Edge-triggered mode
        
        clients[client_fd] = "Unknown"; // Default nickname
        std::cout << "New client connected: fd=" << client_fd << std::endl;
    }
    
    void handleClientData(int client_fd) {
        char buffer[1024];
        ssize_t bytes_read;
        
        // Read all available data (edge-triggered mode requires this)
        while ((bytes_read = read(client_fd, buffer, sizeof(buffer) - 1)) > 0) {
            buffer[bytes_read] = '\0';
            
            // Simple IRC command parsing example
            std::string message(buffer);
            if (message.substr(0, 5) == "NICK ") {
                std::string nickname = message.substr(5);
                // Remove newline characters
                nickname.erase(nickname.find_last_not_of("\r\n") + 1);
                clients[client_fd] = nickname;
                
                std::string response = ":server 001 " + nickname + " :Welcome\r\n";
                send(client_fd, response.c_str(), response.length(), 0);
            }
            else if (message.substr(0, 5) == "PING ") {
                std::string pong = "PONG " + message.substr(5);
                send(client_fd, pong.c_str(), pong.length(), 0);
            }
            else if (message.substr(0, 8) == "PRIVMSG ") {
                // Broadcast message to all clients (simplified)
                broadcastMessage(client_fd, message);
            }
        }
        
        if (bytes_read == 0 || (bytes_read == -1 && errno != EAGAIN && errno != EWOULDBLOCK)) {
            // Client disconnected or error
            disconnectClient(client_fd);
        }
    }
    
    void broadcastMessage(int sender_fd, const std::string& message) {
        std::string formatted_msg = ":" + clients[sender_fd] + " " + message;
        
		for (std::map<int, std::string>::iterator it = clients.begin(); it != clients.end(); ++it) {
    		send(it->first, message.c_str(), message.length(), 0);
		}
    }
    
    void disconnectClient(int client_fd) {
        std::cout << "Client disconnected: " << clients[client_fd] << " (fd=" << client_fd << ")" << std::endl;
        
        removeFromEpoll(client_fd);
        close(client_fd);
        clients.erase(client_fd);
    }
    
    void run() {
        const int MAX_EVENTS = 64;
        struct epoll_event events[MAX_EVENTS];
        
        while (true) {
            // Wait for events (-1 means wait indefinitely)
            int nfds = epoll_wait(epoll_fd, events, MAX_EVENTS, -1);
            
            if (nfds == -1) {
                if (errno == EINTR) continue; // Interrupted by signal
                perror("epoll_wait");
                break;
            }
            
            // Process all ready file descriptors
            for (int i = 0; i < nfds; i++) {
                int fd = events[i].data.fd;
                
                if (fd == server_fd) {
                    // New connection
                    acceptNewClient();
                } else if (events[i].events & EPOLLIN) {
                    // Data available to read from client
                    handleClientData(fd);
                } else if (events[i].events & (EPOLLHUP | EPOLLERR)) {
                    // Client disconnected or error
                    disconnectClient(fd);
                }
            }
        }
    }
    
    ~IRCServer() {
        close(server_fd);
        close(epoll_fd);
    }
};

int main() {
    IRCServer server(6667); // Standard IRC port
    server.run();
    return 0;
}
