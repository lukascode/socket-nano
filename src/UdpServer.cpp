#include "UdpServer.h"

UdpServer::UdpServer(UdpDatagramHandlerFactory *datagramHandlerFactory)
{
	tpSize = defaultThreadPoolSize;
	this->datagramHandlerFactory = datagramHandlerFactory;
}

UdpServer::~UdpServer()
{
	if (tp)
		delete tp;
	if (datagramHandlerFactory)
		delete datagramHandlerFactory;
	if (socket)
		delete socket;
}

void UdpServer::Listen(short port)
{
	Listen("", port);
}

void UdpServer::Listen(std::string ip, short port)
{
	this->ip = ip;
	this->port = port;
	_Listen();
}

void UdpServer::_Listen()
{
	tp = new ThreadPool(tpSize);
	socket = Socket::CreateSocket(SOCK_DGRAM);
	Address *address = ip.empty() ? new Address(port) : new Address(ip, port);
	socket->Bind(address);

	for (;;)
	{
		Address *client;
		std::vector<uint8_t> datagram = socket->RecvFrom(client, 1024);
		UdpDatagramHandler *handler = datagramHandlerFactory->CreateUdpDatagramHandler();
		handler->SetSocket(socket);
		handler->SetDatagram(std::string(datagram.begin(), datagram.end()));
		handler->SetContext(this);
		handler->SetAddress(client);

		// handle datagram
		std::function<void()> task = [handler] {
			handler->HandleDatagram();
			delete handler;
		};
		tp->SubmitTask(task);
	}
}

void UdpServer::setThreadPoolSize(int size)
{
	tpSize = size;
}