#pragma once

#include<thread>
#include<functional>
#include "Socket.h"
#include "ThreadPool.h"
#include "UdpDatagramHandler.h"

class UdpServer
{
public:

	/// Creates udp server 
	UdpServer(std::function<UdpDatagramHandler*()> datagramHandlerFactory);
	~UdpServer();

	/// Bind to all interfaces on the provided port and listen on incoming datagrams
	void Listen(short port);

	/// Bind to interface provided by ip on the provided port
	void Listen(std::string ip, short port);

	/// Sets number of threads in the pool which are used for handling incoming datagrams
	void setThreadPoolSize(int size);

private:
	static const int defaultThreadPoolSize = 20;
	ThreadPool* tp;
	int tpSize;
	Socket *socket;
	short port;
	std::string ip;
	std::function<UdpDatagramHandler*()> datagramHandlerFactory;
	
	void _Listen();
};