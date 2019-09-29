#include "TcpConnectionHandler.h"
#include "TcpServer.h"

TcpConnectionHandler::~TcpConnectionHandler()
{
	if (socket)
		server->Disconnect(socket);
}

void TcpConnectionHandler::SetSocket(std::shared_ptr<Socket> socket)
{
	this->socket = socket;
}

void TcpConnectionHandler::SetServer(std::shared_ptr<TcpServer> server)
{
	this->server = server;
}