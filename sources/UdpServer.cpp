#include "UdpServer.h"

UdpServer::UdpServer(Address address, ServerConnectionHandlerFactory* connHandlerFactory) : Server(address, connHandlerFactory) {

}

Socket* UdpServer::createSocket() {
	int err;
	Socket* socket = new Socket(SOCK_DGRAM, &err);
	if(err == -1) {
		delete socket;
		return NULL;
	}
	return socket;
}

int UdpServer::onListen() {

	ServerConnectionHandlerFactory* connHandlerFactory = Server::getConnHandlerFactory();
	Socket* serverSocket = Server::getSocket();

	char buf[BSIZE];
	struct sockaddr_in client_addr;
	int numbytes;
	socklen_t addr_len = sizeof(struct sockaddr);
	while(1) {
		if((numbytes = recvfrom(serverSocket->getDescriptor(), buf, 0, 0, (struct sockaddr*)&client_addr, &addr_len))==-1) {
			continue;
		}
		int err;
		Socket* client = new Socket(SOCK_DGRAM, &err); //create new socket client
		if(err == -1) continue;
		int ret = connect(client->getDescriptor(), (struct sockaddr*)&client_addr, addr_len);
		if(ret == -1) continue;
		clients.push_back(client); // TODO
		ServerConnectionHandler* handler = connHandlerFactory->createServerConnectionHandler();
		handler->setSocket(client);
		handler->setContext(this);

		//handle connection in thread
		pthread_t thread;
		pthread_create(&thread, NULL, handleConnection, (void*)handler);

	}

}