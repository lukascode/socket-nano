#include "TcpConnectionHandler.h"
#include "TcpServer.h"

TcpConnectionHandler::~TcpConnectionHandler()
{
	if (socket)
		server->Disconnect(socket);
}

void TcpConnectionHandler::SetSocket(Socket *socket)
{
	this->socket = socket;
}

void TcpConnectionHandler::SetServer(TcpServer *server)
{
	this->server = server;
}