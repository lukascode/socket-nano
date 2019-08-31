#include "TcpServer.h"

TcpServer::TcpServer(std::function<TcpConnectionHandler*()> connHandlerFactory)
{
	tpSize = defaultThreadPoolSize;
	this->connHandlerFactory = connHandlerFactory;
}

TcpServer::~TcpServer()
{
	if (socket)
		delete socket;

	if (tp)
		delete tp;

	for (size_t i = 0; i < clients.size(); ++i)
	{
		if (clients[i])
			delete clients[i];
	}
	clients.clear();
}

void TcpServer::Listen(short port)
{
	Listen("", port);
}

void TcpServer::Listen(std::string ip, short port)
{
	this->ip = ip;
	this->port = port;
	_Listen();
}

void TcpServer::_Listen()
{
	tp = new ThreadPool(tpSize);
	socket = Socket::CreateSocket(SOCK_STREAM);
	Address *address = ip.empty() ? new Address(port) : new Address(ip, port);
	socket->Bind(address);
	socket->Listen(20);

	for (;;)
	{
		Socket *client_socket = socket->Accept();
		clients.push_back(client_socket);
		TcpConnectionHandler *handler = connHandlerFactory();
		handler->SetSocket(client_socket);
		handler->SetContext(this);

		// handle connection
		std::function<void()> task = [handler] {
			handler->HandleConnection();
			delete handler;
		};
		tp->SubmitTask(task);
	}
}

bool TcpServer::RemoveClient(Socket *client)
{
	auto it = std::find(clients.begin(), clients.end(), client);
	if (it != clients.end())
	{
		clients.erase(it);
		delete client;
		return true;
	}
	return false;
}

void TcpServer::setThreadPoolSize(int size)
{
	tpSize = size;
}