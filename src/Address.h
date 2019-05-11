#pragma once

#include "NetworkUtils.h"

class Address 
{
public:
	Address(short port);
	Address(std::string address, short port);
	Address(struct sockaddr_in addr);
	Address(const Address& address);

	short GetPort() const;
	std::string GetIP() const;
	std::string ToString() const;
	const struct sockaddr_in* GetRawAddress() const;

private:
	const struct sockaddr_in addr;	
	static sockaddr_in Tempaddr(std::string address, short port);
	static sockaddr_in Tempaddr(short port);
};