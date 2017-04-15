#include "TcpServer.h"

static void handleConnection(ServerConnectionHandler* handler);
static void joinFinishedThreads(std::vector<std::thread*>* connections);

TcpServer::TcpServer(Address address, ServerConnectionHandlerFactory* connHandlerFactory) : Server(address, connHandlerFactory) {

}

Socket* TcpServer::createSocket() {
	Socket* socket;
	try {
		socket = new Socket(SOCK_STREAM, 0);
	} catch(...) {
		delete socket;
		throw;
	} 
	return socket;
}

int TcpServer::onListen() {

	ServerConnectionHandlerFactory* connHandlerFactory = Server::getConnHandlerFactory();
	Socket* serverSocketListener = Server::getSocket();

	if( listen(serverSocketListener->getDescriptor(), BACKLOG) < 0 ) {
		perror("listen");
		return -1;
	}

	//run thread that join connections threads
	std::thread waitt(joinFinishedThreads, &connections);

	while(1) {
		struct sockaddr_in client_addr;
		socklen_t addrlen = sizeof(struct sockaddr_in);
		int client_socket = accept(serverSocketListener->getDescriptor(), (struct sockaddr*)&client_addr, &addrlen);
		if(client_socket < 0) continue;

		Socket* client = new Socket(client_socket);
		clients.push_back(client);

		ServerConnectionHandler* handler = connHandlerFactory->createServerConnectionHandler();
		handler->setSocket(client);
		handler->setContext(this);

		//handle connection in std::thread
		std::thread* t = new std::thread(handleConnection, handler);
		connections.push_back(t);

	}

	waitt.join();

	return 0;
}

static void handleConnection(ServerConnectionHandler* handler) {
	handler->handleConnection();
	delete handler;
}

static void joinFinishedThreads(std::vector<std::thread*>* connections) {
	while(1) {
		for(int i=0; i<connections->size(); ++i) {
			(*connections)[i]->join();
			delete (*connections)[i];
			connections->erase(connections->begin() + i);
		}
		sleep(1);
	}
}