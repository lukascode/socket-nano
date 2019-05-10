#pragma once

#include "Socket.h"

class UdpServer
{
public:
	UdpServer(); // todo add handler
	~UdpServer();

	void Listen(short port);
	void Listen(std::string ip, short port);

protected:
	Socket *socket;
	short port;
	std::string ip;

	void _Listen();
};