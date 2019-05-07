#include "Socket.h"
#include "Address.h"
#include "NetworkUtils.h"
#include <cstdlib>
#include <iostream>
#include "TcpServer.h"
#include "TcpConnectionHandler.h"
#include "TcpConnectionHandlerFactory.h"

class TcpConnectionHandlerImpl : public TcpConnectionHandler
{
public:
    virtual void handleConnection()
    {
        try
        {
            std::cout << "New client connected " << socket->getRemoteAddress().getIP() << std::endl;
            socket->enableTimeout(15);
            for (;;)
            {
                auto data = socket->recvuntil("XYZ123\n", 8192);
                for (size_t i = 0; i < data.size(); ++i)
                {
                    printf("%c", data[i]);
                }
            }
        }
        catch (std::exception &e)
        {
            std::cout << std::string(e.what()) << std::endl;
        }
    }
};

class TcpConnectionHandlerImpl2 : public TcpConnectionHandler
{
public:
    virtual void handleConnection()
    {
        try
        {
            std::cout << "New client connected " << socket->getRemoteAddress().getIP() << std::endl;
            std::string data;
            data += "HTTP/1.1 200 OK\r\n";
            data += "Content-Type: text/html\r\n";
            data += "Content-Length: 62\r\n\r\n";
            data += "<h2>Hello world</h2><p>This is simple http server dev test</p>";
            socket->sendall(data);
        }
        catch (std::exception &e)
        {
            std::cout << std::string(e.what()) << std::endl;
        }
    }
};

class TcpConnectionHandlerFactoryImpl : public TcpConnectionHandlerFactory
{
public:
    virtual TcpConnectionHandler *createTcpConnectionHandler()
    {
        return new TcpConnectionHandlerImpl();
    }
};

int main(void)
{

    TcpServer *server = new TcpServer(1234, new TcpConnectionHandlerFactoryImpl());
    server->Listen();

    return 0;
}