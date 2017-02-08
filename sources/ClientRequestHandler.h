#ifndef CLIENTREQUESTHANDLER_H
#define CLIENTREQUESTHANDLER_H


#include "Socket.h"

class Client;

class ClientRequestHandler {
public:
	virtual int handleConnection()=0;
	void setSocket(Socket* socket);
	void setContext(Client* context);
protected:
	Socket* socket;
	Client* context;
};

#endif /* CLIENTREQUESTHANDLER_H */