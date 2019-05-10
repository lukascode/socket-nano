#include "TcpConnectionHandler.h"

TcpConnectionHandler::~TcpConnectionHandler()
{
	if (socket)
		context->removeClient(socket);
}

void TcpConnectionHandler::setSocket(Socket *socket)
{
	this->socket = socket;
}

void TcpConnectionHandler::setContext(TcpServer *context)
{
	this->context = context;
}