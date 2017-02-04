#ifndef ADDRESS_H
#define ADDRESS_H

#include "NetworkUtils.h"


enum ADRESS_TYPE { IP, NAME };

class Address {

public:


	Address(short port);
	Address(std::string adr, short port, ADRESS_TYPE TYPE);
	Address(struct sockaddr_in addr);

	short getPort();
	std::string getIP();
	bool isBroken();
	struct sockaddr_in* getAddr();
	std::string toString();

private:
	struct sockaddr_in addr;	

	void fill_structure(short port);

	bool broken;

};

#endif /* ADDRESS_H */