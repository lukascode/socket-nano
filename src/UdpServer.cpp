#include "UdpServer.h"

static void handleDatagram(UdpDatagramHandler *handler);
static void joinFinishedThreads(std::vector<std::thread *> *threads);

UdpServer::UdpServer(UdpDatagramHandlerFactory *datagramHandlerFactory)
{
	this->datagramHandlerFactory = datagramHandlerFactory;
}

UdpServer::~UdpServer()
{
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
	socket = Socket::CreateSocket(SOCK_DGRAM);
	Address *address = ip.empty() ? new Address(port) : new Address(ip, port);
	socket->Bind(address);

	std::thread _wait(joinFinishedThreads, &threads);
	for (;;)
	{
		Address *client;
		std::vector<uint8_t> datagram = socket->RecvFrom(client, 1024);
		UdpDatagramHandler *handler = datagramHandlerFactory->CreateUdpDatagramHandler();
		handler->SetSocket(socket);
		handler->SetDatagram(std::string(datagram.begin(), datagram.end()));
		handler->SetContext(this);
		handler->SetAddress(client);

		std::thread *thread = new std::thread(handleDatagram, handler);
		threads.push_back(thread);
	}
	_wait.join();
}

static void handleDatagram(UdpDatagramHandler *handler)
{
	handler->handleDatagram();
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