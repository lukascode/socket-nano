#ifndef SERVER_CONNECTION_HANDLER_FACTORY_H
#define SERVER_CONNECTION_HANDLER_FACTORY_H

#include "NetworkUtils.h"
#include "ServerConnectionHandler.h"

class Server;

class ServerConnectionHandlerFactory 
{
public:
	virtual ServerConnectionHandler* createServerConnectionHandler()=0;
};

#endif /* SERVER_CONNECTION_HANDLER_FACTORY_H */