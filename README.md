# TCP

This is a tcp library in c++ for client and server, synchronous and
asynchronous.

## Examples

### TcpServer
```c++

#include "tcp_server.hpp"

int main() {
    TcpServer tcp_server("127.0.0.1", 3000, 10, true, true);
    tcp_server.acceptClient();

    return 0;
}

```

## Todo
- add windows support and build
