#include "UdpServer.h"

UdpServer::UdpServer() {}

UdpServer::~UdpServer()
{
	if (socket)
		delete socket;
}

void UdpServer::Listen(short port)
{
	Listen("", port);
}

void UdpServer::Listen(std::string ip, short port)
{
	this->ip = ip;
	this->port = port;
	_Listen();
}

int UdpServer::onListen()
{

	ServerConnectionHandlerFactory *connHandlerFactory = Server::getConnHandlerFactory();
	Socket *serverSocket = Server::getSocket();

	char buf[BSIZE];
	struct sockaddr_in client_addr;
	int numbytes;
	socklen_t addr_len = sizeof(struct sockaddr);
	while (1)
	{
		if ((numbytes = recvfrom(serverSocket->getDescriptor(), buf, 0, 0, (struct sockaddr *)&client_addr, &addr_len)) == -1)
		{
			continue;
		}
		Socket *client = new Socket(SOCK_DGRAM); //create new socket client
		int ret = connect(client->getDescriptor(), (struct sockaddr *)&client_addr, addr_len);
		if (ret == -1)
			continue;
		clients.push_back(client); // TODO
		ServerConnectionHandler *handler = connHandlerFactory->createServerConnectionHandler();
		handler->setSocket(client);
		handler->setContext(this);
	}
}