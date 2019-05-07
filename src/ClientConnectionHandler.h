#pragma once


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