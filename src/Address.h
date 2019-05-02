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

	short getPort();
	std::string getIP();
	struct sockaddr_in* getRawAddress();
	std::string toString();

private:
	struct sockaddr_in addr;	
	void fill_structure(short port);
};

#endif /* ADDRESS_H */