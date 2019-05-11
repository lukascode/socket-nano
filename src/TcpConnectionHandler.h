#pragma once

#include "Socket.h"
#include "TcpServer.h"

class TcpServer;

class TcpConnectionHandler 
{
public:
	virtual ~TcpConnectionHandler();
	virtual void HandleConnection()=0;
	void SetSocket(Socket* socket);
	void SetContext(TcpServer* context);
protected:
	Socket* socket;
	TcpServer* context;
};