#ifndef SERVERCONNECTIONHANDLER_H
#define SERVERCONNECTIONHANDLER_H

#include "NetworkUtils.h"

class Server;

class ServerConnectionHandler {
public:
	virtual int handleConnection(Socket* socket, Server* context)=0;
};

#endif /* SERVERCONNECTIONHANDLER_H */