#ifndef SERVERCONNECTIONHANDLER_H
#define SERVERCONNECTIONHANDLER_H

#include "NetworkUtils.h"
#include "Socket.h"

class Server;

class ServerConnectionHandler 
{
public:
	virtual int handleConnection()=0;
	void setSocket(Socket* socket);
	void setContext(Server* context);
protected:
	Socket* socket;
	Server* context;
};

#endif /* SERVERCONNECTIONHANDLER_H */