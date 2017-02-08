#ifndef TCPSERVER_H
#define TCPSERVER_H

#include "Server.h"

class TcpServer : public Server {
public:
	TcpServer(Address address, ServerConnectionHandlerFactory* connHandlerFactory);
protected:
	virtual Socket* createSocket();
	virtual int onListen();
};

#endif /* TCPSERVER_H */