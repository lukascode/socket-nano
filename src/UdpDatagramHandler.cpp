#include "UdpDatagramHandler.h"
#include "UdpServer.h"

UdpDatagramHandler::~UdpDatagramHandler()
{
    if (address)
        delete address;
}

void UdpDatagramHandler::SetSocket(Socket *socket)
{
    this->socket = socket;
}

void UdpDatagramHandler::SetContext(UdpServer *context)
{
    this->context = context;
}

void UdpDatagramHandler::SetDatagram(std::string datagram)
{
    this->datagram = datagram;
}

void UdpDatagramHandler::SetAddress(Address* address)
{
    this->address = address;
}