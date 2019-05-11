#include "TcpConnectionHandler.h"

TcpConnectionHandler::~TcpConnectionHandler()
{
	if (socket)
		context->RemoveClient(socket);
}

void TcpConnectionHandler::SetSocket(Socket *socket)
{
	this->socket = socket;
}

void TcpConnectionHandler::SetContext(TcpServer *context)
{
	this->context = context;
}