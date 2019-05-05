#include "TcpConnectionHandler.h"

void TcpConnectionHandler::setSocket(Socket* socket) 
{
	this->socket = socket;
}

void TcpConnectionHandler::setContext(TcpServer* context) 
{
	this->context = context;
}