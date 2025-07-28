#pragma once

#include <string>
#include <vector>

#include "logger.hpp"

class TcpServer {
public:
    TcpServer();
    TcpServer(std::string address);
    TcpServer(std::string address, int port);
    TcpServer(
        std::string address,
        int port,
        int queue_size,
        bool log_to_console,
        bool log_to_file
    );
    ~TcpServer();
    void acceptClient();

private:
    std::string address;
    int port;
    int queue_size;
    Logger log;
    int server_socket;
    std::vector<int> client_sockets;

private:
    void closeSocket(int socket);
    void closeSockets();
};
