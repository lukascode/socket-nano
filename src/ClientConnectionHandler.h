#ifndef CLIENT_CONNECTION_HANDLER_H
#define CLIENT_CONNECTION_HANDLER_H


#include "Socket.h"

class Client;

class ClientConnectionHandler 
{
public:
	virtual int handleConnection()=0;
	void setSocket(Socket* socket);
	void setContext(Client* context);
protected:
	Socket* socket;
	Client* context;
};

#endif /* CLIENT_CONNECTION_HANDLER_H */