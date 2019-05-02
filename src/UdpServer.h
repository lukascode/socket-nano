#ifndef UDPSERVER_H
#define UDPSERVER_H

#include "Server.h"

class UdpServer : public Server {
public:
	UdpServer(Address address, ServerConnectionHandlerFactory* connHandlerFactory);
protected:
	virtual Socket* createSocket();
	virtual int onListen();
};

#endif /* UDPSERVER_H */