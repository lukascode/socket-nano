#include "ClientConnectionHandler.h"

void ClientConnectionHandler::setSocket(Socket* socket) 
{
	this->socket = socket;
}

void ClientConnectionHandler::setContext(Client* context) 
{
	this->context = context;
}