#include "tcp_server_async.hpp"
#include "tcp_client_async.hpp"
#include "tcp_server.hpp"
#include "tcp_client.hpp"

int main() {
    TcpServer tcp_server("127.0.0.1", 3000, 10, true, true);
    tcp_server.acceptClient();
}
