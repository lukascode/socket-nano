#include "UdpServer.h"

UdpServer::UdpServer(std::function<UdpDatagramHandler *()> datagramHandlerFactory)
{
	listening = false;
	tpSize = defaultThreadPoolSize;
	this->datagramHandlerFactory = datagramHandlerFactory;
}

UdpServer::~UdpServer()
{
	Clean();
}

void UdpServer::Listen(short port)
{
	Listen("", port);
}

void UdpServer::Listen(std::string ip, short port)
{
	if (IsListening())
	{
		throw UdpServerException("Already listening");
	}
	this->ip = ip;
	this->port = port;
	_Listen();
}

void UdpServer::_Listen()
{
	tp = new ThreadPool(tpSize);
	socket = Socket::CreateSocket(SOCK_DGRAM);
	Address *address = ip.empty() ? new Address(port) : new Address(ip, port);
	ip = address->GetIP();
	socket->Bind(address);

	listening = true;
	halted = false;

	while (!halted.load())
	{
		Address *client;
		std::vector<uint8_t> datagram = socket->RecvFrom(client, 1024);

		if (halted.load())
			break;

		UdpDatagramHandler *handler = datagramHandlerFactory();
		handler->SetSocket(socket);
		handler->SetDatagram(std::string(datagram.begin(), datagram.end()));
		handler->SetServer(this);
		handler->SetAddress(client);

		// handle datagram
		std::function<void()> task = [handler] {
			handler->HandleDatagram();
			delete handler;
		};
		tp->SubmitTask(task);
	}
	Clean();
}

void UdpServer::SetThreadPoolSize(int size)
{
	tpSize = size;
}

bool UdpServer::IsListening()
{
	return listening.load();
}

void UdpServer::Clean()
{
	if (socket)
		delete socket;

	listening = false;
	
	if (tp)
		delete tp;
}

void UdpServer::Stop()
{
	halted = true;

	// Send datagram to this server to unblock recvfrom syscall
	Socket *s = Socket::CreateSocket(SOCK_DGRAM);
	s->SendTo(new Address(this->ip, this->port), "Stop");
	delete s;
}