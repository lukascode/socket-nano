#ifndef SERVERCONNECTIONHANDLERFACTORY_H
#define SERVERCONNECTIONHANDLERFACTORY_H

#include "NetworkUtils.h"
#include "ServerConnectionHandler.h"

class Server;

class ServerConnectionHandlerFactory {
public:
	virtual ServerConnectionHandler* createServerConnectionHandler()=0;
};

#endif /* SERVERCONNECTIONHANDLERFACTORY_H */