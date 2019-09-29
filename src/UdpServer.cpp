#include "UdpServer.h"

std::shared_ptr<UdpServer> UdpServer::Create(std::function<std::shared_ptr<UdpDatagramHandler>()> datagramHandlerFactory)
{
	return std::shared_ptr<UdpServer>(new UdpServer(datagramHandlerFactory));
}

UdpServer::UdpServer(std::function<std::shared_ptr<UdpDatagramHandler>()> datagramHandlerFactory)
{
	listening = false;
	tpSize = defaultThreadPoolSize;
	this->datagramHandlerFactory = datagramHandlerFactory;
}

UdpServer::~UdpServer()
{
	Clean();
}

void UdpServer::Listen(uint16_t port)
{
	Listen("", port);
}

void UdpServer::Listen(std::string ip, uint16_t port)
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
	tp = std::make_shared<ThreadPool>(tpSize);
	socket = Socket::Create(SOCK_DGRAM);
	auto address = ip.empty() ? std::make_shared<Address>(port) : std::make_shared<Address>(ip, port);
	ip = address->GetIP();
	socket->Bind(address);

	listening = true;
	halted = false;

	while (!halted.load())
	{
		std::shared_ptr<Address> client;
		std::vector<uint8_t> datagram = socket->RecvFrom(client, 1024);

		if (halted.load())
			break;

		auto handler = datagramHandlerFactory();
		handler->SetSocket(socket);
		handler->SetDatagram(std::string(datagram.begin(), datagram.end()));
		handler->SetServer(shared_from_this());
		handler->SetAddress(client);

		// handle datagram
		std::function<void()> task = [handler] {
			handler->HandleDatagram();
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
		socket.reset();

	listening = false;

	if (tp)
		tp.reset();
}

void UdpServer::Stop()
{
	halted = true;

	// Send datagram to this server to unblock recvfrom syscall
	auto s = Socket::Create(SOCK_DGRAM);
	s->SendTo(std::make_shared<Address>(this->ip, this->port), "Stop");
}