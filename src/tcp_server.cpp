#include <charconv>
#include <string>
#include <format>

#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#include "tcp_server.hpp"
#include "logger.hpp"

using std::string;
using std::format;

///////////////////////////////////////////////////////////////////////////////
// constructors
///////////////////////////////////////////////////////////////////////////////
TcpServer::TcpServer() : log(false, false) {
    address = "127.0.0.1";
    port = 3000;
    queue_size = 10;
    server_socket = -1;
}

TcpServer::TcpServer(string address) : log(false, false) {
    size_t pos = address.find_first_of(":");
    string address_str = address.substr(0, pos);
    string port_str = address.substr(pos + 1);

    if (address_str == "localhost") {
        address_str = "127.0.0.1";
    }

    this->address = address_str;
    queue_size = 10;
    server_socket = -1;

    int port_value;
    auto [ptr, ec] = std::from_chars(
        port_str.data(),
        port_str.data() + port_str.size(),
        port_value
    );

    if (ec == std::errc()) {
        port = port_value;
    } else {
        port = 3000;
    }
}

TcpServer::TcpServer(string address, int port) : log(false, false) {
    this->address = address;
    this->port = port;
    queue_size = 10;
    server_socket = -1;
}

TcpServer::TcpServer(
    string address,
    int port,
    int queue_size,
    bool log_to_console,
    bool log_to_file)
    : log(log_to_console, log_to_file) {
    this->address = address;
    this->port = port;
    this->queue_size = queue_size;
    server_socket = -1;
}

///////////////////////////////////////////////////////////////////////////////
// destructor
///////////////////////////////////////////////////////////////////////////////
TcpServer::~TcpServer() {
    closeSockets();
}

///////////////////////////////////////////////////////////////////////////////
// public member functions
///////////////////////////////////////////////////////////////////////////////
void TcpServer::acceptClient() {
    struct sockaddr_in addr;
    int enable = 1;
    socklen_t addrlen = sizeof(addr);

    int res1 = server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (res1 < 0) {
        log.error("Socket creation error");
        return;
    }

    log.info(format("Server socket created: {}", server_socket));

    int res2 = setsockopt(
        server_socket,
        SOL_SOCKET,
        SO_REUSEADDR,
        (const char *)&enable,
        sizeof(enable)
    );

    if (res2 < 0) {
        log.error("setsockopt failed");
        closeSocket(server_socket);
        return;
    }

    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(port);

    int res3 = bind(server_socket, (struct sockaddr *)&addr, sizeof(addr));
    if (res3 < 0) {
        log.error("Bind failed. Check if port is already in use");
        closeSocket(server_socket);
        return;
    }

    int res4 = listen(server_socket, queue_size);
    if (res4 < 0) {
        log.error("Listen failed");
        closeSocket(server_socket);
        return;
    }

    log.info(format("Server listening on {}:{}...", address, port));

    while (true) {
        int client_socket = accept(
            server_socket,
            (struct sockaddr *)&addr,
            &addrlen
        );

        if (client_socket < 0) {
            log.error("Accept failed");
            continue;
        }

        log.info(format("Client socket created: {}", client_socket));

        client_sockets.push_back(client_socket);

        char client_ip[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &(addr.sin_addr), client_ip, INET_ADDRSTRLEN);
        log.info(format(
            "Connection accepted from {}:{}",
            client_ip,
            ntohs(addr.sin_port)
        ));
    }
}

///////////////////////////////////////////////////////////////////////////////
// private member functions
///////////////////////////////////////////////////////////////////////////////
void TcpServer::closeSocket(int socket) {
    if (socket != -1) {
        close(socket);
        log.info(format("Socket closed: {}", socket));
    }
}

void TcpServer::closeSockets() {
    if (server_socket != -1) {
        close(server_socket);
        log.info(format("Server socket closed: {}", server_socket));
    }

    for (auto socket : client_sockets) {
        if (socket != -1) {
            close(socket);
            log.info(format("Client socket closed: {}", socket));
        }
    }
}
