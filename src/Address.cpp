#include "Address.h"
#include <exception>
#include <string>

Address::Address(short port) 
{
   	addr.sin_addr.s_addr = INADDR_ANY;
   	fill_structure(port);
}

Address::Address(std::string address, short port) 
{
	std::string ip = NetworkUtils::getHostByName(address);	
   	addr.sin_addr.s_addr = inet_addr(ip.c_str());
   	fill_structure(port);
}

Address::Address(const Address& address) 
{
	addr = address.addr;
}

Address::Address(struct sockaddr_in addr) 
{
	this->addr = addr;
}

void Address::fill_structure(short port) 
{
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	memset(&(addr.sin_zero), '\0', 8);
}

short Address::getPort() 
{
	return ntohs(addr.sin_port);
}

std::string Address::getIP() 
{
	return std::string(inet_ntoa(addr.sin_addr));
}

std::string Address::toString() 
{
	std::string ip = getIP();
	std::string port = std::to_string(getPort());
	return ip + ":" + port;
}

struct sockaddr_in* Address::getRawAddress() 
{
	return &addr;
}