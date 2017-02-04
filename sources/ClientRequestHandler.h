#ifndef CLIENTREQUESTHANDLER_H
#define CLIENTREQUESTHANDLER_H

#include "Client.h"

class Client;

class ClientRequestHandler {
public:
	virtual int handleConnection(Socket* socket, Client* context)=0;
};

#endif /* CLIENTREQUESTHANDLER_H */