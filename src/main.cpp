#include "Socket.h"
#include "Address.h"
#include "NetworkUtils.h"
#include <cstdlib>
#include <iostream>
#include "TcpServer.h"
#include "TcpConnectionHandler.h"
#include "TcpConnectionHandlerFactory.h"
#include "UdpServer.h"
#include "UdpDatagramHandler.h"
#include "UdpDatagramHandlerFactory.h"

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
                std::string datastr(data.begin(), data.end());
                std::cout << datastr;
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

class UdpDatagramHandlerImpl : public UdpDatagramHandler
{
public:
    virtual void handleDatagram()
    {
        try
        {
            std::cout << datagram;
            sleep(10);
        }
        catch (const std::exception &e)
        {
            std::cout<<std::string(e.what())<<std::endl;
        }
    }
};

class UdpDatagramHandlerFactoryImpl : public UdpDatagramHandlerFactory
{
public:
    virtual UdpDatagramHandlerImpl *createUdpDatagramHandler()
    {
        return new UdpDatagramHandlerImpl();
    }
};

int main(void)
{

    // TcpServer *server = new TcpServer(new TcpConnectionHandlerFactoryImpl());
    // server->Listen("0.0.0.0", 1234);

    UdpServer *server = new UdpServer(new UdpDatagramHandlerFactoryImpl());
    server->Listen("0.0.0.0", 1234);
    

    return 0;
}