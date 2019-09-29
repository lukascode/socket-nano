# C++ network library
[![Build Status](https://travis-ci.org/lukascode/socket-nano.svg?branch=master)](https://travis-ci.org/lukascode/socket-nano)

The library provides a simple programming interface for operations on network sockets.
For now, only Linux is supported, but windows support is planned in the future.

### Simple tcp server example:

```cpp
#include "include/socknano.h"
#include <iostream>

class HttpRequestHandler : public TcpConnectionHandler
{
public:
    virtual void HandleConnection()
    {
        std::cout << "New client connected: " << socket->GetRemoteAddress().ToString() << std::endl;
        
        std::string data = 
            "HTTP/1.1 200 OK\r\n"
            "Content-Type: text/html\r\n"
            "Content-Length: 18\r\n\r\n"
            "<p>Hello World</p>";

        socket->SendAll(data);
    }
};

int main()
{
    auto server = TcpServer::Create([] { return new HttpRequestHandler(); });

    server->Listen(8080);

    return 0;
}.
```

There is also an example of chat implementation using this library 
[here](https://github.com/lukascode/socket-nano/tree/master/src/examples/chat).

