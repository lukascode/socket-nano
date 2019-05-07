#pragma once

#include "Socket.h"
#include "TcpServer.h"

class TcpServer;

class TcpConnectionHandler 
{
public:
	virtual ~TcpConnectionHandler();
	virtual void handleConnection()=0;
	void setSocket(Socket* socket);
	void setContext(TcpServer* context);
protected:
	Socket* socket;
	TcpServer* context;
};