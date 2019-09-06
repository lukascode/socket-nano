#include "TcpServer.h"

TcpServer::TcpServer(std::function<TcpConnectionHandler *()> connHandlerFactory)
{
	listening = false;
	tpSize = defaultThreadPoolSize;
	this->connHandlerFactory = connHandlerFactory;
}

TcpServer::~TcpServer()
{
	Clean();
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
	if (listening.load())
	{
		throw TcpServerException("Already listening");
	}
	tp = new ThreadPool(tpSize);
	socket = Socket::CreateSocket(SOCK_STREAM);
	Address *address = ip.empty() ? new Address(port) : new Address(ip, port);
	ip = address->GetIP();
	socket->Bind(address);
	socket->Listen(20);

	listening = true;
	halted = false;

	while (!halted.load())
	{
		Socket *client_socket = socket->Accept();

		if (halted.load())
			break;

		clients.push_back(client_socket);
		TcpConnectionHandler *handler = connHandlerFactory();
		handler->SetSocket(client_socket);
		handler->SetServer(this);

		// handle connection
		std::function<void()> task = [handler] {
			handler->HandleConnection();
			delete handler;
		};
		tp->SubmitTask(task);
	}
	Clean();
}

void TcpServer::Clean()
{
	if (socket)
		delete socket;

	listening = false;

	if (tp)
		delete tp;

	for (size_t i = 0; i < clients.size(); ++i)
	{
		if (clients[i])
			delete clients[i];
	}
	clients.clear();
}

bool TcpServer::Disconnect(Socket *client)
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

void TcpServer::Broadcast(std::string &data) const
{
	for (size_t i = 0; i < clients.size(); ++i)
	{
		if (clients[i])
		{
			clients[i]->SendAll(data);
		}
	}
}

void TcpServer::setThreadPoolSize(int size)
{
	tpSize = size;
}

size_t TcpServer::getNumberOfConnections()
{
	return clients.size();
}

void TcpServer::Stop()
{
	halted = true;

	// Disconnect all connections
	for (size_t i = 0; i < clients.size(); ++i)
	{
		if (clients[i])
		{
			clients[i]->Shutdown();
		}
	}

	// Connect to this server to unblock accept syscall
	Socket *s = Socket::CreateSocket(SOCK_STREAM);
	s->Connect(new Address(this->ip, this->port));
	delete s;
}

bool TcpServer::isListening()
{
	return listening.load();
}