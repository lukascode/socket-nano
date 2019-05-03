#ifndef ADDRESS_H
#define ADDRESS_H

#include "NetworkUtils.h"

class Address 
{
public:
	Address(short port);
	Address(std::string address, short port);
	Address(struct sockaddr_in addr);
	Address(const Address& address);

	short getPort() const;
	std::string getIP() const;
	std::string toString() const;
	const struct sockaddr_in* getRawAddress() const;

private:
	const struct sockaddr_in addr;	
	static sockaddr_in tempaddr(std::string address, short port);
	static sockaddr_in tempaddr(short port);
};

#endif /* ADDRESS_H */