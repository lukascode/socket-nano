#include "Client.h"

Client::Client(Address serverAddress, int type, ClientConnectionHandler* reqHandler) 
{
	this->serverAddress = new Address(serverAddress);
	this->reqHandler = reqHandler;
	this->type = type;
	socket = NULL;
}

Client::~Client() 
{
	if(serverAddress) delete serverAddress;
	if(reqHandler) delete reqHandler;
	if(socket) delete socket;
}

int Client::Connect() 
{
	try {
		socket = new Socket(type, 0);
	} catch(...) { throw; }

	int ret = connect(socket->getDescriptor(), (struct sockaddr*)serverAddress->getAddr(), sizeof(struct sockaddr));
	if(ret < 0) {
		std::string err(strerror(errno));
		throw ConnectException("connect: " + err);
	}
	reqHandler->setSocket(socket);
	reqHandler->setContext(this);
	reqHandler->handleConnection();
	return 0;
}

Address* Client::getServerAddress() { return serverAddress; }

ClientConnectionHandler* Client::getReqHandler() { return reqHandler; }

Socket* Client::getSocket() { return socket; }