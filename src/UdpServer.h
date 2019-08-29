#pragma once

#include<thread>
#include<functional>
#include "Socket.h"
#include "UdpDatagramHandlerFactory.h"
#include "ThreadPool.h"

class UdpServer
{
public:
	UdpServer(UdpDatagramHandlerFactory *datagramHandlerFactory);
	~UdpServer();

	void Listen(short port);
	void Listen(std::string ip, short port);
	void setThreadPoolSize(int size);

private:
	static const int defaultThreadPoolSize = 20;
	ThreadPool* tp;
	int tpSize;
	Socket *socket;
	short port;
	std::string ip;
	UdpDatagramHandlerFactory *datagramHandlerFactory;
	
	void _Listen();
};