#include "Server.h"

Server::Server(Address address, ServerConnectionHandlerFactory* connHandlerFactory) {
	this->address = new Address(address);
	this->connHandlerFactory = connHandlerFactory;
	socket = NULL;
}

Server::~Server() {
	if(address) delete address;
	if(socket) delete socket;
	if(connHandlerFactory) delete connHandlerFactory;

	for(int i=0; i<clients.size(); ++i) {
		if(clients[i]) delete clients[i];
	} clients.clear();
}

int Server::Listen() {

	//socket
	socket = createSocket();
	if(socket == NULL) { return -1; }

	//for disturbing message "address already in use"
	int yes=1;
	if( setsockopt(socket->getDescriptor(), SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) < 0 ) {
		perror("setsockopt");
		return -1;
	}

	//bind
	int ret = bind(socket->getDescriptor(), (struct sockaddr*)address->getAddr(), sizeof(struct sockaddr));
	if(ret < 0) {
		perror("bind");
		return ret;
	}

	ret = onListen();
	if(ret < 0) return ret;

	return 0;
}

Address* Server::getAddress() { return address; }

Socket* Server::getSocket() { return socket; }

std::vector<Socket*>* Server::getClients() { return &clients; }

bool Server::removeClient(Socket* client) {
	auto it = std::find(clients.begin(), clients.end(), client);
	if(it != clients.end()) {
		clients.erase(it);
		client->closeSocket();
		delete client;
		return true;
	}
	return false;
}

ServerConnectionHandlerFactory* Server::getConnHandlerFactory() { return connHandlerFactory; }
