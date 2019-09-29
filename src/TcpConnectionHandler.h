#pragma once

#include "Socket.h"

class TcpServer;

class TcpConnectionHandler
{
public:
	virtual ~TcpConnectionHandler();

	/// Handles one particular connection
	virtual void HandleConnection() = 0;

	/// Sets the socket of tcp connection
	void SetSocket(std::shared_ptr<Socket> socket);

	/// Sets TcpServer object as context for handler
	void SetServer(std::shared_ptr<TcpServer> server);

protected:
	std::shared_ptr<Socket> socket;
	std::shared_ptr<TcpServer> server;
};