#pragma once

#include "Socket.h"
#include "UdpDatagramHandlerFactory.h"
#include<thread>

class UdpServer
{
public:
	UdpServer(UdpDatagramHandlerFactory *datagramHandlerFactory);
	~UdpServer();

	void Listen(short port);
	void Listen(std::string ip, short port);

private:
	Socket *socket;
	short port;
	std::string ip;
	UdpDatagramHandlerFactory *datagramHandlerFactory;
	std::vector<std::thread *> threads;
	
	void _Listen();
};