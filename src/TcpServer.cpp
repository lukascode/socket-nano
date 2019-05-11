#include "TcpServer.h"

static void handleConnection(TcpConnectionHandler *handler);
static void joinFinishedThreads(std::vector<std::thread *> *threads);

TcpServer::TcpServer(TcpConnectionHandlerFactory *connHandlerFactory)
{
	this->connHandlerFactory = connHandlerFactory;
}

TcpServer::~TcpServer()
{
	if (connHandlerFactory)
		delete connHandlerFactory;
	if (socket)
		delete socket;

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
	socket = Socket::CreateSocket(SOCK_STREAM);
	Address *address = ip.empty() ? new Address(port) : new Address(ip, port);
	socket->Bind(address);
	socket->Listen(20);

	// join connection threads
	std::thread _wait(joinFinishedThreads, &threads);
	for (;;)
	{
		Socket *client_socket = socket->Accept();
		clients.push_back(client_socket);
		TcpConnectionHandler *handler = connHandlerFactory->CreateTcpConnectionHandler();
		handler->SetSocket(client_socket);
		handler->SetContext(this);

		// handle connection in std::thread
		std::thread *thread = new std::thread(handleConnection, handler);
		threads.push_back(thread);
	}
	_wait.join();
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

static void handleConnection(TcpConnectionHandler *handler)
{
	handler->HandleConnection();
	delete handler;
}

static void joinFinishedThreads(std::vector<std::thread *> *threads)
{
	for (;;)
	{
		for (size_t i = 0; i < threads->size(); ++i)
		{
			(*threads)[i]->join();
			delete (*threads)[i];
			threads->erase(threads->begin() + i);
		}
		sleep(1);
	}
}