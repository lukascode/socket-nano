#include "ServerConnectionHandler.h"

void ServerConnectionHandler::setSocket(Socket* socket) 
{
	this->socket = socket;
}

void ServerConnectionHandler::setContext(Server* context) 
{
	this->context = context;
}