#include "TcpServer.h"

TcpServer::TcpServer(Address address, ServerConnectionHandlerFactory* connHandlerFactory) : Server(address, connHandlerFactory) {

}

Socket* TcpServer::createSocket() {
	int err;
	Socket* socket = new Socket(SOCK_STREAM, &err);
	if(err == -1) {
		delete socket;
		return NULL;
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

		//handle connection in thread
		pthread_t thread;
		pthread_create(&thread, NULL, handleConnection, (void*)handler);

	}

	return 0;
}