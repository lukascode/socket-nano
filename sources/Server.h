#ifndef SERVER_H
#define SERVER_H

#include "NetworkUtils.h"
#include "Address.h"
#include "Socket.h"
#include <vector>
#include "ServerConnectionHandler.h"
#include <algorithm>

#define BACKLOG 10

extern void* handleConnection(void* arg); /* thread connection handler */

struct bundle {
	ServerConnectionHandler* handler;
	Socket* client;
	Server* context;
};

class Server {

public:
	Server(Address address, ServerConnectionHandler* connHandler);
	~Server();
	int Listen(); //factory method

	Address* getAddress();
	Socket* getSocket();
	std::vector<Socket*>* getClients();
	bool removeClient(Socket* client);
	ServerConnectionHandler* getConnHandler();

protected:
	virtual Socket* createSocket()=0;
	virtual int onListen()=0;

	std::vector<Socket*> clients;

private:
	Address* address;
	Socket* socket;
	ServerConnectionHandler* connHandler;

};

#endif /* SERVER_H */