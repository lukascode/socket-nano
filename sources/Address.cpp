#include "Address.h"

#include <string>

Address::Address(short port) {
   addr.sin_addr.s_addr = INADDR_ANY;
   fill_structure(port);
   broken = false;
}

Address::Address(std::string adr, short port, ADRESS_TYPE TYPE) {
	std::string ip = adr;
	if(TYPE == NAME) {
		if( NetworkUtils::getHostByName(adr, &ip) < 0 ) 
			broken = true;
		else broken = false;
	} else broken = false;
   addr.sin_addr.s_addr = inet_addr(ip.c_str());
   fill_structure(port);
}

Address::Address(struct sockaddr_in addr) {
	this->addr = addr;
	broken = false;
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

bool Address::isBroken() {
	return broken;
}

std::string Address::toString() {
	std::string str = "IP: " + getIP() + ", PORT: " + std::to_string((unsigned short)getPort()) + ", isBroken: ";
	if(isBroken()) str += "true"; else str += "false";
	return str;
}

struct sockaddr_in* Address::getAddr() {
	return &addr;
}