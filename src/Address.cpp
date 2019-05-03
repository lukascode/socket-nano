#include "Address.h"
#include <exception>
#include <string>

Address::Address(short port): addr(tempaddr(port)) {}

Address::Address(std::string address, short port): addr(tempaddr(address, port)) {}

Address::Address(const Address& address): addr(address.addr) {}

Address::Address(struct sockaddr_in addr): addr(addr) {}

short Address::getPort() const
{
	return ntohs(addr.sin_port);
}

std::string Address::getIP() const
{
	return std::string(inet_ntoa(addr.sin_addr));
}

std::string Address::toString() const
{
	std::string ip = getIP();
	std::string port = std::to_string(getPort());
	return ip + ":" + port;
}

const struct sockaddr_in* Address::getRawAddress() const
{
	return &addr;
}

struct sockaddr_in Address::tempaddr(std::string address, short port) 
{
	std::string ip = NetworkUtils::getHostByName(address);
	struct sockaddr_in addr = tempaddr(port);
   	addr.sin_addr.s_addr = inet_addr(ip.c_str());
	return addr;
}

struct sockaddr_in Address::tempaddr(short port) {
	sockaddr_in addr;
	addr.sin_addr.s_addr = INADDR_ANY;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	memset(&(addr.sin_zero), '\0', 8);
	return addr;	
}