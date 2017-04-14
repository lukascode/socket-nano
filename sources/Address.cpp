#include "Address.h"
#include <exception>
#include <string>

Address::Address(short port) {
   addr.sin_addr.s_addr = INADDR_ANY;
   fill_structure(port);
}

Address::Address(std::string adr, short port, ADRESS_TYPE TYPE) {
	std::string ip = adr;
	if(TYPE == NAME) {
		if( NetworkUtils::getHostByName(adr, &ip) < 0 ) {
            throw std::invalid_argument("name \""+adr+"\" could not be resolved");
        }	
	} 
   addr.sin_addr.s_addr = inet_addr(ip.c_str());
   fill_structure(port);
}

Address::Address(struct sockaddr_in addr) {
	this->addr = addr;
}

void Address::fill_structure(short port) {
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	memset(&(addr.sin_zero), '\0', 8);
}

short Address::getPort() {
	return ntohs(addr.sin_port);
}

std::string Address::getIP() {
	std::string ip(inet_ntoa(addr.sin_addr));
	return ip;
}

std::string Address::toString() {
	std::string str = "IP: " + getIP() + ", PORT: " + std::to_string((unsigned short)getPort());
	return str;
}

struct sockaddr_in* Address::getAddr() {
	return &addr;
}