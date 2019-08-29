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
	TcpServer(TcpConnectionHandlerFactory *connHandlerFactory);
	~TcpServer();

	void Listen(short port);
	void Listen(std::string ip, short port);
	bool RemoveClient(Socket *client);
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