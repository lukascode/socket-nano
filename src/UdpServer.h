#pragma once

#include "Server.h"

class UdpServer : public Server
{
public:
	UdpServer(Address address, ServerConnectionHandlerFactory *connHandlerFactory);

protected:
	virtual Socket *createSocket();
	virtual int onListen();
};