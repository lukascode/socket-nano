#ifndef SERVER_CONNECTION_HANDLER_H
#define SERVER_CONNECTION_HANDLER_H

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

#endif /* SERVER_CONNECTION_HANDLER_H */