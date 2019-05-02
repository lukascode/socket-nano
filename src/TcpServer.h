#ifndef TCP_SERVER_H
#define TCP_SERVER_H

#include "Server.h"
#include <thread>

class TcpServer : public Server 
{
public:
	TcpServer(Address address, ServerConnectionHandlerFactory* connHandlerFactory);
protected:
	virtual Socket* createSocket();
	virtual int onListen();
private:
    std::vector<std::thread*> connections;
};

#endif /* TCP_SERVER_H */