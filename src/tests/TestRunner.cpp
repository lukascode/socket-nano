#include <iostream>
#include "TestCase1.h"
#include "../socknano.h"

class TcpConnectionHandlerImpl : public TcpConnectionHandler
{
public:
	virtual void HandleConnection()
	{
		try
		{
			std::cout << "New client connected " << socket->GetRemoteAddress().GetIP() << std::endl;
			std::string data;
			data += "HTTP/1.1 200 OK\r\n";
			data += "Content-Type: text/html\r\n";
			data += "Content-Length: 62\r\n\r\n";
			data += "<h2>Hello world</h2><p>This is simple http server dev test</p>";
			socket->SendAll(data);
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
	virtual TcpConnectionHandler *CreateTcpConnectionHandler()
	{
		return new TcpConnectionHandlerImpl();
	}
};

class TcpConnectionHandlerImpl2 : public TcpConnectionHandler
{
public:
    virtual void HandleConnection()
    {
        try
        {
            std::cout << "New client connected " << socket->GetRemoteAddress().GetIP() << std::endl;
            // socket->EnableTimeout(15);
            for (;;)
            {
                auto data = socket->RecvUntil("XYZ123\n", 8192);
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

class UdpDatagramHandlerImpl : public UdpDatagramHandler
{
public:
    virtual void HandleDatagram()
    {
        try
        {
            std::cout << datagram;
            sleep(10);
        }
        catch (const std::exception &e)
        {
            std::cout << std::string(e.what()) << std::endl;
        }
    }
};

class UdpDatagramHandlerFactoryImpl : public UdpDatagramHandlerFactory
{
public:
    virtual UdpDatagramHandlerImpl *CreateUdpDatagramHandler()
    {
        return new UdpDatagramHandlerImpl();
    }
};

int
main()
{
	TcpServer server(new TcpConnectionHandlerFactoryImpl());

	server.Listen(8080);

	return 0;
}
