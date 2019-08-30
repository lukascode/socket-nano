#pragma once

#include <thread>
#include <algorithm>
#include "Address.h"
#include "TcpConnectionHandlerFactory.h"
#include "TcpConnectionHandler.h"
#include "ThreadPool.h"
#include <functional>

class TcpServer
{
public:
	
	/// Creates tcp server 
	TcpServer(TcpConnectionHandlerFactory *connHandlerFactory);

	~TcpServer();

	/// Bind to all interfaces on the provided port and listen on incoming connections
	void Listen(short port);

	/// Bind to interface provided by ip on the provided port
	void Listen(std::string ip, short port);

	/// Removes client socket
	bool RemoveClient(Socket *client);

	/// Sets number of threads in the pool which are used for handling incoming connections
	void setThreadPoolSize(int size);

private:
	static const int defaultThreadPoolSize = 20;
	ThreadPool* tp;
	int tpSize;
	std::vector<Socket *> clients;
	TcpConnectionHandlerFactory *connHandlerFactory;
	Socket *socket;
	short port;
	std::string ip;

	void _Listen();
};