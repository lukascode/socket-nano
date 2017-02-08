#include "ClientRequestHandler.h"

void ClientRequestHandler::setSocket(Socket* socket) {
	this->socket = socket;
}

void ClientRequestHandler::setContext(Client* context) {
	this->context = context;
}