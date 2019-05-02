#ifndef SERVER_H
#define SERVER_H

#include "NetworkUtils.h"
#include "Address.h"
#include "Socket.h"
#include <vector>
#include "ServerConnectionHandler.h"
#include "ServerConnectionHandlerFactory.h"
#include <algorithm>

#define BACKLOG 10

class Server 
{

public:
	Server(Address address, ServerConnectionHandlerFactory* connHandlerFactory);
	~Server();
	int Listen(); //factory method

	Address* getAddress();
	Socket* getSocket();
	std::vector<Socket*>* getClients();
	bool removeClient(Socket* client);
	ServerConnectionHandlerFactory* getConnHandlerFactory();

protected:
	virtual Socket* createSocket()=0;
	virtual int onListen()=0;

	std::vector<Socket*> clients;

private:
	Address* address;
	Socket* socket;
	ServerConnectionHandlerFactory* connHandlerFactory;

};

#endif /* SERVER_H */