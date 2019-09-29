#pragma once

#include "NetworkUtils.h"

class Address
{
public:
	/// Creates address object using provided port
	Address(uint16_t port);

	/// Creates address object using provided address and port
	Address(std::string address, uint16_t port);

	/// Creates address object using provided low level address structure
	Address(struct sockaddr_in addr);

	/// Copy constructor
	Address(const Address &address);

	/// Gets the assign port to the address
	uint16_t GetPort() const;

	/// Gets the assign ip to the address
	std::string GetIP() const;

	/// Gets string representation of an address
	std::string ToString() const;

	/// Gets low level address structure
	const struct sockaddr_in *GetRawAddress() const;

private:
	const struct sockaddr_in addr;
	static sockaddr_in Tempaddr(std::string address, uint16_t port);
	static sockaddr_in Tempaddr(uint16_t port);
};