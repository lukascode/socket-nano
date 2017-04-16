#ifndef CLIENTCONNECTIONHANDLER_H
#define CLIENTCONNECTIONHANDLER_H


#include "Socket.h"

class Client;

class ClientConnectionHandler {
public:
	virtual int handleConnection()=0;
	void setSocket(Socket* socket);
	void setContext(Client* context);
protected:
	Socket* socket;
	Client* context;
};

#endif /* CLIENTCONNECTIONHANDLER_H */