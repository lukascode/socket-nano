#pragma once

#include "Socket.h"

class TcpServer;

class TcpConnectionHandler 
{
public:
	virtual ~TcpConnectionHandler();

	/// Handles one particular connection
	virtual void HandleConnection()=0;

	/// Sets the socket of tcp connection
	void SetSocket(Socket* socket);

	/// Sets TcpServer object as context for handler
	void SetContext(TcpServer* context);
protected:
	Socket* socket;
	TcpServer* context;
};