#pragma once

#include <thread>
#include <algorithm>
#include "Address.h"
#include "TcpConnectionHandlerFactory.h"
#include "TcpConnectionHandler.h"

class TcpServer
{
public:
	TcpServer(TcpConnectionHandlerFactory *connHandlerFactory);
	~TcpServer();

	void Listen(short port);
	void Listen(std::string ip, short port);
	bool removeClient(Socket *client);

private:
	std::vector<std::thread *> threads;
	std::vector<Socket *> clients;
	TcpConnectionHandlerFactory *connHandlerFactory;
	Socket *socket;
	short port;
	std::string ip;

	void _Listen();
};