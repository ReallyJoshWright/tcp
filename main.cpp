#include "tcp_server_async.hpp"
#include "tcp_client_async.hpp"
#include "tcp_server.hpp"
#include "tcp_client.hpp"
#include "logger.hpp"

int main() {
    Logger log(true, true);
    log.info("Hello");
}
