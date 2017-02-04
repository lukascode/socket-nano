#include "Client.h"

Client::Client(Address serverAddress, int type, ClientRequestHandler* reqHandler) {
	this->serverAddress = new Address(serverAddress);
	this->reqHandler = reqHandler;
	this->type = type;
	socket = NULL;
}

Client::~Client() {
	if(serverAddress) delete serverAddress;
	if(reqHandler) delete reqHandler;
	if(socket) delete socket;
}

int Client::Connect() {

	int err;
	socket = new Socket(type, &err);
	if(err < 0) { return err; }

	int ret = connect(socket->getDescriptor(), (struct sockaddr*)serverAddress->getAddr(), sizeof(struct sockaddr));
	if(ret < 0) {
		perror("connect");
		return -1;
	}

	reqHandler->handleConnection(socket, this);

}

Address* Client::getServerAddress() { return serverAddress; }

ClientRequestHandler* Client::getReqHandler() { return reqHandler; }

Socket* Client::getSocket() { return socket; }