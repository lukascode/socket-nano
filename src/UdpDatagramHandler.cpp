#include "UdpDatagramHandler.h"
#include "UdpServer.h"

void UdpDatagramHandler::SetSocket(std::shared_ptr<Socket> socket)
{
    this->socket = socket;
}

void UdpDatagramHandler::SetServer(std::shared_ptr<UdpServer> server)
{
    this->server = server;
}

void UdpDatagramHandler::SetAddress(std::shared_ptr<Address> address)
{
    this->address = address;
}

void UdpDatagramHandler::SetDatagram(std::string datagram)
{
    this->datagram = datagram;
}