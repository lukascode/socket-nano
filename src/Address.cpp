#include "Address.h"
#include <exception>
#include <string>

Address::Address(uint16_t port) : addr(Tempaddr(port)) {}

Address::Address(std::string address, uint16_t port) : addr(Tempaddr(address, port)) {}

Address::Address(const Address &address) : addr(address.addr) {}

Address::Address(struct sockaddr_in addr) : addr(addr) {}

uint16_t Address::GetPort() const
{
	return ntohs(addr.sin_port);
}

std::string Address::GetIP() const
{
	return std::string(inet_ntoa(addr.sin_addr));
}

std::string Address::ToString() const
{
	std::string ip = GetIP();
	std::string port = std::to_string(GetPort());
	return ip + ":" + port;
}

const struct sockaddr_in *Address::GetRawAddress() const
{
	return &addr;
}

struct sockaddr_in Address::Tempaddr(std::string address, uint16_t port)
{
	std::string ip = NetworkUtils::GetHostByName(address);
	struct sockaddr_in addr = Tempaddr(port);
	addr.sin_addr.s_addr = inet_addr(ip.c_str());
	return addr;
}

struct sockaddr_in Address::Tempaddr(uint16_t port)
{
	sockaddr_in addr;
	addr.sin_addr.s_addr = INADDR_ANY;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	memset(&(addr.sin_zero), '\0', 8);
	return addr;
}