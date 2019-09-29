#include "TcpServer.h"

std::shared_ptr<TcpServer> TcpServer::Create(std::function<std::shared_ptr<TcpConnectionHandler>()> connHandlerFactory)
{
	return std::shared_ptr<TcpServer>(new TcpServer(connHandlerFactory));
}

TcpServer::TcpServer(std::function<std::shared_ptr<TcpConnectionHandler>()> connHandlerFactory)
{
	listening = false;
	tpSize = defaultThreadPoolSize;
	this->connHandlerFactory = connHandlerFactory;
}

TcpServer::~TcpServer()
{
	Clean();
}

void TcpServer::Listen(uint16_t port)
{
	Listen("", port);
}

void TcpServer::Listen(std::string ip, uint16_t port)
{
	if (IsListening())
	{
		throw TcpServerException("Already listening");
	}
	this->ip = ip;
	this->port = port;
	_Listen();
}

void TcpServer::_Listen()
{
	tp = std::make_shared<ThreadPool>(tpSize);
	socket = Socket::Create(SOCK_STREAM);
	auto address = ip.empty() ? std::make_shared<Address>(port) : std::make_shared<Address>(ip, port);
	ip = address->GetIP();

	socket->Bind(address);
	socket->Listen(20);

	listening = true;
	halted = false;

	while (!halted.load())
	{
		auto client_socket = socket->Accept();

		if (halted.load())
			break;

		clients.push_back(client_socket);
		auto handler = connHandlerFactory();
		handler->SetSocket(client_socket);
		handler->SetServer(shared_from_this());

		// handle connection
		std::function<void()> task = [handler] {
			handler->HandleConnection();
		};
		tp->SubmitTask(task);
	}
	Clean();
}

void TcpServer::Clean()
{
	if (socket)
		socket.reset();

	listening = false;

	if (tp)
		tp.reset();

	clients.clear();
}

bool TcpServer::Disconnect(std::shared_ptr<Socket> client)
{
	auto it = std::find(clients.begin(), clients.end(), client);
	if (it != clients.end())
	{
		clients.erase(it);
		return true;
	}
	return false;
}

void TcpServer::Broadcast(std::string &data) const
{
	Broadcast(data, nullptr);
}

void TcpServer::Broadcast(std::string &data, std::shared_ptr<Socket> socket) const
{
	for (size_t i = 0; i < clients.size(); ++i)
	{
		if (clients[i] && (!socket || clients[i] != socket))
		{
			clients[i]->SendAll(data);
		}
	}
}

void TcpServer::SetThreadPoolSize(int size)
{
	tpSize = size;
}

size_t TcpServer::GetNumberOfConnections()
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
	auto s = Socket::Create(SOCK_STREAM);
	s->Connect(std::make_shared<Address>(this->ip, this->port));
}

bool TcpServer::IsListening()
{
	return listening.load();
}