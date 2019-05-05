#ifndef TCP_SERVER_H
#define TCP_SERVER_H

#include <thread>
#include <algorithm>
#include "Address.h"
#include "TcpConnectionHandlerFactory.h"

class TcpServer
{
public:
	TcpServer(short port, TcpConnectionHandlerFactory* connHandlerFactory);
	TcpServer(std::string ip, short port, TcpConnectionHandlerFactory* connHandlerFactory);
	~TcpServer();

	void Listen();
	bool removeClient(Socket* client);
private:
    std::vector<std::thread*> connections;
	std::vector<Socket*> clients;
	TcpConnectionHandlerFactory* connHandlerFactory;
	Socket* socket;
	short port;
	std::string ip;
};

#endif /* TCP_SERVER_H */